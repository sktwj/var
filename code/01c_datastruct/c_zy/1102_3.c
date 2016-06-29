#include <stdio.h>

int main(void)
{
	int i, j, sum = 0;
	printf("1000以内的完数有：");
	for (i = 1; i <= 1000; i++) {
		for (j = 1; j < i; j++) {
			if (!(i%j)) {
				sum += j;
			}
		}
		if (sum == i) {
			printf("%d ", i);
		}
		sum = 0;
	}
	printf("\n");
	return 0;
}
