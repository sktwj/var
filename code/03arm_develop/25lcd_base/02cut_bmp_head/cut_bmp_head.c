#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"

int main(int argc, char **argv)
{
	if (argc != 3) {
		fprintf(stderr, "Usage: cmd <source bmp file> <output bin file>\n");
		return -1;
	}

	//打开bmp源文件
	FILE *fp_src = fopen(argv[1], "r");
	if (!fp_src) {
		perror("fopen source bmp file");
		goto err_fopen_src;
	}

	//新建目标文件
	FILE *fp_dst = fopen(argv[2], "w");
	if (!fp_dst) {
		perror("fopen create dst file");
		goto err_fopen_dst;
	}

	//读取文件头
	struct bmp_file head = {0};
	fread(&head, sizeof(head), 1, fp_src);

	//将文件指针定位到像素数据的首地址
	fseek(fp_src, head.offset, SEEK_SET);

	unsigned char *buf = (unsigned char*)calloc(1, head.size - head.offset);
	fread(buf, head.size - head.offset, 1, fp_src);
	fwrite(buf, head.size - head.offset, 1, fp_dst);

	free(buf);
	fclose(fp_dst);
	fclose(fp_src);

	return 0;

err_fopen_dst:
	fclose(fp_src);
err_fopen_src:
	return -1;
}