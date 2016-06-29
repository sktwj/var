#include <stdio.h>

//指针：就是地址，仅仅是用来区分不同内存区域的一个地址值
//指针变量：用来存放变量的地址的变量，或者说以地址为值的变量
//通常把指针保存了一个变量的地址说成是：指针指向该变量
//指针的值：就是它保存的变量的地址
//指针的类型：去掉声明中变量名后剩余的部分
//指针指向的变量的类型：去掉声明中变量名和前面的*号剩余的部分
//指针的地址：我们通常无须关心

int main(void)
{
	int * p1, p2; //下面三种书写形式都可，但推荐第3种 
	int* p3, p4; 
	int *p5, p6 = 10; //*出现在声明中，仅仅用来说明p5是一个指针变量，不是普通的变量
	
	printf("size p5 = %d\n", sizeof(p5));

	p5 = &p6;
	// *出现在表达式中，表示间接访问（间接寻址），或者叫解引用指针
	printf("p6 = %d\n", p6); //直接访问
	printf("*p5 = %d\n", *p5); //间接访问形式1

	p3 = &p6;
	printf("*p3 = %d\n", *p3); //间接访问

	printf("==============二级指针===============\n");

	int **pp; //pp的类型int **, pp指向的变量的类型是int *

	// & 是取地址运算符，用来取出某变量占据的内存首地址
	pp = &p5;
	printf("&p5 = %p\n", &p5);
	printf("pp = %p\n", pp);
	printf("&p6 = %p\n", &p6);
	printf("*pp = %p\n", *pp);
	printf("**pp = %d\n", **pp); //间接访问形式2

	printf("&pp = %p\n", &pp);

	return 0;
}
