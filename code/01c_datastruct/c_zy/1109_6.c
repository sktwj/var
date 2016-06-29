#include <stdio.h>

void reverse_str(char *s, int l, int r)
{
	if (l >= r) { //递归的结束条件
		return;
	}

	char tmp = s[l];
	s[l] = s[r];
	s[r] = tmp;

	reverse_str(s, l + 1, r - 1);
}

void __reverse_str(char *s, int l, int r)
{
	if (l < r) { 
		char tmp = s[l];
		s[l] = s[r];
		s[r] = tmp;
	} else { //递归的结束条件
		return;
	}

	__reverse_str(s, l + 1, r - 1);
}
int main(void)
{
	char s[] = "world";
	int n = sizeof(s)/sizeof(s[0]);
	__reverse_str(s, 0, n - 2);
	printf("s = %s\n", s);
	return 0;
}
