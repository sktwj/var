#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

void sig_handler(int signo)
{
	printf("catch a signal:%d,%s\n", signo, strsignal(signo));
}


int main()
{
	struct sigaction sa, old_sa;
	memset(&sa, 0, sizeof(sa));
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sa.sa_handler = sig_handler;

	if (sigaction(SIGINT, &sa, &old_sa) == -1) {
		perror("sigaction");
		return -1;
	}	

	while (1) {
		pause();
	}

	return 0;
}
