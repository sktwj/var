#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>

//信号处理函数
void sig_handler(int signo)
{
	if (signo == SIGINT) {
		printf("catch SIGINT\n");
	}else if (signo == SIGUSR1) {
		sleep(5);
		printf("catch SIGUSR1\n");
	}else if (signo == SIGRTMIN+1) {
		printf("cat SIGRTMIN\n");
	}

}

int main()
{
	//信号安装函数 
	// SIG_IGN SGI_DFT
	
	//if (signal(SIGINT, SIG_IGN) == SIG_ERR) {

	if (signal(SIGINT, sig_handler) == SIG_ERR) {
		perror("signal");
		return -1;
	}	
	if (signal(SIGUSR1, sig_handler) == SIG_ERR) {
		perror("signal");
		return -1;
	}

	if (signal(SIGRTMIN+1, sig_handler) == SIG_ERR) {
		perror("signal");
		return -1;
	}

#define BUF_SIZE	1024
	char buf[BUF_SIZE]	= {0};

	//pause();
	if (fgets(buf, BUF_SIZE,stdin) == NULL) {
		if (errno == EINTR) {
			fprintf(stderr, "signal interrupt\n");
		}
	} else {
		printf("buf:%s\n", buf);
	}	


	return 0;
}
