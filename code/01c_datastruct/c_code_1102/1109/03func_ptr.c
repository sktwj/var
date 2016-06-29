#include <stdio.h>

int max(int a, int b)
{
	return a > b ? a : b; 
}
int min(int a, int b)
{
	return a < b ? a : b; 
}

int main(void)
{
	//函数指针
	int (*pm)(int, int) = max;

	printf("max = %d\n", max(3,4));
	printf("max = %d\n", (*pm)(3,4));
	
	printf("max = %d\n", pm(3,4));

	//函数指针数组
	int (*p[])(int, int) = {max, min};
	printf("min = %d\n", p[1](3, 4));
	printf("min = %d\n", (*(p+1))(3, 4));

	return 0;
}
