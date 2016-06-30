#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include <errno.h>

#define  SEM_NAME1 	"/uplooking1"
#define  SEM_NAME2 	"/uplooking2"

int main()
{
	sem_t *sem1 = sem_open(SEM_NAME1, O_RDWR| O_CREAT | O_EXCL, 0666, 1);	
	if (sem1 == SEM_FAILED) {
		if (errno == EEXIST) {
			sem1 = sem_open(SEM_NAME1, O_RDWR);
		} else {
			perror("sem_open");
			return -1;
		}
	}

	sem_t *sem2 = sem_open(SEM_NAME2, O_RDWR| O_CREAT | O_EXCL, 0666, 0);	
	if (sem2 == SEM_FAILED) {
		if (errno == EEXIST) {
			sem2 = sem_open(SEM_NAME2, O_RDWR);
		} else {
			perror("sem_open");
			return -1;
		}
	}



	while (1) {
		sem_wait(sem1);
		fprintf(stderr, "process pid = %d\n", getpid());
		sleep(1);
		sem_post(sem2);
	}

	sem_close(sem1);
	sem_close(sem2);
	return 0;
}
