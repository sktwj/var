#include <stdio.h>

//结构体的内存对齐规则(因不同系统不同编译器而异)：
//1、每个成员的首地址是在必须能够被自身长度和cpu字长(指针的长度)中比较小的那个整除的地址上
//2、最后一个成员负责整个结构体占据的内存空间能够被cpu字长整除

//结构体：由不同或相同类型的成员组成的集合
#pragma pack(1) 
struct foo {
	char c; //1+1
	short b; //2
	int a; //4
	int d; //4
	double f; //8
};

//pragma 指令用于执行系统预先定义好的某种行为
#pragma pack() //用于改变结构体的默认对齐系数，()中的数字可以是2的幂，但是超过cpu字长将恢复默认值4
struct foo2 {
	int a;
	char c;
	short b;
};

int main(void)
{
	printf("sizeof(\"hello\") = %d\n", sizeof("hello"));
	printf("sizeof(\"hello\" + 1) = %d\n", sizeof("hello" + 1));

	printf("目前struct foo的默认对齐系数是：%d\n", __alignof__(struct foo));
	printf("size foo = %d\n", sizeof(struct foo));
	
	printf("目前struct foo2的默认对齐系数是：%d\n", __alignof__(struct foo2));
	printf("size foo2 = %d\n", sizeof(struct foo2));
	return 0;
}
