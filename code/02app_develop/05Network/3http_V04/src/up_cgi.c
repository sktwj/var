#include "server.h"

int cgi_handler(int conn_fd, char *parameter)
{
	DBG_PRINTF("====> %s\n", __FUNCTION__);
	int sockfds[2];
	pid_t pid;

	int iret = socketpair(AF_LOCAL, SOCK_STREAM, 0, sockfds);
	if (iret != 0) {
		ERR_PRINTF("socketpair failed.");
		perror("socketpair");
		return -1;
	}

	if ((pid = fork()) == -1) {
		fprintf(stderr, "Error: fork");
		perror("fork");
		return -1;
	}else if (pid == 0) {
		char buf[BUF_SIZE] = {0};
		DBG_PRINTF("cgi_handler,parameter = %s\n", parameter);
		
		
		if (strstr(parameter, ".cgi") != NULL) {
			chdir("../cgi-bin/");
			snprintf(buf, BUF_SIZE,"./%s", parameter);
		}else{
			snprintf(buf, BUF_SIZE,"%s", parameter);
		}
		DBG_PRINTF("cgi_handler,parameter = %s\n", buf);
		close(sockfds[0]);
		dup2(sockfds[1], STDERR_FILENO);
		dup2(sockfds[1], STDOUT_FILENO);
		
		execlp(buf, buf, NULL);		
		fprintf(stderr, "execlp failed.");
		exit(-1);
	}
	close(sockfds[1]);
	
	int n = 0;
	wait(&n);
	char buf[BUF_SIZE] = {0};
	printf("n = %d\n", n);
	if (n != 0) {
		n = snprintf(buf, BUF_SIZE,"HTTP/1.0 500 %s\r\n" \
                  "Content-type: %s,charset=UTF-8\r\n" \
                  STD_HEADER \
                  "\r\n", strerror(n),"text/plain");
	} else {
		n = snprintf(buf, BUF_SIZE,"HTTP/1.0 200 OK\r\n" \
                  "Content-type: %s\r\n" \
                  STD_HEADER \
                  "charset=UTF-8\r\n" \
                  "\r\n", "text/html");
	}
	write(conn_fd, buf, n);
	
	do {
		n = read(sockfds[0], buf, BUF_SIZE);
		write(conn_fd, buf, n);
	} while (n > 0);
	
	DBG_PRINTF("n = %d,buf=%s", n, buf);
	DBG_PRINTF("<==== %s\n", __FUNCTION__);
	return 0;
}