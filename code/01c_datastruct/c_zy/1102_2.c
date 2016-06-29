#include <stdio.h>

int main(void)
{
	char c;
	//EOF可以用来表示文本文件尾，也可以用来表示输入结束(需要使用ctrl+d键表示)
	while ((c = getchar()) != EOF) {
		if (c >= 'a' && c <= 'y') {
			putchar(c + 1);
		} else if (c == 'z') {
			putchar('a');
		} else {
			putchar(c);
		}
	}
	return 0;
}
