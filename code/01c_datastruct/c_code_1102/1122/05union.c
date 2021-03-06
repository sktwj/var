#include <stdio.h>

//联合体（共用体）：不同或相同类型的成员占用同一块内存空间的结构
//1、联合体变量的大小取决于其中最大成员的大小
//2、联合体变量在赋值时，是按照第一个成员的类型来接收的
//3、在同一时刻，只能将该内存当作其中一种类型来使用
//4、该内存空间的值是最后一次通过某个成员所改变的值

//联合体类型
union u_foo {
	int a;
	short s;
	char c;
} u = {0x11223344};

union u_endian {
	int a;
	short s[2];
};
void __swap(union u_endian *pu)
{
	short tmp = pu->s[0];
	pu->s[0] = pu->s[1];
	pu->s[1] = tmp;
}


union hehe{
    int a;
    short s;
    char c;

}

int main(void)
{
//swap, ue = 0x33441122
//size union = 4
//u.a = 0x11223344
//u.s = 0x3344
//u.c = 0x44
//u = 0x11223377
	union u_endian ue = {0x11223344};
	__swap(&ue);
	printf("swap, ue = %#x\n", ue);

	printf("size union = %d\n", sizeof(union u_foo));
	printf("u.a = %#x %p\n", u.a, &u.a);
	printf("u.s = %#x %p\n ", u.s, &u.s);
	printf("u.c = %#x %p %#x\n", u.c, &u.c , *((char *)(&u.c+2)));

	u.c = 0x77;
	printf("u = %#x\n", u);

	return 0;
}
