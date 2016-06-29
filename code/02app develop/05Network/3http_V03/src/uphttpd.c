#include <stdio.h>
#include <stdlib.h>

#include "server.h"


static volatile sig_atomic_t srv_shutdown = 0;
int srv_destroy = 0;
static int sock_fd = 0;
struct master_info *pmaster = NULL;

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
"<body> <h1> 404 Not Found </h1> </body>\n"
"</html>\n";

int cli_fd[BACKLOG] = {0};

void set_nonblock(int fd)
{
	int flag = fcntl(fd, F_GETFL); 
	flag |= O_NONBLOCK; 
	fcntl(fd, F_SETFL, flag);
}

int sock_listen()
{
	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_fd == -1){
		perror("socket");
		return -1;
	}

	//设置socket 非阻塞
	set_nonblock(sock_fd);
	
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
            server_destroy();
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

/******************************************************************************
Description.: Send error messages and headers.
Input Value.: * fd.....: is the filedescriptor to send the message to
              * which..: HTTP error code, most popular is 404
              * message: append this string to the displayed response
Return Value: -
******************************************************************************/
void send_error(int fd, int which, char *message)  //TODO  可变参函数 ...
{
  char buffer[BUF_SIZE] = {0};

  if ( which == 401 ) {
    sprintf(buffer, "HTTP/1.0 401 Unauthorized\r\n" \
                    "Content-type: text/plain\r\n" \
                    STD_HEADER \
                    "WWW-Authenticate: Basic realm=\"PRG_NAME\"\r\n" \
                    "\r\n" \
                    "401: Not Authenticated!\r\n" \
                    "%s", message);
  } else if ( which == 404 ) {
    sprintf(buffer, "HTTP/1.0 404 Not Found\r\n" \
                    "Content-type: text/plain\r\n" \
                    STD_HEADER \
                    "\r\n" \
                    "404: Not Found!\r\n" \
                    "%s", message);
  } else if ( which == 500 ) {
    sprintf(buffer, "HTTP/1.0 500 Internal Server Error\r\n" \
                    "Content-type: text/plain\r\n" \
                    STD_HEADER \
                    "\r\n" \
                    "500: Internal Server Error!\r\n" \
                    "%s", message);
  } else if ( which == 400 ) {
    sprintf(buffer, "HTTP/1.0 400 Bad Request\r\n" \
                    "Content-type: text/plain\r\n" \
                    STD_HEADER \
                    "\r\n" \
                    "400: Not Found!\r\n" \
                    "%s", message);
  } else {
    sprintf(buffer, "HTTP/1.0 501 Not Implemented\r\n" \
                    "Content-type: text/plain\r\n" \
                    STD_HEADER \
                    "\r\n" \
                    "501: Not Implemented!\r\n" \
                    "%s", message);
  }

  write(fd, buffer, strlen(buffer));
}


/******************************************************************************
Description.: Send HTTP header and copy the content of a file. To keep things
              simple, just a single folder gets searched for the file. Just
              files with known extension and supported mimetype get served.
              If no parameter was given, the file "index.html" will be copied.
Input Value.: * fd.......: filedescriptor to send data to
              * parameter: string that consists of the filename
              * id.......: specifies which server-context is the right one
Return Value: -
******************************************************************************/
int up_sendfile(int fd, char *parameter)
{
    DBG_PRINTF("====> %s\n", __FUNCTION__);

  char buffer[BUF_SIZE] = {0};
  char *extension, *mimetype=NULL;
  int i, lfd;

  /* in case no parameter was given */
  if ( parameter == NULL || strlen(parameter) == 0 )
    parameter = "index.html";

  /* find file-extension */ //TODO CGI  http://192.168.0.254/cgi-bin/shutdown?time=now
  if ( (extension = strstr(parameter, ".")) == NULL ) {
    send_error(fd, 400, "No file extension found");
    return -1;
  }

  /* determine mime-type */
  for ( i=0; i < LENGTH_OF(mimetypes); i++ ) {
    if ( strcmp(mimetypes[i].dot_extension, extension) == 0 ) {
      mimetype = (char *)mimetypes[i].mimetype;
      break;
    }
  }

  /* in case of unknown mimetype or extension leave */
  if ( mimetype == NULL ) {
    ERR_PRINTF("unkown mime type\n");
    mimetype = "application/octet-stream"; //下载
    //send_error(fd, 404, "MIME-TYPE not known");
    //return -1;
  }

  /* now filename, mimetype and extension are known */
  DBG_PRINTF("trying to serve file \"%s\", extension: \"%s\" mime: \"%s\"\n", parameter, extension, mimetype);

  /* build the absolute path to the file */
  snprintf(buffer, sizeof(buffer)-1, "%s/%s", up_conf.doc_root_path, parameter);
  

  /* try to open that file */
  if ( (lfd = open(buffer, O_RDONLY)) < 0 ) {
    ERR_PRINTF("file %s not accessible\n", buffer);
    send_error(fd, 404, "Could not open file");
    return -1;
  }
  DBG_PRINTF("opened file: %s\n", buffer);

  /* prepare HTTP header */
  i = sprintf(buffer, "HTTP/1.0 200 OK\r\n" \
                  "Content-type: %s\r\n" \
                  STD_HEADER \
                  "\r\n", mimetype);
  DBG_PRINTF("[%d]%s", i, buffer);
  i = strlen(buffer);
  /* first transmit HTTP-header, afterwards transmit content of file */
  do {
    if ( write(fd, buffer, i) < 0 ) {
      close(lfd);
      return -1;
    }
  } while ( (i=read(lfd, buffer, sizeof(buffer))) > 0 );
  if (strcmp(mimetype, "text/plain") == 0) {
    DBG_PRINTF("%s\n", buffer);
  }

  /* close file, job done */
  close(lfd);
  DBG_PRINTF("<==== %s\n", __FUNCTION__);

  return 0;
}

