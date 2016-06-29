//#include "../lib_so/head/head.h"

//在使用自己制作的库函数时，可以在该文件内直接包含库文件中的函数所在的头文件；也可以不在该文件内包含，而是在终端在命令行中用-I -L -l指定需要链接的库文件

int main(void)
{
	luck("boys and girls");

	return 0;
}
