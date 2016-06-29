#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

int main(int argc, char *argv[])
{
	int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_fd == -1) {
		perror("socket");
		return -1;
	}
	printf("socket ok, %d\n", sock_fd);
	
	struct sockaddr_in srv_addr;
	bzero(&srv_addr, sizeof(srv_addr));
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_port = htons(atoi(argv[2]));
	inet_aton(argv[1], &srv_addr.sin_addr);

	connect(sock_fd, srv_addr, sizeof(srv_addr));
	//read,write
	
	close(sock_fd);	

	return 0;
}