void worker_del(struct worker_info *pworker)
{
    close(pworker->conn_fd);
    //从链表中清除
    pthread_mutex_lock(&pmaster->mutex);
    list_del_init(&pworker->list);
    pthread_mutex_unlock(&pmaster->mutex);

}


void *client_handler(void *args)
{
    DBG_PRINTF("====>client_handler\n");
	struct worker_info *pworker = (struct worker_info *)args;

    struct iobuffer_t iobuffer = {0};
   // while (!srv_shutdown) {
    	iobuffer.len = read(pworker->conn_fd, iobuffer.buf, IO_BUF_SIZE);
    	if (iobuffer.len <= 0) {
        	if (iobuffer.len == 0) {
        		DBG_PRINTF("connect closed by peer\n");
                pworker->status = S_DETACH; //断开连接
            } else {
                perror("read");
            }
            worker_del(pworker);
            return (void *)-1;
    	}else {
        	DBG_PRINTF("recv:%s[%d]\n", iobuffer.buf, iobuffer.len);
        	char method[PATH_SIZE] = {0};
    		char uri[PATH_SIZE] = {0};
    		char http_ver[PATH_SIZE] = {0};
    		sscanf(iobuffer.buf, "%s %s %s", method, uri, http_ver);
    		DBG_PRINTF("method = %s\n", method);
    		DBG_PRINTF("uri = %s\n", uri);
    		DBG_PRINTF("http_ver = %s\n", http_ver);
    	
            if (up_sendfile(pworker->conn_fd, uri + 1) < 0) {
                //close(pworker->conn_fd);
                worker_del(pworker);
                return (void *)-1;
            }
            close(pworker->conn_fd);
	   }
    //}
    DBG_PRINTF("<==== %s\n", __FUNCTION__);

    return (void *)NULL;
}

void *client_handler2(void *args)
{
    DBG_PRINTF("====>client_handler\n");
    int conn_fd = (int)args;

    struct iobuffer_t iobuffer = {0};
   // while (!srv_shutdown) {
    read_again:
        iobuffer.len = read(conn_fd, iobuffer.buf, IO_BUF_SIZE);
        if (iobuffer.len <= 0) {
            if (iobuffer.len == 0) {
                DBG_PRINTF("connect closed by peer\n");
            } else {
                if (errno == EAGAIN) {
                    goto read_again;
                }
                perror("read");
            }
            return (void *)-1;
        }else {
            DBG_PRINTF("recv:%s[%d]\n", iobuffer.buf, iobuffer.len);
            char method[PATH_SIZE] = {0};
            char uri[PATH_SIZE] = {0};
            char http_ver[PATH_SIZE] = {0};
            sscanf(iobuffer.buf, "%s %s %s", method, uri, http_ver);
            DBG_PRINTF("method = %s\n", method);
            DBG_PRINTF("uri = %s\n", uri);
            DBG_PRINTF("http_ver = %s\n", http_ver);
        
            if (up_sendfile(conn_fd, uri + 1) < 0) {
                //close(pworker->conn_fd);
                //worker_del(pworker);
                return (void *)-1;
            }
            close(conn_fd);
       }
    //}
    DBG_PRINTF("<==== %s\n", __FUNCTION__);

    return (void *)NULL;
}

