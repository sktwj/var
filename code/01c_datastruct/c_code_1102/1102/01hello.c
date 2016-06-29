#include <stdio.h>

/*
编译的步骤：
	1、预处理（预处理器cpp）
		gcc -E xx.c > xx.i
	2、编译（编译器cc1）
		gcc -S xx.i -o xx.s
	3、汇编（汇编器as）
		gcc -c xx.s -o xx.o
	4、链接（连接器ld）
		gcc xx.o yy.o -o a.out
*/
//	5、将a.out加载进内存，运行

#if 0
LSB：least significant bit 小端有效，小端架构（数据的低位部分存放在内存的低地址）
MSB：most significant bit 大端
#endif

//void main() //不标准写法1
//main() //不标准写法2
//标准C中main函数的标准写法1
int main(void)
//标准C中main函数的标准写法2(在需要通过命令行传递参数时使用)
//int main(int argc, char *argv[])
{
	printf("hello world!\n");
	return 0;
}


