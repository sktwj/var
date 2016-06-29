#include <stdio.h>

void swap(int *a, int *b) //失败版本
{
	int *tmp = a;
	*a = *b;
	*b = *tmp;
}

void swap2(int *a, int *b) //成功版本
{
	int *pa = a, *pb = b;
	int tmp = *pa;
	*pa = *pb;
	*pb = tmp;
}

void swap_ok(int *a, int *b) //成功版本
{
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

int main(void)
{
	int a = 3, b = 4;
	printf("before, a = %d, b = %d\n", a, b);
	swap2(&a, &b);
	//swap_ok(&a, &b);
	printf("after, a = %d, b = %d\n", a, b);
	return 0;
}
