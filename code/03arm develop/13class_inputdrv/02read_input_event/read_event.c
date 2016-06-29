#include <stdio.h>
#include <fcntl.h>
#include <linux/input.h>



int main(int argc, char **argv)
{
	if (argc != 2) {
		fprintf(stderr, "Usage: cmd <event_device path>\n");
		return -1;
	}

	int fd = open(argv[1], O_RDWR);
	if (fd < 0) {
		perror("open device");
		return -1;
	}

	struct input_event event = {0};
	while (1) {
		read(fd, &event, sizeof(event));
		switch (event.type) {
		case EV_SYN:
			printf("EV_SYN ");
			break;
		case EV_KEY:
			printf("EV_KEY ");
			break;
		case EV_REL:
			printf("EV_REL ");
			break;
		case EV_ABS:
			printf("EV_ABS ");
			break;
		default:
			printf("event=0x%x ", event.type);
			break;
		}

		printf("code=%d value=%d\n", event.code, event.value);
	}


	close(fd);

	return 0;
}