#include <stdio.h>
//#include <stddef.h>

// 该宏用于计算type类型的结构体中的名字为member的成员距离它所在的结构体的首地址的偏移量
// @type: member成员所在的结构体的类型
// @member: type类型的结构体中的某个成员
#define offsetof(type, member) \
	( (size_t)(&((type *)0)->member) )

// 该宏用于得到ptr指向的member成员所在的结构体的首地址
// @ptr: member成员的首地址（或者说是指向member成员的指针）
#define container_of(ptr, type, member) \
	( (type *)((size_t)(ptr) - offsetof(type, member)) )

struct foo {
	int a;
	char c;
	char *p;
} f = {.p = "hello"};

int main(void)
{
	printf("offset p = %d\n", offsetof(struct foo, p));

	printf("f.p = %s\n", container_of(&f.a, struct foo, a)->p);

	return 0;
}
