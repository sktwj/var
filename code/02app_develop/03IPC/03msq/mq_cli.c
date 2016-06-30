#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <errno.h>

#define MQ_NAME	"/uplooking"

int main()
{

	mqd_t mqd = mq_open(MQ_NAME, O_RDWR | O_CREAT | O_EXCL, 0666, NULL);

	if (mqd == -1) {
		if (errno == EEXIST) {
			mqd = mq_open(MQ_NAME, O_RDWR);
		}	else {
			perror("mq_open");
			return -1;
		}
	}

#define  BUF_SIZE	1024 
	char buf[BUF_SIZE] = {0};
	int iret = 0, n;

	while (1) {
		if (fgets(buf, BUF_SIZE, stdin) == NULL) {
			break;
		}
		n = strlen(buf);
		buf[n - 1] = '\0';
		iret = mq_send(mqd, buf, strlen(buf), 0);
		if (iret == -1) {
			perror("mq_send");
			return -1;
		}
	}
	printf("EOF\n");
	mq_close(mqd);
	return 0;
}
