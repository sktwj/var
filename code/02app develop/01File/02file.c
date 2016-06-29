#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
		n = fread(rbuf, 1, BUF_SIZE, pf);
		fwrite(rbuf, 1, n, stderr);
	}

	fseek(pf, 0, SEEK_END);
	printf("total_size: %ld\n", ftell(pf));

	fclose(pf);

	return 0;
}
