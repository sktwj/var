#include <stdio.h>

//变长数组VLA: variable length array
//变长数组存在的意义是：将数组空间的开辟推迟到程序运行期间

int n = 3;
//int a[n]; //不允许定义全局的变长数组。该数组存放在.bss段，是在程序一开始（即编译之前）就初始化好了，它的长度不可能由之后存放在.data段的某个变量的值来决定，所以不允许。

int main(void)
{
	int n = 3;
	int a[n]/* = {0}*/; //变长数组：变长数组的长度并不会因为其中表示元素个数的变量的改变而发生变化。变长数组不允许在声明的同时初始化。仅仅是因为数组的元素个数不是常量或常量表达式，而是变量，才命名为变长数组。
	printf("size a = %d\n", sizeof(a));

	n = 5;
	printf("size a = %d\n", sizeof(a));

	int i = 0;
	for (; i < n; i++) {
		a[i] = i;
	}

	return 0;
}
