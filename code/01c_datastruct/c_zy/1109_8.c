#include <stdio.h>

int count = 0;
static inline void move(char start, char end, int n)
{
	printf("move %d: %c --> %c\n", n, start, end);
	count++;
}

static inline void hanoi(char start, char mid, char end, int n)
{
	if (n == 1) { //递归的结束条件
		move(start, end, n);
	} else {
		hanoi(start, end, mid, n - 1);
		move(start, end, n);
		hanoi(mid, start, end, n - 1);
	}
}

int main(void)
{
	int n = 10;
	hanoi('A', 'B', 'C', n);
	printf("移动%d个盘子，共需要%d步\n", n, count);
	return 0;
}
