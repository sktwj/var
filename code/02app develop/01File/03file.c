#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	if (argc < 2) {
		fprintf(stderr, "[Error]:缺少参数\n");
		return -1;
	}

	FILE *pf = fopen(argv[1], "r");
	if (pf == NULL) {
		perror("fopen failed");
		return -1;
	}
#define  BUF_SIZE	4096
	char rbuf[BUF_SIZE] = {0};
	size_t n;
	while (!feof(pf)) {
		if (fgets(rbuf, BUF_SIZE, pf) != NULL) {
			sleep(1);
			fputs(rbuf, stderr);
		}
	}

	fseek(pf, 0, SEEK_END);
	printf("total_size: %ld\n", ftell(pf));

	fclose(pf);

	return 0;
}
