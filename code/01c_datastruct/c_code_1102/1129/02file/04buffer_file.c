#include <stdio.h>

#if 0
对于文件操作的函数：
	1、针对一个字符的：
		fgetc getc getchar
	   	fputc putc putchar
	2、针对一行的：	
		fgets gets
		fputs puts
	3、指针一块区域的：
		fread 
		fwrite
#endif

int main(int argc, char *argv[])
{
	if (argc != 3) {
		printf("usage: ./a.out read_file write_file\n");
		return 1;
	}

	FILE *fp1, *fp2;

	if (!(fp1 = fopen(argv[1], "r"))) {
		perror("open file for read"); //以一种最简单的方式向用户报告一个错误。首先打印perror函数的参数，然后跟: 真正发生错误的原因
		return 1;
	}
	if (!(fp2 = fopen(argv[2], "w+"))) {
		perror("open file for write");
		fclose(fp1);
		return 1;
	}

#define MAX 100
	char s[MAX] = {0};
#if 0
	while (fgets(s, MAX, fp1)) {
		fputs(s, fp2);
	}
#endif

	int ret = 0;
	//fread函数用于读取文件内指定区域的一部分内容，返回的是成功读取的条目，也就是第三个参数。但是它无法区分是到达了文件尾还是发生了错误。可以通过ferror函数测试是否发生了错误，通过feof函数判断是否达到了文件尾。feof函数不但可以用于判断二进制文件尾，还可以判断是否达到了文本文件尾。但是EOF仅仅能用于判断是否达到了文本文件尾。
	while (ret = fread(s, 1, 5, fp1)) {
		printf("ret = %d\n", ret);
        write(1, s, ret);
		fwrite(s, 1, ret, fp2);
		if (!ferror(fp1)) {
			printf("no err occurs !\n");
		} else {
			printf("err occurs !\n");	
			break;
		}
	}

	fclose(fp1);
	fclose(fp2);

	return 0;
}
