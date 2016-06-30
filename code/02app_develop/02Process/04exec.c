#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main()
{
	printf("hello,world1");

	pid_t p = fork();

	if (p == 0) {
		printf("child\n");
		int iret = execl("/bin/ls", "ls", "-l", "-h", "-a", NULL);
		printf("Error:%s", strerror(errno));
		return 0;
	}
	
	wait(NULL);
	printf("wait done\n");

	return 0;
}