int worker_add(int conn_fd)
{
    DBG_PRINTF("====> %s\n", __FUNCTION__);

	struct worker_info *pworker = (struct worker_info *)
				malloc(sizeof(struct worker_info));
	if (pworker == NULL) {
		perror("malloc");
		return -1;
	}
	//初始化
	pworker->status = S_INIT;
	pworker->tid = 0;

	pthread_mutex_lock(&pmaster->mutex);
	list_add_tail(&pmaster->list, &pworker->list);
	pthread_mutex_unlock(&pmaster->mutex);

	pworker->conn_fd = conn_fd;

	int iret = pthread_create(&pworker->tid, NULL, client_handler, pworker);
	if (iret != 0) {
		ERR_PRINTF("pthread_create failed.");
		pthread_mutex_lock(&pmaster->mutex);
		list_del_init(&pworker->list);
		pthread_mutex_unlock(&pmaster->mutex);
		free(&pworker);
		return -1;
	}

    pthread_detach(pworker->tid);

	pmaster->worker_nums += 1;
	DBG_PRINTF("create new thread(%zu), worker_nums = %zu\n", (size_t)pworker->tid, pmaster->worker_nums);
    DBG_PRINTF("<==== %s\n", __FUNCTION__);
	return 0;    
}

int server_run2()
{
    DBG_PRINTF("====> %s\n", __FUNCTION__);
	struct sockaddr_in cli_addr;
	socklen_t addr_len = sizeof(cli_addr);

	pmaster = (struct master_info *)
				malloc(sizeof(*pmaster));
	if (pmaster == NULL) {
		perror("malloc");
		return -1;
	}

	//初始化
	//头节点的处事呼
	list_head_init(&pmaster->list);
	//互斥锁，条件变量初始化
	pthread_mutex_init(&pmaster->mutex, NULL);
	pthread_cond_init(&pmaster->cond, NULL);
	pmaster->worker_nums = 0;
	pmaster->sock_fd = sock_fd;

    fd_set rdfds;
    struct timeval tv = {0, 0};
    //线程池
    pool_init(3);


	while (!srv_shutdown) {
        FD_ZERO(&rdfds);
        FD_SET(sock_fd, &rdfds);
        //DBG_PRINTF("sock_fd = %d\n", sock_fd);
        tv.tv_sec = 0;
        tv.tv_usec = 500000;

        int i = select(sock_fd + 1, &rdfds, NULL, NULL, &tv);
        
        if (i == -1) {
            perror("select");
            return -1;
        }else if (i == 0) {
            continue;
        }else{
            if (FD_ISSET(sock_fd, &rdfds)) {    
        		int conn_fd = accept(sock_fd, (struct sockaddr *)&cli_addr, &addr_len);
        		if (conn_fd == -1) {
        			ERR_PRINTF("accept failed.\n");
                    perror("accept");
        			break;
        		}
        		if (up_conf.verbose){
        			DBG_PRINTF("new connection %s:%hu\n", inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));
        		}
        		//worker_add(conn_fd);
                pool_add_worker(client_handler2, conn_fd);
                DBG_PRINTF("worker_nums = %zu\n", pmaster->worker_nums);
                }
	   }
    }	

    if (srv_shutdown) {
        server_destroy();
    }

    DBG_PRINTF("<==== %s\n", __FUNCTION__);
    return 0;
}
#if 0
int server_run()
{
    DBG_PRINTF("====> %s\n", __FUNCTION__);
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
#endif
/*服务器端的销毁*/
int server_destroy()
{

    #if 0
    //链表遍历销毁
    struct list_head *cur, *Next;
    list_for_each_safe(cur, Next, &pmaster->list) {
        struct worker_info *pworker = container_of(cur, struct worker_info, list);
        pthread_cancel(pworker->tid);
        //mutex_lock
        list_del_init(&pworker->list);
        //mutex_unlock
        pmaster->worker_nums -= 1;
        free(pworker);
    }

    DBG_PRINTF("workder_nums = %zu\n", pmaster->worker_nums);
    pthread_mutex_destroy(&pmaster->mutex);
    pthread_cond_destroy(&pmaster->cond);
    #endif

    if (srv_destroy == 1) {
        return 0;
    }
    srv_destroy = 1;

    /*销毁线程池*/
    pool_destroy ();

    free(pmaster);

	return 0;
}


int main(int argc, char *argv[])
{
	//1. 配置解析
	if (up_conf_init(argc, argv) <0){
		ERR_PRINTF("up_conf_init failed.\n");
		return -1;
	}
	
	
	//2. 服务器初始化
    if (server_init() < 0) {
		ERR_PRINTF("server_init failed.\n");
		return -1;
	}

	server_run2();
		

	return 0;
}
