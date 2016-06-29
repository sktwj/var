#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>


int main()
{
	pid_t p = fork();
	/*
	 僵尸进程：指的是子进程已经退出了，但是父进程还没有过来替它回收的进程。
	避免僵尸进程：
	1. 主动收尸：
	2. 杀掉父进程， 僵尸进程变成孤儿进程，默认会过继给pid 1的进程
	3. 忽略信号： SIGCHLD
	 */
	switch (p) {
		case -1:
			perror("fork");
			return -1;
			break;
		case 0:
			//child
			printf("child, pid = %d, ppid = %d\n", getpid(), getppid());
			return 0;
			break;
		default:
			break;
	}
	printf("parent pid = %d, childpid = %d\n", getpid(), p);
	int i = 10;
	while (--i) {
		sleep(1);
	}
	int iret = 0;
	p = wait(&iret);
	if (p == -1) {
		perror("wait");
		return -1;
	}
	printf("wait done\n");
	if (WIFEXITED(iret)) {
		printf("child(%d) exit, return %d\n", p, WEXITSTATUS(iret));	
	}

	return 0;
}

