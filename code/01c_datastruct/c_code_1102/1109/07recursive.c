#include <stdio.h>

//递归：直接或间接调用自己
//递归函数：直接或间接调用自己的函数，被称为递归函数
//迭代（循环）和递归的异同点：
//1、迭代基于循环结构；递归基于选择结构
//2、都有可能出现无限执行的情况：迭代在判断条件永远不可能为假时会出现无限循环（死循环）；递归在永远无法到达结束条件时，会出现无限递归
//3、都需要检查结束条件
//4、迭代是用过反复的执行同一段代码实现的；递归是通过不断的进行函数的调用实现的
//5、递归相对于迭代来说，效率是比较低的，在时间和空间的开销上来说都是如此（因为函数的调用所伴随的就有入栈和出栈的开销）

//完成递归的必备条件：1、每次的递归所面临的是更小规模的同样问题；2、必须有结束条件
//任何可以使用递归实现的程序，都可以转化为使用迭代
//递归和迭代的选择：如果程序对于效率的要求是比较严格的，那么请不要使用递归；如果程序对于可读性的要求更高，那么推荐使用递归。所以主要还是取决于程序的实际要求。递归带来的好处就是牺牲效率来达到增强可读性的需要。

int factorial(int n)
{
	if (n == 1) {
		return 1;
	}
	return n * factorial(n - 1);
}

int norec_factorial(int n)
{
	int ret = 1;
	while (n >= 1) {
		ret *= n--;
	}
	return ret;
}

void foo(int n)
{
	if (n > 0) {
		foo(--n);
		printf("%d ", n);
		foo(--n);
	}	
}

int main(void)
{
	int n = 3;
	foo(n);

	printf("\nrec, %d! = %d\n", 10, factorial(10));
	printf("norec, %d! = %d\n", 10, norec_factorial(10));
	return 0;
}