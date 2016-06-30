#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

/* typedef  unsigned short pid_t; */
int c = 30;
int main()
{
	pid_t pid = getpid();
	pid_t ppid = getppid();
	printf("hello,my pid is %d, getppid is %d\n", pid, ppid);

	int a = 10;
	int *pb = (int *)malloc(sizeof(int));
	*pb = 20;

	pid = fork();
		
	if (pid == -1) {
		perror("fork failed");
		return -1;
	}

	if (pid == 0) {
		//child process
		printf("this is child, pid:%d, ppid:%d\n", getpid(), getppid());
		++a;
		*pb +=1;
		++c;
		printf("[child]a = %d, b = %d, c = %d\n", a, *pb, c);
		//exit(0);
		return 0;
	}

	//parent process
	printf("this is parent: pid:%d, ppid = %d\n", getpid(), getppid());
	printf("hello,world\n");
	printf("[parent]a = %d, b = %d, c = %d\n", a, *pb, c);

	return 0;	
}
