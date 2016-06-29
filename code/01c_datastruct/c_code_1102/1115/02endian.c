#include <stdio.h>

//cpu架构：大端架构MSB和小端架构LSB。通常也称为字节序，就是字节的存放顺序。
//小端架构little_endian：数据的低位部分存放在内存的低地址上
//大端架构big_endian：数据的低位部分存放在内存的高地址上

int test_endian(int *n)
{
	return  *((char *)(n)) == 0x44;
}

int main(void)
{
	int a = 0x11223344;
	printf("%s\n", test_endian(&a) ? "little_endian" : "big_endian");

	return 0;
}
