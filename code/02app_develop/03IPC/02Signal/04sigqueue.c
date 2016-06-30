#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int main(int argc, char *argv[])
{
	if (argc < 4) {
		fprintf(stderr, "Usage: %s pid signo data\n", argv[0]);
		return -1;
	}
	pid_t pid = atoi(argv[1]); 
	int signo = atoi(argv[2]);
	int data = atoi(argv[3]);
	union sigval sv;
	sv.sival_int = data;

	int iret = sigqueue(pid, signo, sv);
	if (iret == -1) {
		perror("sigqueue");
		return -1;
	}


	return 0;
}
