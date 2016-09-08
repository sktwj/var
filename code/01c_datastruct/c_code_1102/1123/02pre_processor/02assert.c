#include <stdio.h>
/*#include <assert.h>*/

//断言宏assert的作用：用于帮助程序员找程序中的bug，只应该用于程序调试阶段（debug）；到了发布阶段（release），所有的断言宏都应该被去掉，不是真正的删除，而是通过第6行来开启和关闭。因为一旦程序因为某些原因需要重新修改，那么就需要重新启用该断言宏，而不是重新再写一次。

//#define NDEBUG //该行的打开用于关闭断言宏，注释掉该行用于开启断言宏

#ifndef NDEBUG
#include <stdlib.h>
#endif

int main(int argc, char **argv)
{
#ifndef NDEBUG
#define MY_ASSERT(expr) ({ \
	if (!(expr)) { \
	 	printf("%s: %s:%d: %s: Assertion `" #expr "' failed.\n",  argv[0]+2, __FILE__, __LINE__, __FUNCTION__); \
	 	abort(); \
	 } \
})
#else
#define MY_ASSERT(expr)
#endif

	int *p = 0;
	
    /*assert(p != 0);*/
	MY_ASSERT(p != 0);

	printf("hello world\n");

	return 0;
}
