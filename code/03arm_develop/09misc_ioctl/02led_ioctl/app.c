#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "ldm_cmd.h"

int main()
{
	int fd = open("/dev/ldm", O_RDWR);
	if (fd < 0) {
		perror("open");
		goto err_open;
	}

	unsigned char buf = 0b1001;
	//write(fd, &buf, sizeof(buf));
	ioctl(fd, LED_ON, buf);
	sleep(5);

	ioctl(fd, LED_OFF);

	close(fd);

	return 0;

err_open:
	return -1;
}
