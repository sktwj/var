#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "utils.h"

struct  write_info {
	pid_t pid;
	sem_t *sem_write;
	sem_t *sem_read;
	int  shm_fd;
	struct shm_buf *shm_buf;
};


/*写进程，负责把标准输入的内容写入到共享内存中*/
static int write_init(struct write_info *info)
{
	info->pid = getpid();

	info->sem_write = sem_open(SEM_WRITE, O_RDWR);
	if (info->sem_write == SEM_FAILED) {
		perror("sem_open");
		return -1;
	}

	info->sem_read = sem_open(SEM_READ, O_RDWR, 0);
	if (info->sem_read == SEM_FAILED) {
		perror("sem_open");
		return -1;
	}

	info->shm_fd = shm_open(SHM_NAME, O_RDWR, 0);
	if (info->shm_fd == -1) {
		perror("shm_open");
		return -1;
	}

	struct stat sb;
	fstat(info->shm_fd, &sb);
	
	info->shm_buf = mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, info->shm_fd, 0);

	if (info->shm_buf == MAP_FAILED) {
		perror("mmap");
		return -1;
	}	

	return 0;
}

int write_destroy(struct write_info *info)
{
	//shm_unlink(SHM_NAME);
	sem_close(info->sem_write);
	sem_close(info->sem_read);
	//sem_unlink(SEM_WRITE);
	//sem_unlink(SEM_READ);

	return 0;
}

int main(int argc, char *argv[])
{
	struct write_info *pinfo = (struct write_info *)malloc(sizeof(struct write_info));

	int iret = write_init(pinfo);
	if (iret == -1) {
		fprintf(stderr, "Error: write init failed\n");
		return -1;
	}
	
	while (1) {
		//阻塞知道信号量非0
		sem_wait(pinfo->sem_read);
		int n = write(STDOUT_FILENO, pinfo->shm_buf->buf, pinfo->shm_buf->count);
		sem_post(pinfo->sem_write);
		if (n == 0) {
			break;
		}
	}	

	write_destroy(pinfo);
	return 0;
}
