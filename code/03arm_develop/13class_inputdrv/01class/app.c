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

	unsigned char buf = 0b1001;
	write(fd, &buf, sizeof(buf));
	printf("addr of buf = 0x%x\n", &buf);

	close(fd);

	return 0;

err_open:
	return -1;
}
