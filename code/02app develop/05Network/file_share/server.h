#pragma once

#define  SRV_PORT		6666
#define  DEFAULT_PATH	.
#define  MAX_CLI_NUMS	100
#define  BACKLOG		20
#define  BUF_SIZE		1024

#define  ERR_UNKOWN_CMD	 "Error:unkown cmd\n"


struct srv_info {
	int sock_fd;
	int conn_cli_fds[MAX_CLI_NUMS]; //客户端fd的数组
	int pid; // srv pid
};
