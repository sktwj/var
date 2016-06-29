#include <stdio.h>

void print_reverse_str(char *s)
{
	if (*s == '\0') {
		return;
	}

	print_reverse_str(s + 1);
	printf("%c", *s);
}

int main(void)
{
	char s[] = "world";
	print_reverse_str(s);
	printf("\n");

	return 0;
}
