#include <stdio.h>

void foo1(int *p)
{
	p = NULL;
}

void foo2(int **p)
{
	*p = NULL;
}

int main(void)
{
	int a, *p = &a;
	printf("1, p = %p\n", p);
	//foo1(p);
	foo2(&p);
	printf("2, p = %p\n", p);

#if 0
	int a = 5;
	if (0 < a < 2) { //<==> (0 < a) < 2
		printf("ok\n");
	} else {
		printf("no\n");
	}
#endif
	return 0;
}
