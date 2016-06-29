#include <stdio.h>
#include <stdarg.h>
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
	//s[i] = '\0';

	str_reverse(s+1, i+1);
	if (n >= 0) {
		strcpy(s, s+1);
	}
	return count;
}

#define CNT 100
int m_sprintf(char *arr, const char *fmt, ...)
{
	int count = 0;
	int d = 0;

	char *save = arr;
	va_list ap;
	va_start(ap, fmt);

	while (*fmt) { 
		if (*fmt == '%') {
			fmt++;
			switch (*fmt) {
			case 'd':
				d = itoa(arr, va_arg(ap, int));
				count += d;
				arr += d;
				break;
			case 'c': {
				*arr = (char)va_arg(ap, int);
				count++;
				arr++;
				break;
			}
			case 's': {
				char *p1 = va_arg(ap, char *);
				char *p2 = p1;
				while (*p1++ != '\0') {
					count++;
				}
				strcpy(arr, p2);
				arr += strlen(p2);
				break;
			}
			case '%':
				strcpy(arr, fmt);
				count++;
				arr++;
				break;
			default:
				strcpy(arr, "%");
				count++;
				arr++;
			}
		} else {
			strncpy(arr, fmt, 1);
			count++;
			arr++;
		}
		fmt++;
	}

	va_end(ap);

	return count;
}

int main(void)
{
	char s[CNT] = {0};
	int n;
	n = m_sprintf(s, "int = %d, char = %c, str = %s %% %z\n", 1234, 'A', "world linux");
	printf("m_printf, n = %d\n", n);
	printf("%s\n", s);

	//n = sprintf(s, "int = %d\n", 1234);
	//n = sprintf(s, "int = %d, char = %c, str = %s %% %z\n", 1234, 'A', "world linux");
	//printf("printf, n = %d\n", n);
	//printf("%s\n", s);

	return 0;
}
