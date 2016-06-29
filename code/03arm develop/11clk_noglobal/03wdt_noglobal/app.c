#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#include <stdlib.h>

#include "ldm_cmd.h"

int main(int argc, char **argv)
{
	int fd = open("/dev/ldm", O_RDWR);
	if (fd < 0) {
		perror("open");
		goto err_open;
	}

	if ((argc < 2) || (argc > 3)) {
		fprintf(stderr, "Usage: cmd <ioctl_cmd> [cycle]\n");
		return -1;
	}

	if (!strcmp(argv[1], "on")) {
		ioctl(fd, WDT_ON);
	} else if (!strcmp(argv[1], "off")) {
		ioctl(fd, WDT_OFF);
	} else if (!strcmp(argv[1], "int")) {
		ioctl(fd, WDT_CFG_INT);
	} else if (!strcmp(argv[1], "reset")) {
		ioctl(fd, WDT_CFG_RESET);
	} else if (!strcmp(argv[1], "cycle")) {
		ioctl(fd, WDT_SET_CYCLE, atoi(argv[2]));
	} else {
		fprintf(stderr, "unknown ioctl command \"%s\", you may use \"on\", \"off\", \"int\", \"reset\", \"cycle\"\n", argv[1]);
	}

	close(fd);

	return 0;

err_open:
	return -1;
}
