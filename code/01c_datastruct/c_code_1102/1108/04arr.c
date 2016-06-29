#include <stdio.h>
#include <string.h>

//数组：同类型的数据元素的集合

int main(void)
{
	//a数组的类型是int [2]; 其中每个元素的类型是int
	int a[] = {1,2,}; //未赋值元素只能堆积在赋值列表的最后

	printf("sizeof(a) = %d\n", sizeof(a));
	printf("count of element = %d\n", 
			sizeof(a)/sizeof(int));

	// '\0'是字符串的结束标记，字符串是一种特殊的字符数组
	char s[] = {'1', '2'}; //s是字符数组，不是字符串

	//下面两种赋值方式相同，数组的类型都是char [3]
	char s2[] = {"12"};
	char s3[] = "12"; //即是字符数组，也是字符串
	printf("size s = %d, size s3 = %d\n", 
			sizeof(s), sizeof(char [3]));

	//strlen用来计算字符串的有效长度，不包括字符串的结束标记'\0'，所以对于字符数组来说，在遇到第一个'\0'之前的所有字符都会被认为是该字符数组的有效内容。
	printf("s = %s, strlen s = %d\n", s, strlen(s));
	printf("strlen s3 = %d\n", strlen(s3));

	printf("==============二维数组================\n");	

	//arr数组的类型是：int [3][2]
	//arr数组只有3个元素，分别是arr[0],arr[1],arr[2],他们又分别是另一个一维数组,他们的类型是int [2],他们中的每个元素的类型是int
	int arr[][2] = {{1,2}, {3,4}, {5,6}};
	int brr[][2] = {1,2, 3,4, 5,6};
	printf("size arr = %d\n", sizeof(int [3][2]));
	printf("size arr[0] = %d\n", sizeof(arr[0]));
	printf("size arr[0] = %d\n", sizeof(int [2]));
	
	char str[][10] = {"hello", "world", "linux"};
	printf("size str = %d\n", sizeof(str));
	printf("str[1] = %s\n", str[1] + 2);
	
	printf("str + 1 = %s\n", str + 1);
	printf("str + 1 = %s\n", *(str + 1));

	return 0;
}
