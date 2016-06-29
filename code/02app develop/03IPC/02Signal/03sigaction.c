#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>

void my_sigaction(int signo,
		siginfo_t *info, void *unused)
{
	printf("signal sender: pid = %d, uid=%d\n", info->si_pid, info->si_uid);
	printf("si_int = %d\n", info->si_int);
}

int main()
{
	struct sigaction sa, old_sa;
	//如果希望接受处理signo以外的其他信息，可以使用sigaction, siginfo_t , sa_flags = SA_SIGINFO
	//SA_RESTART:解决因为信号而发生中断，导致错误返回，从而让该函数重新执行
	memset(&sa, 0, sizeof(sa));
	sa.sa_flags = SA_SIGINFO | SA_RESTART;
	sigemptyset(&sa.sa_mask);
	sa.sa_sigaction = my_sigaction;

	if (sigaction(SIGINT, &sa, &old_sa) == -1) {
		perror("sigaction");
		return -1;
	}
	if (sigaction(SIGUSR1, &sa, &old_sa) == -1) {
		perror("sigaction");
		return -1;
	}	
	printf("pid = %d\n", getpid());
#if 0
	while (1) {
		pause();
	}
#endif
#define  BUF_SIZE	1024
	char rbuf[BUF_SIZE] = {0};
#if 0
	while (1) {
		if (fgets(rbuf, BUF_SIZE, stdin) == NULL) {
			if (errno == EINTR) {
				continue;
			}else {
				perror("fgets");
				break;
			}
		} 
		break;
	}
#endif
	if (fgets(rbuf, BUF_SIZE, stdin) == NULL) {
		perror("fgets");
	}else{
		printf("%s",rbuf);
	}

	return 0;
}
