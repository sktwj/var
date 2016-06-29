#include <stdio.h>

static inline void merge_sort(int *s, int n)
{
	int m = n/2; //中值
	int l = 0; //左边一半最左边元素的下标
	int r = l + m; //右边一半最左边的元素的下标

	if (m > 0) {
		merge_sort(s, m); //中值左边递归
		merge_sort(s + m, n - m); //中值右边递归
	}

	int t[n]; //变长数组，用于临时存放排好序的元素

	int i = 0;
	while ((l < m) && (r < n)) {
		if (s[l] <= s[r]) {
			t[i++] = s[l++];
		} else {
			t[i++] = s[r++];
		}
	}

	while (l < m) { //右边一半先排完
		t[i++] = s[l++];
	} 
	while (r < n) { //左边一半先排完
		t[i++] = s[r++];
	}

	for (i = 0; i < n; i++) {
		s[i] = t[i];
	}
}

int main(void)
{
	int s[] = {8,1,2,5,0,3,7,2,9,6};
	int n = sizeof(s)/sizeof(int);

	merge_sort(s, n);
	int i = 0;
	for (; i < n; i++) {
		printf("%d ", *(s + i));
	}
	printf("\n");

	return 0;
}
