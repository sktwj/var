#include "server.h"

#define  LISTEN_PORT	12345
#define  BACKLOG		5

int  cli_fd[BACKLOG];
static volatile sig_atomic_t srv_shutdown = 0;

int sock_init()
{
	int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_fd == -1){
		perror("socket");
		return -1;
	}

	//设置socket 非阻塞
	int flag = fcntl(sock_fd, F_GETFL); 
	flag |= O_NONBLOCK; 
	fcntl(sock_fd, F_SETFL, flag);
	
	struct sockaddr_in srv_addr;
	socklen_t addr_len = sizeof(srv_addr);

	bzero(&srv_addr, sizeof(srv_addr));
	srv_addr.sin_family = AF_INET;
	inet_pton(AF_INET, "0.0.0.0", &srv_addr.sin_addr);
	srv_addr.sin_port = htons(LISTEN_PORT);

	//端口复用
	int reuse_opt = 1;
	setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &reuse_opt, sizeof(reuse_opt));

	int iret = bind(sock_fd, (struct sockaddr *)&srv_addr, addr_len);
	
	if (iret == -1) {
		perror("bind");
		return -1;
	}

	iret = listen(sock_fd, BACKLOG);
	if (iret == -1) {
		perror("listen");
		return -1;
	}

	DBG_PRINTF("server is running, listen at %s:%hu\n", inet_ntoa(srv_addr.sin_addr), ntohs(srv_addr.sin_port));

	return sock_fd;
}

static void sig_handler(int signo)
{
	switch (signo) {
		case SIGINT:
			srv_shutdown = 1;
			break;
		case SIGPIPE:
			break;
	}
}
int main()
{
	int sock_fd = sock_init();
	
	signal(SIGINT, sig_handler);
	signal(SIGPIPE, sig_handler); // SIG_IGN

	struct sockaddr_in cli_addr;
	socklen_t addr_len = sizeof(cli_addr);

	fd_set rdfds;
	int max_fd = 0;
	int i;
	for (i = 0; i <sizeof cli_fd / sizeof cli_fd[0]; ++i) {
		cli_fd[i] = -1;
	}	

	struct timeval tv = {0, 0};

	while (!srv_shutdown) {
		FD_ZERO(&rdfds);
		FD_SET(sock_fd, &rdfds);
		max_fd = sock_fd;
		for (i = 0; i < sizeof cli_fd/ sizeof(cli_fd[0]); ++i) {
			if (cli_fd[i] != -1) {
				FD_SET(cli_fd[i], &rdfds);
				max_fd = MAX(max_fd, cli_fd[i]);
			}
		}
		tv.tv_sec = 2;
		tv.tv_usec = 0;
		i = select(max_fd + 1, &rdfds, NULL, NULL, &tv);
		if (i == -1) {
			perror("select");
			return -1;
		}else if (i == 0) {
			continue;
		}else{
			for (i = 0; i < BACKLOG; ++i) {
				if (cli_fd[i] == -1) {
					continue;
				}
				if (FD_ISSET(cli_fd[i], &rdfds)) {
					int conn_fd = cli_fd[i];
					char buf[BUF_SIZE] = {0};

					int n = read(conn_fd, buf, BUF_SIZE);
					//int n = recv(conn_fd, buf, BUF_SIZE, MSG_DONTWAIT);
					if (n == -1) {
						perror("recv");
					}
					if (n == 0) {
						DBG_PRINTF("connect closed by peer\n");
						FD_CLR(conn_fd, &rdfds);
						cli_fd[i] = -1;
					}else {
						DBG_PRINTF("recv:%s\n", buf);
						write(conn_fd, buf, n);
					}
				}
			}
			if (FD_ISSET(sock_fd, &rdfds)) {
				//new connect coming
				int conn_fd = accept(sock_fd, (struct sockaddr *)&cli_addr, &addr_len);
				DBG_PRINTF("new connection %s:%hu\n", inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));
				for (i = 0; i < BACKLOG; ++i) {
					if (cli_fd[i] == -1) {
						cli_fd[i] = conn_fd;
						FD_SET(conn_fd, &rdfds);
						break;
					}		 
				}
				if (i == BACKLOG) {
					ERR_PRINTF("client has reach the MAX limit\n");
					char buf[BUF_SIZE] = {0};
					i = snprintf(buf, BUF_SIZE,"[Error] client has reach the max limit of %d\n", BACKLOG);
					write(conn_fd, buf, i);
					close(conn_fd);
				}
			}
		}
	}

	for (i = 0; i < BACKLOG; ++i) {
		if (cli_fd[i] != -1) {
			close(cli_fd[i]);
		}
	}


	return 0;
}
