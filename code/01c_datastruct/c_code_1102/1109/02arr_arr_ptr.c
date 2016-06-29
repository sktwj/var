#include <stdio.h>

int main(void)
{
	char s[][10] = {"hello", "world", "linux"};//字符数组
	char *t[3] = {"hello", "world", "linux"}; //字符指针数组

	printf("size s = %d\n", sizeof(char [3][10]));
	printf("size t = %d\n", sizeof(char *[3]));

	*(*(s + 1) + 2) = '2';
	printf("s[1] = %s\n", s + 1);

	//*(*(t + 1) + 2) = '2'; //error, 试图修改.rodata段内的"world"
	printf("t[1] = %s\n", *(t + 1)); //t+1中t的类型是char**

	char (*ps)[3][10] = &s;
	printf("s[2] = %s\n", s + 2); //s的类型是char (*)[10]
	printf("s[2] = %s\n", *(s + 2)); //整个表达式的类型是char *

	printf("s[2][1] = %c\n", *(*(*ps + 2) + 1));

	char (*pa)[10] = s + 1;
	printf("s[0] = %s\n", pa - 1);
	printf("s[0][1] = %c\n", *(*(pa - 1) + 1));

	pa = &s[1]; //&s[1] = &*(s + 1) = s + 1

	char *pb = s[1] + 1; //s[1]表示二维数组中第1个元素，s[1]的类型是char*，因为在表达式中它表示指向数组首元素的指针
   	printf("&s[1][1] = %s\n", pb);	
   	printf("s[1][1] = %c\n", *pb);	
	pb = *(s + 1) + 1;

	//定义指向整个t数组的指针
	char *(*pc)[3] = &t;
	//char *(*pc[3]) = &t;
	printf("t[1] = %s\n", *(*pc + 1));
	printf("&t[1] = %p\n", t + 1);
	printf("&t[1][0] = %p\n", *(t + 1));

	return 0;
}
