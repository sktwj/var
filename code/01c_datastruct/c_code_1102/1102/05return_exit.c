#include <stdio.h>
#include <stdlib.h>

void foo(void)
{
	printf("%s\n", __func__);
	//return;
	exit (0); //exit出现在任何函数内，都可以使得程序终止；return 只有出现在main函数，才能使得程序终止
}

int main(void)
{
	foo();
	printf("ok\n");
	return 0;
}
