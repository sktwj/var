#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

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
	inet_aton("0.0.0.0", &srv_addr.sin_addr);

	int iret = bind(sock_fd, (struct sockaddr *)&srv_addr, addrlen);

	ssize_t n;
	#define BUF_SIZE   64*1024
	unsigned char buf[BUF_SIZE] = {0};
	printf("server is running listen on udp:%d\n", SRV_PORT);
	while (1) {
		n = recvfrom(sock_fd, buf, BUF_SIZE, 0, (struct sockaddr *)&cli_addr, &addrlen);
		if (n == -1){
			perror("recvfrom");
			break;
		}
		printf("recv from (%s:%d)[%zd]%s", inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port), n, buf);
		fflush(stdout);
	}

	close(sock_fd);

	return 0;
}
