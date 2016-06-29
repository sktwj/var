#include "config.h"

static int global_data = 0; //全局变量，初值为0，存放在bss段

int main()
{
	static int static_local_data; //静态局部变量，无初值，存放在bss段


	printf("hello world %s 0x%x %c %d %d\n", "linux", 0x1234, 'Q', global_data, static_local_data);
	//printf("hello world %s 0x%x %c\n", "linux", 0x1234, 'Q');

	return 0;
}
