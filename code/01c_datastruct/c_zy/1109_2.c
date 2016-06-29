#include <stdio.h>

void strreverse(char *s)
{
	char *start = s, *end = s;
	for (; *(end + 1)!='\0'; end++) {} //end定位到'\0'之前的那个字符

	char tmp;
	for (; start < end; start++, end--) {
		tmp = *start;
		*start = *end;
		*end = tmp;
	}
}

int main(void)
{
	char s[] = "world";
	strreverse(s);
	printf("after reverse, s = %s\n", s);
	return 0;
}
