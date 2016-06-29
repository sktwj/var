#include <stdio.h>

int m_atoi(const char *s)
{
	int flag = 1;
	if (*s == '-') {
		flag = -1;
		s++;
		if (*s == '+') {
			return 0;
		}
	} else if (*s++ == '+') {
		if (*s == '-') {
			return 0;
		}
	}
	int n = 0;
	for (; *s; s++) {
		if ((*s >= '0') && (*s <= '9')) {
			n = n * 10 + (*s - '0');
		} else {
			break;
		}
	}

	return flag * n;
}

int main(void)
{
	const char *s = "-1234 hello 123";
	printf("%d\n", m_atoi(s));

	printf("atoi, %d\n", atoi("-1234 hello 123"));

	return 0;
}
