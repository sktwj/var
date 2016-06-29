#include <stdio.h>

int find_second_max(int *a, int n)
{
	int i, m1 = a[0], m2 = a[1]; //m1记录最大的元素值，m2记录次大的元素值
	
	for (i = 1; i < n; i++) {
		if (a[i] > m1) {
			m2 = m1;
			m1 = a[i];
		} else if (a[i] > m2) {
			m2 = a[i];
		}
	}

	return m2;
}

int main(void)
{
	int a[] = {4,1,6,2,17,9,0,3,8,5};
	int n = sizeof(a)/sizeof(int);
	printf("数组中第二大的元素值是：%d\n", 
			find_second_max(a, n));
	return 0;
}
