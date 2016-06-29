#include <stdio.h>

#if 0
	变量的声明：存储类型 数据类型 变量名；
	存储类型：extern static register volatile

	变量的分类：
	作用域：变量可以被访问到的区域
		1> 从作用域的角度来说：
		a.文件作用域
		b.函数原型作用域
		c.函数作用域
		d.代码块作用域

	生命周期：变量被开辟内存空间到该空间被回收的这段时间
		2> 从生命周期的角度来说：
		a.静态生命周期：在程序编译期间就存在了，在整个程序终止时空间才被回收
		b.自动生命周期：在其所在函数被调用时被开辟空间，到函数调用结束被回收空间
		c.动态生命周期：在程序运行期间才被开辟空间，到调用内存释放函数后才会回收空间
#endif

// nm 可执行文件 //可以用来查看可执行文件中的符号表
// size 可执行文件 //可以查看可执行文件中各段的大小
// .bss: block started by symbol 以符号开始的块，用来存放程序中未初始化的数据的区域，仅仅简单的维护该段内的变量的起始地址和结束地址，并不存值，在程序初始化时被系统清0的


int a; //在.bss段，全局变量，具有文件作用域(在整个工程内都能被访问)
static int sa; //在.bss段，静态全局变量，具有文件作用域(但仅在该文件内可以被访问)
static int ss = 10; //在.data段，静态全局变量
static int ss1 = 0; //在.bss段，静态全局变量
int ss2 = 20; //在.data段
int ss3 = 0; //在.bss段
const int ca = 100; //在.rodata段

//static 的作用：
// 1、修饰全局变量：改变了变量的作用域，使得该变量成为本文件私有的全局变量
// 2、修饰局部变量：改变了变量的生命周期，使得变量的生命周期变长了，成为了常驻内存的变量; 使得该变量的值的初始化只会进行一次，值是在前次值的基础上发生变化的
// 3、修饰函数：使得该函数成为本文件私有的函数

static int foo2(void); //静态函数的原型声明方式
int main(void)
{
	printf("global a = %d, sa = %d\n", a, sa);
	int a = 2; //在stack段，函数作用域，自动生命期，每次在其函数被调用时，并不能保证每次位于内存中的同一个内存地址
	static int sa; //在.bss段，函数作用域，静态生命期
	printf("function sa = %d\n", sa);

	static int ss1 = 0; //在.bss段


	void foo(int a) //C99标准允许函数的嵌套定义
	{
		//a是函数原型作用域
		//printf("%s, a = %d\n", __FUNCTION__, a);
		int b = 0; //函数作用域
		static int b1;

		b++;
		b1++;

		printf("foo, b = %d, b1 = %d\n", b, b1);
		printf("foo, &b = %p, &b1 = %p\n", &b, &b1);
	}

	foo(1);
	foo(1);
	foo(1);

	{
		int a = 3; //a具有代码块作用域
	}

	//extern void by_called(void); //通常全局函数的声明都是省略不写的，因为函数默认都是全局的，除非用static限定
	by_called();

	foo2();

	return 0;
}

static int foo2(void)
{
	printf("foo:: %s\n", __func__);
}
