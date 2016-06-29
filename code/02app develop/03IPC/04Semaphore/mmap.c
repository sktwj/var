#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <errno.h>
#include <semaphore.h>

#define  MAX_NUM	500000
#define  SEM_NAME "/uplooking"
int main()
{
	int fd = open("1.txt", O_RDWR);
	if (fd == -1) {
		perror("open");
		return -1;
	}
	int n = 0;
	//write(fd, &n, sizeof(n));	
	struct stat sb;
	fstat(fd, &sb);
	printf("1.txt st_size = %d\n", sb.st_size);
	
	sem_t *sem = sem_open(SEM_NAME, O_RDWR|O_CREAT|O_EXCL, 0666, 1);
	if (sem == SEM_FAILED) {
		if (errno == EEXIST) {
			sem = sem_open(SEM_NAME, O_RDWR);
		} else {
			perror("sem_open");
			return -1;
		}
	}

	int *p = (int *)mmap(NULL, sb.st_size, PROT_READ| PROT_WRITE, MAP_SHARED, fd, 0);
	if (p == MAP_FAILED) {
		perror("mmap");
		return -1;
	}
	printf("*p = %d\n", *p);
		
	int i;
	
	pid_t pid = fork();

	if (pid == 0) {
		for (i = 0; i < MAX_NUM; ++i) {
			sem_wait(sem);
			n = *p;
			n += 1;
			*p = n;
			sem_post(sem);
		}
		exit(0);
	} else {
		for (i = 0; i < MAX_NUM; ++i) {
			sem_wait(sem);
			n = *p;
			n += 1;
			*p = n;
			sem_post(sem);
		}
	}
	wait(NULL);
	//msync 
	printf("p = %d\n", *p);
	munmap(p, sb.st_size);
	sem_close(sem);
	sem_unlink(SEM_NAME);
	close(fd);
	return 0;
}
