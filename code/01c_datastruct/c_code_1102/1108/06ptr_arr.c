#include <stdio.h>

int main(void)
{
	int a[] = {1,2,3};

	//数组名出现在表达式中，类型已经转换为指向数组首元素的指针的类型int *
	int *p = a + 1;
	printf("*p = %d\n", *p);
	printf("*p = %d\n", *(p - 1));

	//段错误(segmentation fault): 访问了无权访问的内存空间或者访问了NULL
	char arr[] = "123";
	char *parr = arr + 1;
	printf("parr = %s\n", parr);
	printf("*parr+1 = %c\n", *parr += 2);
	printf("arr = %s\n", arr);

	char *ps[4]; //指针数组：数组元素是指针类型的数组

	//数组指针：指向数组的指针
	char (*pa)[4]; //pa的类型是char (*)[4]
	pa = &arr; //数组名前加&，表示定义了一个指向整个数组的指针变量

	printf("pa = %s\n", pa);
	printf("arr[1] = %c\n", *(*pa + 1)); //*pa取出的是arr,类型是char*
	printf("arr[1] = %c\n", *((char *)pa + 1));
	printf("arr[1] = %c\n", *(((char *)(pa + 1)) - 3));

	//数组在以下情况，不会转换为指针来使用：
	//1、作为sizeof的操作数时，类型是数组类型类型
	//2、对数组取&时，表示取出整个数组的首地址

	return 0;
}
