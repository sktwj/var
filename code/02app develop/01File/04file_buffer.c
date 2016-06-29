#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <assert.h>

int  main(int argc, char *argv[])
{
	assert(argc >= 2);
	
	FILE* pf = fopen(argv[1], "r");
	if (pf == NULL) {
		perror("fopen failed");
		return -1;
	}
	
	char buf[1] = {0};

	int n;
	while (1) {
		n = fread(buf, 1, 1, pf);
		if (n < 1) {
			break;
		}
	}

	fclose(pf);

	return 0;
}
