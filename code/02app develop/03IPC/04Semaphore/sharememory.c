#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#define   SHM_NAME "/uplooking"

int main()
{
	int fd = shm_open(SHM_NAME, O_RDWR | O_CREAT |O_TRUNC, 0666);
	if (fd == -1) {
		perror("shm_open");
		return -1;
	}
	





	shm_unlink(SHM_NAME);
	return 0;
}
