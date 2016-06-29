#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

int main(int argc, char *argv[])
{
	if (argc < 2) {
		fprintf(stderr, "Error:缺少参数\n");
		return -1;
	}	
	/*EXCL: 如果文件已存在，则报错*/
	int fd = open(argv[1], O_RDWR | O_CREAT | O_EXCL, 0755);
	if (fd == -1) {
		fprintf(stderr, "open failed.\n");
		fprintf(stderr, "errno = %d(%s)\n", errno, strerror(errno));;
		perror("open failed");
		if (errno == EEXIST) {
			/*文件存在则在后面追加*/
			fd = open(argv[1], O_RDWR | O_APPEND);
		} else {
			return -1;
		}
	}

	printf("fd = %d\n ", fd);
	int i = 10, n;
	time_t t;
#define  BUF_SIZE	1024
	char buf[BUF_SIZE] = {0};

	lseek(fd, 20, SEEK_SET);
#if 0
	for (; i > 0; --i) {
		time(&t);
		n = snprintf(buf, BUF_SIZE, "%s", ctime(&t));
		write(fd, buf, n);
		printf("%s", buf);
		sleep(1);
	}
#endif
	printf("offset by seek_set:%lld\n", lseek(fd, 0, SEEK_CUR));
	close(fd);

	return 0;
}
