#include <stdio.h>

//函数定义中：参数的()中什么也没写，表示可以传递任意类型，任意个数的参数
void foo1()
{
	printf("%s\n", __FILE__);
	printf("%s\n", __FUNCTION__);
	printf("%s\n", __func__);
	printf("%s\n", __DATE__);
	printf("%s\n", __TIME__);
	printf("%d\n", __LINE__);
	printf("%d\n", __STDC__); //判断编译器是否遵循标准C，为1表示遵循，为0不遵循
}

//函数的参数()中出现void,表示在调用时不能传递任何参数
void foo2(void)
{
	printf("=== %s === \n", __func__);
}

int main(void)
{
	foo1(10, "hello", 's');
	foo2();
	return 0;
}
