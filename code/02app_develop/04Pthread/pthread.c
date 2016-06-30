#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

void *thrd_func(void *arg)
{
	int nsecond = (int)arg;
	//pthread_mutex_lock(&mtx);
	sleep(nsecond);
	printf("[%d]tid=%u, sleep(%d), pid=%d\n", nsecond, pthread_self(), nsecond, getpid());
	//pthread_mutex_unlock(&mtx);
	return NULL;
}

int main()
{
	pthread_t tid[3] = {0};

	int i;
	for (i = 0; i < sizeof tid /sizeof tid[0]; ++i){
		pthread_create(tid + i, NULL, thrd_func, (void *)(i + 1));
	}

	for (i = 0; i < sizeof tid / sizeof tid[0]; ++i) {
		pthread_join(tid[i], NULL);
	}

	return 0;
}
