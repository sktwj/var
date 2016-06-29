#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <errno.h>
#include <signal.h>

#define MQ_NAME	"/uplooking"

static volatile  sig_atomic_t srv_shutdown = 0;


void sig_handler(int signo)
{
	switch (signo) {
		case SIGINT:
		case SIGTERM:
			srv_shutdown = 1;
			break;
		defualt:
			break;
	} 
}



int main()
{
	signal(SIGINT, sig_handler);
	signal(SIGTERM, sig_handler);

	mqd_t mqd = mq_open(MQ_NAME, O_RDWR | O_CREAT | O_EXCL, 0666, NULL);

	if (mqd == -1) {
		if (errno == EEXIST) {
			mqd = mq_open(MQ_NAME, O_RDWR);
		}	else {
			perror("mq_open");
			return -1;
		}
	}

//缓冲区大小一定要大于8192
#define  BUF_SIZE	8192 
	char buf[BUF_SIZE] = {0};
	int iret = 0, prio;

	while (!srv_shutdown) {
		iret = mq_receive(mqd, buf, BUF_SIZE, &prio);
		if (iret == -1) {
			perror("mq_send");
			return -1;
		}
		printf("receive:(%d)%s, prio=%d\n", iret, buf, prio);
	}

	printf("EOF\n");
	mq_close(mqd);
	mq_unlink(MQ_NAME);
	return 0;
}
