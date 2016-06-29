#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
	int fd = open("/dev/ldm", O_RDWR);
	if (fd < 0) {
		perror("open");
		goto err_open;
	}

	printf("pid=%d\n", getpid());

	unsigned char buf = 0;
	read(fd, &buf, 1);

	close(fd);

	return 0;
err_open:
	return -1;
}
