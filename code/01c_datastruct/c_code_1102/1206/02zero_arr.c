#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct foo {
	int a;
	char *p;
	char par[]; //零长数组：只能作为结构体的最后一个成员存在；它仅仅标识它之前的那个成员的结束地址，不占据整体结构体变量的大小
};

struct foo2 {
	char par[0]; //零长度的数组：作为结构体的唯一成员存在时，在C中，0是不能省略的
};

int main(void)
{
	printf("size (struct foo) = %d\n", sizeof(struct foo));
	printf("size (struct foo2) = %d\n", sizeof(struct foo2));

	struct foo *pf = (struct foo *)malloc(sizeof(struct foo) + 20);
	if (!pf) {
		printf("malloc failed!\n");
		return 1;
	}
	memcpy(pf->par, "hello world", sizeof("hello world"));

	printf("pf->par = %s\n", pf->par);

	return 0;
}
