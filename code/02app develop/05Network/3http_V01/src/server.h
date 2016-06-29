#pragma once

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/time.h>
#include <signal.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <getopt.h>
#include <assert.h>
#include <ctype.h>

#define BUF_SIZE	1024
#define PRG_NAME	"UpHttpd"
#define VERSION		"0.0.1"
#define BACKLOG		100

#define   XPRINTF(level, fmt, args...) \
		fprintf(stderr, "[%s](%s:%d:%s)"fmt, level, __FILE__, __LINE__, __FUNCTION__, ##args)


#ifdef _DEBUG
#define  DBG_PRINTF(fmt, args...)	\
		XPRINTF("DEBUG", fmt, ##args)
#else
#define  DBG_PRINTF(fmt, args...)  
#endif


#define  ERR_PRINTF(fmt, args...)	\
		XPRINTF("ERR", fmt, ##args)


#define  MAX(a, b)	((a) > (b)? (a) : (b))

#define  PATH_SIZE	128
#define  DEF_PORT	1234
#define  DEF_DOC_ROOT	"../html"
#define  DEF_CONF_FILE	"../uphttpd.conf"
#define  DEF_LOG_FILE	"../uphttpd.log"

struct up_cfg_info {
	int listen_port; //监听的端口
	char doc_root_path[PATH_SIZE]; //文档根路径
	char conf_file[PATH_SIZE]; //配置文件路径
	char log_file[PATH_SIZE]; //日志文件路径
	int run_as_daemon; //是否以守护进程的方式运行
	int verbose; //显式具体执行流程方便调试
};

int up_conf_init(int argc, char *argv[]);

