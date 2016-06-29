#include <stdio.h>

//非递归查找，返回的是查找的元素的下标
int binary_search(int *a, int num, int l, int r)
{
	int mid; //中值元素的下标

	while (l <= r) {
		mid = (l + r)/2;
		if (num < a[mid]) {
			r = mid - 1;
		} else if (num > a[mid]) {
			l = mid + 1;
		} else {
			return mid;
		}
	}

	return -1;
}

int binary_search_rec(int *a, int num, int l, int r)
{
	if (l > r) { //递归的结束条件
		return -1;
	}

	int mid = (l + r)/2;
	if (num == a[mid]) {
		return mid;
	} else if (num < a[mid]) {
		r = mid - 1;
		return binary_search_rec(a, num, l, r);
	} else {
		l = mid + 1;
		return binary_search_rec(a, num, l, r);
	}
}

int main(void)
{
	int a[] = {0,1,2,3,4,5,16,17,18,19};
	int n = sizeof(a)/sizeof(int);
	int search = 8;
	int k = binary_search_rec(a, search, 0, n - 1);
   	if (k != -1) {
		printf("找到了，元素的下标是：%d\n", k);
	} else {
		printf("没找到！\n");
	}
	return 0;
}
