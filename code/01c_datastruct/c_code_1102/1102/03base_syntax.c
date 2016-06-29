#include <stdio.h>
#include <stdbool.h>

int main(void)
{
	//<一>数据类型
	//任何变量都是有类型的，类型的意义在于：
	//1、用于决定编译器需要为此变量开辟的字节数
	//2、决定了变量能够执行的操作
#if 0
	0、从有无符号来分：分为有符号类型和无符号类型
	size_t <==> unsigned int //32位
		   <==> unsigned long //64位	

	ssize_t <==> signed int //32位
			<==> signed long //64位	
#endif

	//1、基本数据类型：整型、浮点型、布尔型、枚举类型、指针、函数
	char c = 65;
	int i = 5;
	long int li = 10; //long li;
	short int si = 1; //short si;
	printf("c = %c, size c = %d\n", c, sizeof(c));
	printf("i = %d, size i = %u\n", i, sizeof(i));
	printf("li = %ld, size li = %u\n", li, sizeof(li));
	printf("si = %hd, size si = %u\n", si, sizeof(si));

	float f = 0.1; //单精度浮点型
	double d = 0.11; //双精度浮点型
	long double ld = 0.111; //长双精度
	printf("f = %f, size f = %u\n", f, sizeof(f));
	printf("d = %lf, size d = %u\n", d, sizeof(d));
	printf("ld = %LF, size ld = %u\n", ld, sizeof(ld));

	bool b = 10; //bool型只有真和假两个值，在C中通常用0表示假，用非0表示真
	printf("b = %d, size b = %u\n", b, sizeof(b));

	enum efoo { red = 100, blue, green, };
	enum efoo e = blue;
	//e = 200; //在C++中无法编译通过，因为C++语法规定，枚举类型的变量只能从枚举列表中取值
	printf("e = %d, size e = %u\n", e, sizeof(e));
	printf("green = %d\n", green);

	int *p; 
	printf("p = %p\n", p);
	printf("&p = %p\n", &p);
	//printf("*p = %p\n", *p); //试图访问指针指向的内存中的值
	printf("size p = %d\n", sizeof(p));
	printf("size *p = %d\n", sizeof(*p)); //sizeof是在编译期间就可以确定值了

	//sizeof是C中运算符，用来计算某种类型的变量在内存中占据的字节数的
	int a = 10;
	printf("sizeof(a) = %d, sizeof(int) = %d\n",
			sizeof(a), sizeof(int));
	printf("sizeof a = %d\n", sizeof a);
	//printf("sizeof int = %d\n", sizeof int); //error，类型出现在sizeof的操作数位置，必须有()
	printf("sizeof(a++ + ++a) = %d\n", 
			sizeof(a++ + ++a));
	printf("a = %d\n", a);

	//typeof是运算符号，是GNU C对标准C的扩展，用来取出变量的类型
	typeof(a) tb = 20;
	printf("tb = %d\n", tb);

	//foo函数的类型是： void (int, int)
	void foo(int a, int b)
	{
		printf("foo, %s\n", __func__);
	}
	foo(1,2);

	//2、组合类型：数组、结构体、联合体
	//s数组的类型是：char [5]
	//s数组中每个元素的类型是：char
	char s[] = {"1234"};
	printf("size s = %d\n", sizeof(s));
	printf("size s = %d\n", sizeof(char [5]));
	printf("count of s = %d\n", 
			sizeof(char [5])/sizeof(char));

	struct sfoo {
		char c1;
		char c2;
		int num;
	} sf = {1, 2, 10}, *psf = &sf;
	printf("size sf = %d\n", sizeof(struct sfoo));
	printf("sf.num = %d\n", sf.num);
	printf("psf->num = %d\n", psf->num);

	//联合体所有成员共用同一块内存空间，大小取决于其中最大成员的大小
	union ufoo {
		int i;
		char c;
		short s;
	};
	union ufoo uf = {0x12345678};
	uf.i = 0x12345678;
	printf("size uf = %d\n", sizeof(union ufoo));
	union ufoo *puf = &uf;
	printf("uf.s = 0x%x\n", uf.s);
	printf("uf.c = %#x\n", uf.c);
	printf("puf->c = %#x\n", puf->c);

	//<二>运算符
	//逗号运算符 ,
	int aa = 3, bb = 4;
	int ee = (aa += 1, bb += aa); //逗号表达式的值取决于最后一个表达式的值
	printf("ee = %d\n", ee);
	
	int cc;
	int dd = (3, cc = aa);
	printf("dd = %d\n", dd);

	//?: 条件运算符 expr1 ? expr2 : expr3
	(aa > 0) ? (aa += bb) : (aa -= bb);
	printf("aa = %d\n", aa);

	// && || 逻辑运算符
	// expr1 && expr2  如果expr1为假，不执行expr2
	// expr1 || expr2 如果expr1为真，不执行expr2
	int ff = (aa == 1) && (bb += 2) || (cc += 1);
	printf("ff = %d\n", ff);

	//++ -- 运算符号	
	int num = 10;
	printf("后置++: %d\n", num++); 
	printf("前置++: %d\n", ++num); 

	num = 10;
	//大部分编译器对于参数入栈的顺序是：最后一个参数先入栈，依次是之前的参数
	//前置++和--改变的是变量本身
	//后置++和--取出的是当前表达式的值，是一个瞬间产生的临时变量
	//12 13 13
	printf("%d, %d, %d\n", num++, ++num, ++num);
	num = 10;
	printf("%d, %d, %d, %d, %d, %d\n", 
	num, --num, num--, num++, ++num, ++num);
	printf("num = %d\n", num);

	return 0;
}
