#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main()
{
	printf("pid1 = %d is running\n", getpid());
	daemon(0, 0);
	while (1) {
		sleep(1);
		printf("pid1 = %d is running\n", getpid());
	}

	return 0;
}
