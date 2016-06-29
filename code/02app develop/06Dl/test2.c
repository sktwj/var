#include <stdio.h>

int add(int a,int b)
{
	return a+b;
}

int main(void)
{
	int (*call)(int,int);

	call=&add;
	printf("%d\n",(*call)(1,3));
	printf("%d\n",call(1,3));



}
