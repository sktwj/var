#include <stdio.h>

static inline void swap(int *a, int *b)
{
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

//先把大的元素排好
void bubble_sort(int *a, int n)
{
	int i, j;
	for (i = 0; i < n - 1; i++) {
		for (j = 0; j < n - 1 - i; j++) {
			if (a[j] > a[j+1]) {
				swap(a + j, a + j + 1);
			}
		}
	}
}

//先把小的元素排好
void bubble_sort_small(int *a, int n)
{
	int i, j;
	for (i = 0; i < n - 1; i++) {
		for (j = n - 1; j > i; j--) {
			if (a[j] < a[j-1]) {
				swap(a + j, a + j - 1);
			}
		}
	}
}

void select_sort(int *a, int n)
{
	int i, j, min; //min用于记录值最小的元素的下标
	for (i = 0; i < n - 1; i++) {
		for (min = i, j = i+1; j <= n - 1; j++) {
			if (a[j] < a[min]) { //找到了值更小的元素
				min = j;
			}
		}
		if (min != i) { //说明确实找到了值更小的元素
			swap(a + i, a + min);
		}
	}
}

int main(void)
{
	int a[] = {3,1,8,6,5,2,7,0,9,4};
	int n = sizeof(a)/sizeof(*a);

	//bubble_sort(a, n);
	//bubble_sort_small(a, n);
	select_sort(a, n);
	
	int i = 0;
	for (; i < n; i++) {
		printf("%d ", a[i]);
	}
	printf("\n");

	return 0;
}
