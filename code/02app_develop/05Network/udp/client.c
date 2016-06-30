#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define  SRV_PORT  6666

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
	inet_aton("255.255.255.255", &srv_addr.sin_addr);

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
		if (fgets(buf, BUF_SIZE, stdin) == NULL) {
			break;
		}
		n = sendto(sock_fd, buf, strlen(buf), 0, 
				(struct sockaddr *)&srv_addr, addrlen);
		if (n == -1){
			perror("sendto");
			break;
		}
		printf("sendto (%s:%hu)[%lu]%s", inet_ntoa(srv_addr.sin_addr), ntohs(srv_addr.sin_port), strlen(buf), buf);
		fflush(stdout);
	}

	close(sock_fd);

	return 0;
}
