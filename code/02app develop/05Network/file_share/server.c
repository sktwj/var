#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

struct srv_info srv_info;
static volatile sig_atomic_t srv_shutdown = 0;


void sig_handler(int signo)
{
	switch (signo) {
		case SIGCHLD:
			wait(NULL);
			break;
		case SIGINT:
			srv_shutdown = 1;
			break;	
	}
}

int cmd_parser(const char *cmd_buf, int buf_size, int conn_fd)
{
	printf("cmd=%s", cmd_buf); //
	fflush(stdout);
	if (strncmp(cmd_buf, "LIST", 4) == 0) {
		//list_handler();
	} else if (strncmp(cmd_buf, "DLOAD:", 6) == 0) {
		//download_handler();
	} else if (strncmp(cmd_buf, "ULOAD:", 6) == 0) {
		//upload_handler();
	} else if (strncmp(cmd_buf, "RM:", 3) == 0) {
		//remove
	} else {
		printf("cmd not support\n");
		write(conn_fd, ERR_UNKOWN_CMD, strlen(ERR_UNKOWN_CMD)+1);
		return -1;
	}


	return 0;
}

int cli_handler(int conn_fd)
{
	char buf[BUF_SIZE] = {0};

	while (!srv_shutdown) {
	int iret = read(conn_fd, buf, BUF_SIZE - 1);
	if (iret == 0) {
		fprintf(stderr, "Error: connect closed bye peer\n");
		return -1;
	}

	//解析命令
	cmd_parser(buf, iret, conn_fd);
	}

	return 0;
}

int srv_init()
{
	//srv_info.sock_fd 
	int sock_fd; 
	int iret; 
	//1. socket
	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_fd == -1) {
		perror("socket failed.");
		return -1;
	}
	//2. bind
	struct  sockaddr_in srv_addr;
	socklen_t addr_len = sizeof(srv_addr);
	bzero(&srv_addr, addr_len);
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_port = htons(SRV_PORT);
	srv_addr.sin_addr.s_addr = htonl(0);

	int opt_addr_reuse = 1;
	setsockopt(sock_fd, SOL_SOCKET,SO_REUSEADDR, &opt_addr_reuse, sizeof(opt_addr_reuse));

	iret = bind(sock_fd, (struct sockaddr *)&srv_addr, addr_len);
	if (iret == -1) {
		perror("bind failed.");
		return -1;
	}
	//3. listen
	iret = listen(sock_fd, BACKLOG);
	if (iret == -1) {
		perror("listen");
		return -1;
	}

	srv_info.pid = getpid();
	srv_info.sock_fd = sock_fd;
	int i;
	for (i = 0; i < MAX_CLI_NUMS; ++i) {
		srv_info.conn_cli_fds[i] = -1;
	}

	//信号处理函数
	signal(SIGCHLD, sig_handler);
	signal(SIGPIPE, SIG_IGN);

	return 0;
}

int srv_run()
{
	int conn_fd = 0;
	struct sockaddr_in cliaddr;
	bzero(&cliaddr, sizeof(cliaddr));
	socklen_t addrlen = 0;
	int i;

	while (! srv_shutdown) {
		conn_fd = accept(srv_info.sock_fd, 
			(struct sockaddr *)&cliaddr, &addrlen);
		if (conn_fd == -1) {
			perror("accept");
			continue;
		}
		for (i = 0; i < MAX_CLI_NUMS; ++i) {
			if (srv_info.conn_cli_fds[i] == -1) {
				srv_info.conn_cli_fds[i] = conn_fd;
				break;
			}
		}

		pid_t p = fork();
		if (p == -1) {
			perror("fork");
			return -1;
		}
		if (p == 0) {
			//child process
			close(srv_info.sock_fd);

			cli_handler(conn_fd);
			exit(0);
		}

		close(conn_fd);
	}
	close(srv_info.sock_fd);

	return 0;
}


int srv_destroy()
{
	//销毁对应的资源

	return 0;
}


int main()
{
	int iret = srv_init();
	if (iret == -1) {
		fprintf(stderr, "[Error]:srv_init failed.\n");
		return -1;
	}

	//main_loop
	srv_run();

	return 0;
}