#include <stdio.h>
#include <stdlib.h>
#include "server.h"


static volatile sig_atomic_t srv_shutdown = 0;
static int sock_fd = 0;
/*默认设置*/
struct up_cfg_info up_conf = {
	1234, //listen_port
	"../www",
	"../uphttpd.conf",
	"../log/uphttpd.log",
	0, //run as deamon
	1, //verbose
};


const char ok_response[] = 
"HTTP/1.1 200 OK\r\n"
"Server: "PRG_NAME" "VERSION"\r\n"
"Content-Type: text/html; charset=UTF-8\r\n"
"\r\n";

const char not_found_response[] = 
"HTTP/1.1 404 Not Found\r\n"
"Content-Type: text/html\r\n"
"\r\n"
"<html>\n"
"<body> <h1> 404 Not Found </h1> </body> \n"
"</html>\n";

int cli_fd[BACKLOG] = {0};

int sock_listen()
{
	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
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
	srv_addr.sin_port = htons(up_conf.listen_port);

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

	return 0;
}

void sig_handler(int signo)
{
	switch (signo) {
		case SIGINT:
			srv_shutdown = 1;
			break;
		case SIGCHLD:
			break;
	}
}


int server_init()
{
	//1. 服务器初始化
	if (sock_listen() < 0){
		ERR_PRINTF("sock_listen\n");
		return -1;
	}

	//2.信号处理
	signal(SIGINT, sig_handler);
	signal(SIGPIPE, SIG_IGN);

	//3.日志

	return 0;
}

int client_handler(int index)
{
#if 1
					int conn_fd = cli_fd[index];
					char buf[BUF_SIZE] = {0};

					int n = read(conn_fd, buf, BUF_SIZE);
					if (n == -1) {
						perror("recv");
					}
					if (n == 0) {
						DBG_PRINTF("connect closed by peer\n");
						cli_fd[index] = -1;
					}else {
						DBG_PRINTF("recv:%s\n", buf);
						char method[PATH_SIZE] = {0};
						char uri[PATH_SIZE] = {0};
						char http_ver[PATH_SIZE] = {0};
						sscanf(buf, "%s %s %s", method, uri, http_ver);
						DBG_PRINTF("method = %s\n", method);
						DBG_PRINTF("uri = %s\n", uri);
						DBG_PRINTF("http_ver = %s\n", http_ver);
						// uri == '/' --> /index.html
						char path[PATH_SIZE] = {0};
						snprintf(path, PATH_SIZE, "%s/index.html", up_conf.doc_root_path);
						DBG_PRINTF("path = %s", path);
						FILE *pf = fopen(path, "r");
						if (pf == NULL) {
							write(conn_fd, not_found_response, strlen(not_found_response) + 1);	
						} else {
						int n = fread(buf, 1, BUF_SIZE, pf);
						write(conn_fd, ok_response, strlen(ok_response) + 1);
						write(conn_fd, buf, n);
						fclose(pf);
						}
					}
#endif
	return 0;
}


int server_run()
{
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

					client_handler(i);
				}
			}
			if (FD_ISSET(sock_fd, &rdfds)) {
				//new connect coming
				int conn_fd = accept(sock_fd, (struct sockaddr *)&cli_addr, &addr_len);
				if (up_conf.verbose){
				DBG_PRINTF("new connection %s:%hu\n", inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));
				}
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

/*服务器端的销毁*/
int server_destroy()
{
	return 0;
}


int main(int argc, char *argv[])
{
	//1. 配置解析
	if (up_conf_init(argc, argv) <0){
		ERR_PRINTF("up_conf_init failed.\n");
		return -1;
	}
	
	
	//2.
    if (server_init() < 0) {
		ERR_PRINTF("server_init failed.\n");
		return -1;
	}

	server_run();
		

	return 0;
}
