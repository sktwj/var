#include <stdio.h>

#if 0
	C中以#号开头的行被称为预处理指令，分为3种：
	1、文件包含 
		#include <系统的头文件/usr/include/xx.h>
		#include "自己的头文件（可以是相对路径，也可以是绝对路径）"
	区别是：搜寻的路径不同，""是先在当前目录下寻找，没找到，会继续到系统头文件所在目录进行查找，还没找到，才报错
			<>是直接去系统头文件所在目录下查找，没找到就直接报错退出

	2、宏：就是通常说的宏替换，或者叫宏代换。它是字符串类型的原样替换，它只管换，不做任何其他工作。
	   分为无参数宏（通常也称为符号常量）和有参数宏
		#define 宏名 宏替换列表
	   	#define 宏名(参数1, 参数2, ...) 宏替换列表（可以包含语句）

	3、条件编译
		形式1：
		#if 0/1
		...
		#endif

		形式2：
		#ifdef xx宏  <==> #if defined xx宏
		...
		#elif yy宏
		...
		#else
		...
		#endif

		形式3：
		#ifndef xx宏 <==> #if ! defined xx宏
		...
		#elif yy宏
		...
		#else
		...
		#endif

#endif

int main(void)
{
	//1、定义无参宏（符号常量）：宏替换列表应该整体用()括起来
	//在程序中符号常量经常出现的地方是数组的元素个数的位置。定义符号常量的好处是：1、为了程序维护起来更加方便，因为一般宏名都可以望名知义；2、可以起到一改全改的效果。像10这类不明其义的值尽量不要出现在程序中，通常我们称之为幻数。
	#define M (10 + 10)
	printf("M = %d\n", M * M);

	//取消宏定义：
	#undef M
	//printf("M = %d\n", M * M);

	//2、定义带参数的宏  
	//\被称为续行符，用于连接处于多行的内容为一行，\之后一直到该行尾不能有任何的字符
	#define MUL(n) ((n) * (n)) //宏替换列表中的每个参数都应该用()括起来
	int n = 1;
	printf("MUL: %d\n", 100/MUL(n+1)*MUL(n+1));
	
	#define MAX(a, b) \
		((a) > (b) ? (a) : (b))

	//GUN C 对于传递有副作用的参数时，是能够正常处理的，但ANSI/ISO C 无法处理
	#define GNU_MAX(a, b) \
		({ int m = a, n = b; m > n ? m : n;})

	int max(int a, int b) {
		return a > b ? a : b;
	}

	int a = 3, b = 4;
	printf("macro, max = %d\n", MAX(3, 4));
	//printf("macro, max = %d\n", MAX(a++, b++));
	//printf("GNU_macro, max = %d\n", GNU_MAX(a++, b++));
	printf("function, max = %d\n", max(a++, b++));

	//使用有参数的宏 和 使用函数的区别：
	//1、宏是在预处理时进行替换和展开的；函数是在程序运行阶段进行压栈处理的
	//2、宏是在预处理时将宏直接用替换列表进行替换，所以没有函数在调用时的开销
	//3、宏在预处理之后已经被删除了，所以无法在程序运行期间对宏进行调试，但函数可以
	//4、虽然宏没有函数入栈出栈的开销，也就是说效率会相对比较高，但与此同时带来的后果是使得可执行文件的尺寸变大了
	//5、宏和inline函数都是将代码直接进行粘贴这一点是相同的，但区别是宏参数是无类型的，inline函数(不过就是函数的一种)的参数是有类型的，所以在被调用时，编译器是不会检查传递给宏的参数的，而函数的参数都会进行类型的检查。

	// #被称为字符串化符， #宏参数 --> "宏参数"
	#define STRLIZE(s) #s
	printf("str, %s\n", STRLIZE(hello));

	// ##被称为粘贴符号，将多个宏参数拼接为一个
	#define PASTE(a, b, c) a##b ## c
	printf("paste 1, %lf\n", PASTE(1,e,3));
	printf("paste 2, %d\n", PASTE(1,,3));

	#define A(x) #x
	#define B(x) A(x) 
	//宏参数带有##或#时，将会阻止宏的继续展开
	printf("1::, %s\n", A(B(123))); //"B(123)"
	printf("3::, %s\n", A(A(123))); //"A(123)"
	
	//如果宏在展开时，发现替换列表是另外一个宏，那么这个对应的宏被继续展开，直到不能展开为止。
	printf("2::, %s\n", B(A(123))); 
	//B(A(123))--> A(A(123))--> A("123") --> "\"123\""
	//B(A(123))--> B("123") --> A("123") --> "\"123\""	
	printf("4::, %s\n", B(B(123))); 
	//B(B(123))--> A(B(123))--> "B(123)" //从外向里展开 
   	//B(B(123))--> B(A(123))--> B("123")--> A("123") 
	//--> "\"123\""	 //从里向外展开

	printf("A(123) = %s\n", A(123));

	return 0;
}