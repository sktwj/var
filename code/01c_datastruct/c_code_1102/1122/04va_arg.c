#include <stdio.h>
#include <stdarg.h>

//...之前的参数被称为固定参数，至少一个
//...被称为可变参数
int sum(int m, int n, ...)
{
	va_list ap; //定义va_list类型的变量(底层实现为一个指针)
	va_start(ap, n); //初始化ap变量，使其指向最后一个固定参数

	int sum = 0, ret;
	while (ret = va_arg(ap, int)) { //每一次调用va_arg，可以取出一个可变参数
        printf("sum%d += ret %d\n", sum ,ret);
		sum += ret;
	}

	va_end(ap); //清理ap变量(底层实现：ap=NULL)

	return sum;
}

void foo_arg(int n, ...)
{
	va_list ap;
	va_start(ap, n);
	printf("1, %c\n", (char)va_arg(ap, int));
	printf("%s\n", va_arg(ap, char *));

	//1、多次调用可变参数列表是可能的
	//2、在访问完某一个可变参数，想跳过后面的某几个可变参数去访问后续的可变参数是不行的，因为va_arg宏是根据可变参数的类型来决定跳过的字节数的
	//3、在访问完某一个可变参数之后，不想继续访问后续可变参数是可以的
	//4、在va_arg被调用时，如果实际参数的类型和形式参数的类型不兼容时，编译器将作缺省的参数类型的提升：char,short-->int; float-->double；如果无法提升，将发生一个随机的错误
	va_start(ap, n);
	printf("2, %c\n", (char)va_arg(ap, int));
	printf("%s\n", va_arg(ap, char *));
	printf("%f\n", (float)va_arg(ap, double));
	printf("%hd\n", (short)va_arg(ap, int));
	
	va_end(ap);
}

int main(void)
{
    //sum = 15
    //1, c
    //hello world
    //2, c
    //hello world
    //3.140000
    //10
	printf("sum = %d\n", sum(3,0, 1,2,3,4,5, 0));
	
	foo_arg(6, 'c', "hello world", 3.14, 10);

	return 0;
}
