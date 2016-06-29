#include <stdio.h>

int main(void)
{
	int arr[] = {1,2,3};
	//arr = &arr[1];
	//arr = arr + 1; //=号左边的arr的类型是int [3]
				   //=号右边的arr的类型是int *
	
	arr[0] = arr[1];
	int *b = &arr[3]; //访问数组中最后一个元素之后的那个地址是合法的

	//arr[3] = {2,3,4}; //试图修改数组最后一个元素之后的那个地址中的值，这在C语言中是不合法的
	
	int (*pa)[3] = &arr;
	printf("arr[2] = %d\n", *((int *)(pa + 1) - 1));

	return 0;
}
