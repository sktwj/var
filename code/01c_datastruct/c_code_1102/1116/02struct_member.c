#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct foo {
	int a;
	char *p;
	char s[10];
};

int main(void)
{
	{
	int a[3] = {0,1,};
	//int aa[3];
	//aa = a;

	int b[3];
	//b = {0,1,2}; //b放在赋值号左边，就表示数组所在内存的首地址，是一个常量
	int arr[3] = {[1] = 1,};
	printf("arr[0] = %d\n", arr[0]);
	}

	//分量赋值方式1：.成员名=所赋的值
	struct foo f = {.s = "world", .p = "hello",};
	struct foo ff = f; //结构体唯一支持的运算操作
	printf("ff.s = %s\n", ff.s);
#if 0
	if (ff == f) { //结构体无法整体判等，只能对其中的成员进行分量的比较
		printf("相等\n");
	} else {
		printf("不等\n");
	}
#endif

	//分量赋值方式2：成员名: 所赋的值
	struct foo f2 = {p: "hello",};
	printf("f.p = %s\n", f.p); //f.p中的.称为成员运算符
	printf("f2.p = %s\n", f2.p);

	struct foo f3 = {10, "hello", };
	printf("size f3 = %d\n", sizeof(f3));

	struct foo f4;
	//f4 = {10,"hello","world"}; //同14行
	f4.a = 10;
	f4.p = "hello";
	//f4.s = "world"; //error，同14行
	strcpy(f4.s, "world");

	struct foo f5 = {
		10, 
		.s = "world", 
	};

	//malloc函数用于程序在执行过程中动态的申请堆内存空间，申请的空间并不会被初始化。申请堆内存之后，都应该判断是否申请成功，如果出现了申请失败的情况，通常意味着已经出现了内存泄露。
	f5.p = (char *)malloc(sizeof("hello"));
	if (!f5.p) {
		printf("malloc failed!\n");
		return 1;
	}
	memset(f5.p, 0, sizeof("hello")); //通常memset函数是配合malloc函数来使用的，用于将malloc申请的堆空间清0
	strcpy(f5.p, "hello");
	printf("f5.p = %s\n", f5.p);

	struct foo *pf = &f5;
	printf("pf->s = %s\n", pf->s); //->被称为箭头运算符，左操作数必须是指针类型的变量

	//可以使用" valgrind 可执行文件 "来检查是否出现了内存使用问题，特别是内存泄露
	free(f5.p);

	printf("＝＝＝申请堆内存＝＝＝\n");
	//calloc申请的堆空间是会被自动清0的，这也是和malloc申请空间的区别
	pf = (struct foo *)calloc(1, sizeof(struct foo));
	pf->p = (char *)malloc(10);
	if ((!pf) || (!pf->p)) {
		fprintf(stdout, "calloc failed!\n");
		return 1;
	}
	printf("1, pf->p = %p\n", pf->p);
	memcpy(pf->p, "linux", sizeof("linux"));
	printf("pf->p = %s\n", pf->p);
	pf->p = realloc(pf->p, 
	(sizeof("linux") + sizeof("kernel") - 1));
	if (!pf->p) {
		printf("realloc failed!\n");
		free(pf->p);
		free(pf);
		exit (EXIT_FAILURE);
	}
	printf("2, pf->p = %p\n", pf->p);
	strncat(pf->p, "kernel", sizeof("kernel"));
	printf("realloc, pf->p = %s\n", pf->p);

	free(pf->p);
	pf->p = NULL; //在指针指向的堆空间被释放之后，都应该把该指针置为NULL。这样的话，才能保证不会再通过该指针去访问已经被使用掉的堆内存空间。
	printf("==free, pf->p = %p\n", pf->p);
	if (pf->p) {
		printf("ok1\n");
		strcpy(pf->p, "hello");
		printf("ok2\n");
	}
	printf("free, pf->p = %s\n", pf->p);

	//free(pf);
	//free(pf); //内存使用错误之一: 多次释放同一块内存

	//内存使用错误的种类：
	//1、多次释放同一块堆内存
	//2、只释放堆内存的一部分
	//3、释放不是自己申请的堆内存
	//4、释放堆内存之后，继续使用
	
	return 0;
}
