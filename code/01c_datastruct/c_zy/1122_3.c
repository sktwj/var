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
	s[i] = '\0';

	str_reverse(s+1, i+1);
	if (n >= 0) {
		strcpy(s, s+1);
	}
	return count;
}

int m_printf(const char *fmt, ...)
{
	int count = 0;
#define CNT 100
	char s[CNT] = {0};

	va_list ap;
	va_start(ap, fmt);

	while (*fmt) { 
		if (*fmt == '%') {
			fmt++;
			switch (*fmt) {
			case 'd':
				count += itoa(s, va_arg(ap, int));
				fputs(s, stdout);
				break;
			case 'c':
				putchar(va_arg(ap, int));
				count++;
				break;
			case 's': {
				char *p1 = va_arg(ap, char *);
				char *p2 = p1;
				while (*p1++ != '\0') {
					count++;
				}
				fputs(p2, stdout);
				
				break;
			}
			case '%':
				putchar(*fmt);
				count++;
				break;
			default:
				putchar('%');
				count++;
			}
		} else {
			putchar(*fmt);
			count++;
		}
		fmt++;
	}

	va_end(ap);

	return count;
}

int main(void)
{
	int n;
	n = m_printf("int = %d, char = %c, str = %s %% %z\n", 1234, 'A', "world linux");
	printf("m_printf, n = %d\n", n);
	
	n = printf("int = %d, char = %c, str = %s %% %z\n", 1234, 'A', "world linux");
	printf("printf, n = %d\n", n);

	return 0;
}
