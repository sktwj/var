#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>

#define MQ_NAME "/uplooking"

int main()
{
	mqd_t mqd = mq_open(MQ_NAME, O_RDONLY);
	if (mqd == -1) {
		perror("mq_open");
		return -1;
	}

	struct mq_attr mq_attr = {0};
	int iret = mq_getattr(mqd, &mq_attr);
	if (iret == -1) {
		perror("mq_getattr");
		goto err;
		return -1;
	}

	printf("mq_flags = %ld\n", mq_attr.mq_flags);
	printf("mq_maxmsg = %ld\n", mq_attr.mq_maxmsg);
	printf("mq_msgsize = %ld\n", mq_attr.mq_msgsize);
	printf("mq_curmsgs = %ld\n", mq_attr.mq_curmsgs);
	
	mq_close(mqd);
	return 0;
err:
	mq_close(mqd);
	return -1;
}
