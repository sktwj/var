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
#include <pthread.h>
#include <errno.h>
#include <time.h>
#include "list.h"

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
#define  DBG_PRINTF(fmt, args...)  	fprintf(stderr, fmt, ##args)
#endif


#define  ERR_PRINTF(fmt, args...)	\
		XPRINTF("ERR", fmt, ##args)


#define  MAX(a, b)	((a) > (b)? (a) : (b))
#define LENGTH_OF(x) (sizeof(x)/sizeof(x[0]))


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

enum { S_INIT, S_IDEL, S_RUNNING, S_DETACH } ; //worker的状态 
//工作线程
struct worker_info {
	int status;
	pthread_t tid;
	struct list_head list;
	int  conn_fd;
};
//主线程
struct master_info {
	struct list_head list;
	pthread_mutex_t mutex;
	pthread_cond_t  cond;
	size_t worker_nums;  //线程数量
	int sock_fd;
};
#define  IO_BUF_SIZE (16 * 1024)

struct iobuffer_t {
	int len;
	char buf[IO_BUF_SIZE];
};

struct server_info {
	struct up_cfg_info *pcfg;
	struct master_info *pmaster;
	struct worker_info *pworker;
	pid_t p;
};

#define STD_HEADER \
                   "Server: "PRG_NAME" "VERSION" \r\n" \
                   "Cache-Control: no-store, no-cache, must-revalidate, pre-check=0, post-check=0, max-age=0\r\n" \
                   "Pragma: no-cache\r\n" \
                   "Expires: Mon, 3 Jan 2000 12:34:56 GMT\r\n" \
                   "Connection: close\r\n" 

static const char ok_response[] = 
"HTTP/1.1 200 OK\r\n"
"Server: "PRG_NAME" "VERSION"\r\n"
"Content-Type: text/plain; charset=UTF-8\r\n"
"\r\n";

static const struct {
  const char *dot_extension;
  const char *mimetype;
} mimetypes[] = {
  { ".html", "text/html" },
  { ".htm",  "text/html" },
  { ".css",  "text/css" },
  { ".js",   "text/javascript" },
  { ".txt",  "text/plain" },
  { ".jpg",  "image/jpeg" },
  { ".jpeg", "image/jpeg" },
  { ".png",  "image/png"},
  { ".gif",  "image/gif" },
  { ".ico",  "image/x-icon" },
  { ".swf",  "application/x-shockwave-flash" },
  { ".cab",  "application/x-shockwave-flash" },
  { ".jar",  "application/java-archive" },
  { ".mp3",  "audio/mpeg"},
};

int server_destroy();
int
pool_add_worker (void *(*process) (void *arg), void *arg);

void
pool_init (int max_thread_num);
int
pool_destroy ();
