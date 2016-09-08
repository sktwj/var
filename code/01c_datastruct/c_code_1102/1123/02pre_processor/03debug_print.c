#include <stdio.h>

//该调试打印宏也是用来帮助程序员找bug的，也只应该用于程序的调试阶段。因为在调试阶段，总不可避免会有很多中间变量的临时打印，来验证程序的某个结果的正确与否。一旦调试通过了，这些临时打印的中间变量就不应该再被打印出来了，此时如果采用手工一行一行去除printf打印语句的方式，那么很容易遗漏和搞错，而且耗时费力。那么最好的选择就是在程序一开始进行书写的时候就布置好该调试打印宏，它的开启和关闭都是非常容易的，对于程序的维护也是最好的。

//#define DEBUG

#ifdef DEBUG
#define DEBUG_PRINT(fmt, args...) \
	printf(fmt, ##args)
#else
#define DEBUG_PRINT(fmt, args...)
#endif

int main(void)
{
	int i = 5;
	while (i--) {
		DEBUG_PRINT("debug, i = %d\n", i);
	}
	printf("forever, i = %d\n", i);
	DEBUG_PRINT("loop over!\n");

	return 0;
}
