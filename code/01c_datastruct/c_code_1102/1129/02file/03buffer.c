#include <stdio.h>

//第2套文件操作：遵循标准C，是可以移植的，在所有遵循标准C的平台上都可以使用
//可以支持两种类型的文件：文本文件，二进制文件
//文本文件和二进制文件的区别：
//1、不同系统的文本文件都对应有一个或两个字符用于表示行结束，windows中用"\r\n"表示；UNIX使用"\n"表示；mac os 使用"\n"，早期mac系统使用"\r"表示。二进制文件是没有换行符号的。
//2、文本文件用字符表示字节；二进制文件不一定是，可能是字节组才能表示（比如4个字节才能表示一个int类型的值），字节都是平等的。
//3、可以用特定的字符去表示文本文件尾，曾经在windows中就出现过用ctrl+z表示文件尾，但是大部分系统是没有的。

//流：表示任意输入的源或任意输出的目的地。stdio.h不仅能表示文件流，还可以表示其他类型的流。

int main(void)
{
	FILE *fp = fopen("./01buffer.c", "r");
	if (!fp) {
		fprintf(stderr, "open file for read failed!\n");
		return 1;
	}

	int c;
	//EOF用于表示文本文件尾，因为EOF的值是-1，不会出现在文本文件中。但是不能用于表示二进制文件尾，因为它可能是二进制文件中的内容。
	//while ((c = fgetc(fp)) != EOF) {
	c = getc(fp);
	while (!feof(fp)) {
		fputc(c, stdout);
		c = getc(fp);
	}

	fseek(fp, 10l, SEEK_SET); //fseek作用同lseek，但是函数返回值的意义不同
	printf("ftell, %ld\n", ftell(fp)); //ftell用于计算文件当前位置距离文件开头的偏移量，单位是long int

	rewind(fp); //将文件指针置到文件起始位置<==>fssek(fp,0l,SEEK_SET)
	printf("rewind, %ld\n", ftell(fp)); 

	fclose(fp);

	return 0;
}

