#include <stdio.h>

int test_increment_arr(int *a, int n)
{
	if (n == 1) { //递归的结束条件
		return 1;
	} 
	return test_increment_arr(a, n - 1) 
		&& (a[n - 2] < a[n - 1]);
}

int main(void)
{
	int a[] = {1,2,3,4,5,6, 0};
	int n = sizeof(a)/sizeof(a[0]);
	if (test_increment_arr(a, n)) {
		printf("是递增的数组！\n");
	} else {
		printf("不是递增的数组！\n");
	}

	return 0;
}
