#include <stdio.h>

//回调函数：不会通过函数名直接调用，只能通过指向该函数的指针去间接调用的函数，被称为回调函数。

//回调函数的好处：分离了调用层和被调用层，对于调用层来说，被调用层的函数是谁，他根本不需关心，他唯一关心的是是否存在一个有固定接口(API)的函数(该函数必须有一个参数和回调函数的类型对应)可以被调用。一定是在某个事件的驱动下，回调函数才会被间接调用的。

static inline void callback1(void *n) //回调函数
{
	printf("hello, %s!\n", (char *)n);
}

static inline void callback2(void *n) //回调函数
{
	int sum = 0;
	while ((int)(--n)) {
		sum += (int)n;
	}
	printf("sum = %d\n", sum);
}

int main(void)
{
	do_it(callback1, "everyone");
	do_it(callback2, (void *)3);

	//callback1(); //此时的调用方式不能将callback1函数作为回调函数

	return 0;
}
