#include <stdio.h>

//定义int类型的别名是elem_t
typedef int elem_t;
//typedef unsigned int size_t; 

void foo(void)
{
	printf("%s\n", __func__);
}

int main(void)
{
	elem_t a = 10;

#if 1
	char s[3][10] = {0};
	typedef char (arr_t)[3][10]; //定义复杂类型的别名，是需要将复杂类型和别名结合起来写的
	arr_t t = {"hello"};
	printf("t = %s\n", t);
	printf("*t = %s\n", *t);
#endif
	//char *p = "hello";
	//char ss[3][10] = {"hello"};

	typedef void (*foo_t)(void); //foo函数的函数指针类型的别名
	foo_t pf = foo;
	pf(); //通过类型别名定义的变量去调用foo函数

	return 0;
}
