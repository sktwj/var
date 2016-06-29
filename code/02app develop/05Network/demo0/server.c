#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>

#define   MYPORT	6666
#define   BACKLOG	10

int client_handler(int conn_fd)
{
#define BUF_SIZE	1024
	char buf[BUF_SIZE] = {0};

	while (1) {	
		int n = read(conn_fd, buf, BUF_SIZE);
		if (n == -1) {
			perror("read");
			goto err;
		}
		if (n == 0) {
			fprintf(stderr, "connect close by peer\n");
			break;
		}
		printf("recv:[%d]%s", n, buf);
		int i;
		for (i = 0; i < n; ++i) {
			buf[i] = toupper(buf[i]);
		}
		write(conn_fd, buf, n);
	}
	close(conn_fd);
	return 0;
err:
	close(conn_fd);
	return -1;
}

void sig_handler(int signo)
{
	switch (signo) {
		case SIGCHLD:
			wait(NULL);
			break;
	}
}


int main()
{
	signal(SIGCHLD, sig_handler);
	signal(SIGPIPE, SIG_IGN);

	//1. 创建套接字
	int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	
	if (listen_fd == -1) {
		perror("socket");
		return -1;
	}

	//2.绑定地址和端口
	struct sockaddr_in srv_addr;
	//memset(&srv_addr, 0, sizeof(srv_addr));
	bzero(&srv_addr, sizeof(srv_addr));
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_port = htons(MYPORT);
   	inet_aton("0.0.0.0", &srv_addr.sin_addr);
	socklen_t  addr_len = sizeof(srv_addr);

	//地址和端口复用	
	int opt_addr_reuse = 1;
	setsockopt(listen_fd, SOL_SOCKET,SO_REUSEADDR, &opt_addr_reuse, sizeof(opt_addr_reuse));

	//绑定
	int iret = bind(listen_fd, (const struct sockaddr *)&srv_addr, addr_len);
	if (iret == -1) {
		perror("bind");
		return -1;
	}

	//3. listen
	iret = listen(listen_fd, BACKLOG);
	if (iret == -1) {
		perror("listen");
		return -1;
	}

	struct sockaddr_in cli_addr;
	
	//4. main_loop
	int conn_fd;
	while (1) {
		//5. accept
		conn_fd = accept(listen_fd, (struct sockaddr *)&cli_addr, &addr_len);
		if (conn_fd == -1) {
			perror("accept");
			break;
		}
		pid_t p = fork();
		//处理客户端请求
		if (p == -1) {
			perror("fork");
			break;
		}
		if (p == 0) {
			close(listen_fd);
			printf("fork new process: %d\n", getpid());
			printf("new connect from %s:%hu\n",
					inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));
			client_handler(conn_fd);
			return 0;
		}
		close(conn_fd);
	}
	return 0;
}
