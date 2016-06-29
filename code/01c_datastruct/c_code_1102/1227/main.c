#include "sort.h"

int main(void)
{
	struct sort s;
	sort_init(&s);

	elem_t arr[] = {9,0,2,1,6,3,1,-3,4,5};
	size_t n = sizeof(arr)/sizeof(elem_t);

	//s.bubble(arr, n);
	//s.select(arr, n);
	//s.insert(arr, n);
	//s.quick(arr, n);
	//s.heap(arr, n);
	//s.shell(arr, n);
	s.merge(arr, n);

	printf("after sort: \n");
	size_t i = 0;
	for (; i < n; i++) {
		printf("%d ", arr[i]);
	}
	printf("\n");

	return 0;
}
