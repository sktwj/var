#pragma once

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include <errno.h>
#include <unistd.h>

#define  SHM_NAME "/uplooking"
#define  SEM_WRITE	"/up_writer"
#define  SEM_READ	"/up_reader"

#define  BUF_SIZE	4096

struct shm_buf {
	int count;
	char buf[BUF_SIZE];
};
