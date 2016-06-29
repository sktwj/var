#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <assert.h>
int  main(int argc, char *argv[])
{
	assert(argc >= 2);
	int fd = open(argv[1], O_RDONLY);
	if (fd == -1) {
		perror("open failed");
		return -1;
	}
	
	char buf[1] = {0};

	int n;
	while (1) {
		n = read(fd, buf, 1);
		if (n < 1) {
			break;
		}
	}

	close(fd);

	return 0;
}
