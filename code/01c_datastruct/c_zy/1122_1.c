#include <stdio.h>
#include <string.h>

void str_reverse(char *s, int n)
{
	char *p1 = s, *p2 = s;
	while (*(p2+1)) {p2++;}

	char tmp;
	for (; p1 < p2; p1++, p2--) {
		tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
	}
}

int itoa(char *s, int n)
{
	int count = 0;
	if (n < 0) {
		s[0] = '-';
		count++;
	} else {
		s[0] = ' ';
	}
	int m = (n < 0 ? -n : n);
	int i = 1;
	while (m) {
		s[i++] = m%10 + '0';
		count++;
		m /= 10;
	}
	s[i] = '\0';

	str_reverse(s+1, i+1);
	if (n >= 0) {
		strcpy(s, s+1);
	}
	return count;
}

int main(void)
{
	int n = +-1234;
	char s[100] = {0};
	
	printf("成功打印的字符个数＝%d\n", \
			itoa(s, n));

	printf("s = %s\n", s);

#if 0
	//printf函数的返回值是成功打印的字符的个数
	printf("%d\n", printf("%d", printf("43")));
	printf("%s\n", "hello world");
	fprintf(stdout, "%s\n", "hello world");
#endif
#if 1
#define LEN 100
	char st[LEN] = {0};
	printf("n = %d\n", sprintf(st, "%d", +-1234));
	printf("st = %s\n", st);
#endif

	return 0;
}
