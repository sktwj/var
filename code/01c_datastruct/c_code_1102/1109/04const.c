#include <stdio.h>

int main(void)
{
	const int a/* = 10*/; //const在C中从来没有常量的意思，只有只读(read-only)的意思。

	//a = 10; //只读类型的变量在声明的同时没有赋值，永远没有机会赋值了，因为除了声明之外的赋值，都是企图在改变该变量的值。

	int b;
	b = 20;
	
	int const *p2; //该声明和下一行没有差异，只是书写形式不同
	const int *p1 = &b; //仅仅限定了，无法通过p1去修改它指向的变量的值 
	//*p1 = 30; //error
	int *p4 = &b;
	*p4 = 40; //可以通过其他方式修改变量b的值
	//b = 30; //同上
	printf("b = %d\n", b);

	p1 = &a; //p1本身是可以改变指向的
	//*p1 = 15; //*p1 被限定为const，所以依然无法通过p1去修改它指向的变量的值
	printf("a = %d\n", a);

	int *p6 = &a; //警告，p6指向的变量本身是const的，如果没有发出警告，会让我们误以为可以通过p6去修改a的值

	int * const p3 = &a; //p3指针变量本身是const的，也就是说它的指向是无法修改的
	*p3 = 25; //上面一行会有警告，因为p3指向的a变量本身是const的，那么如果编译器不发出警告，意味着可以轻易的通过p3去修改它指向的a变量的值，这当然就违背了a被const限定的本意。
	printf("a = %d\n", a);

	//p3 = &b; //error，p3是const的，所以无法修改p3的指向

	const int * const p5 = &b;
	//*p5 = 50; //error
	//p5 = &a; //error

	return 0;
}
