#include <stdio.h>

/*
#:把宏的参数转化为一个字符串
##: 把宏的参数和普通字符间隔并拼接
 */
#define  FOO(a, b) \
		printf(#a"=%d b=%d", a, b);
#define  BAR(a)	\
		a##bcd
#define  XXPRINTF(fmt, args...)	\
		printf("{test}"fmt, ##args)

int main()
{
	FOO(12, 20);
	BAR(1);
	XXPRINTF("my name is %s", "richard");
	return 0;
}
