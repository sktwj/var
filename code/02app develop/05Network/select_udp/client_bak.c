#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
#include <stddef.h>
#include "protocol.h"

#define  SRV_PORT  6666

struct host_info {
	struct sockaddr_in srv_addr;
	int sock_fd;
} host;

int reg_handler(const char *buf)
{
	struct msg_info msg = {0};
	msg.mtype = M_REG;
	time(&msg.mtime);
	msg.mtime = htonl(msg.mtime); //host to network
	strncpy(msg.buf, buf+1, BUF_SIZE);

	int n;
	n = sendto(host.sock_fd, &msg, offsetof(struct msg_info, buf) + strlen(msg.buf), 0, 
				(struct sockaddr *)&(host.srv_addr), sizeof(host.srv_addr));
	//接受从服务器段来的回应，TODO

	return n;
}

int main(int argc, char *argv[])
{
	int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock_fd == -1) {
		perror("socket");
		return -1;
	}

	struct sockaddr_in srv_addr, cli_addr;
	socklen_t addrlen = sizeof(srv_addr);
	bzero(&srv_addr, addrlen);
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_port = htons(SRV_PORT);
	inet_aton("127.0.0.1", &srv_addr.sin_addr);
	host.srv_addr = srv_addr;
	host.sock_fd = sock_fd;

	//inet_aton("255.255.255.255", &srv_addr.sin_addr);
	//开启广播发送功能
	int so_broadcast = 1;
	int iret = setsockopt(sock_fd, SOL_SOCKET, SO_BROADCAST, &so_broadcast, sizeof(so_broadcast));
	if (iret == -1) {
		perror("setsockopt");
		return -1;
	}

	ssize_t n;
	#define BUF_SIZE   64*1024
	char buf[BUF_SIZE] = {0};
	

	while (1) {
		//fgets 导致进程阻塞
		if (fgets(buf, BUF_SIZE, stdin) == NULL) {
			break;
		}
		int n = strlen(buf);
		buf[n-1] = '\0';
		switch (buf[0]) {
			case 'R': 
					//register
					reg_handler(buf);
					break;
			case 'C':
					//chat
					//chat_handler(buf);
					break;
			case 'Q':
					//quit_handler(buf);
					break;
			case 'L':
					//list_handler(buf);
					break;
			default:
					fprintf(stderr, "Error: out of range [R|C|Q|L]\n");
					break;
		}
		#if 0
		n = sendto(sock_fd, buf, strlen(buf), 0, 
				(struct sockaddr *)&srv_addr, addrlen);
		if (n == -1){
			perror("sendto");
			break;
		}
		#endif
		printf("sendto (%s:%hu)[%lu]%s", inet_ntoa(srv_addr.sin_addr), ntohs(srv_addr.sin_port), strlen(buf), buf);
		fflush(stdout);
	}

	close(sock_fd);

	return 0;
}
