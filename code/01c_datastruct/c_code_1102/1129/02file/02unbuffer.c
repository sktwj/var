#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

//不带缓冲区的文件操作：遵循的GNU(posix)标准。是不可移植的。只能用在UNIX或类UNIX系统中。
//
//fd1 = 0
//fd2 = 3
//file position = 1035

int main(void)
{
	close(0); //关闭标准输入文件
	//系统在开启时，任何一个进程中，三个文件（标准输入文件，标准输出文件，标准错误输出文件）是被自动打开的，他们分别占用的文件描述符是0,1,2。可以通过/usr/include/unistd.h文件来查看。

	//对于文件的操作步骤：
	//1、打开文件
	int fd1 = open("./01buffer.c", O_RDONLY);
	int fd2 = open("./tmp.c", O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);

	if ((fd1 < 0) || (fd2 < 0)) {
		printf("open file failed!\n");
		return 1;
	}
	printf("fd1 = %d\n", fd1);
	printf("fd2 = %d\n", fd2);

	char c;
	//2、操作文件
	while (read(fd1, &c, 1)) {
		write(fd2, &c, 1);
	}

	//lseek函数用于重置文件的读取位置，其中第2个参数是想对于第3个参数的偏移量，第3个参数表示文件的某个位置，可以是SEEK_SET(表示文件起始位置),SEEK_CUR(表示文件当前位置),SEEK_END(表示文件尾的位置)。其返回值表示文件目前位置距离文件起始位置的字节数。
	printf("file position = %d\n", 
			lseek(fd1, 0, SEEK_CUR));

	//3、关闭文件
	//关闭文件的原因：对于类似服务器的程序(长年不关机)一定要在使用完文件后及时关闭。不关闭的后果可能有以下几种：1、系统对于打开文件的数量是有限制的；2、对于文件的修改可能并不能真正得以改变；3、对于文件的操作是互斥的，在一个用户操作某个文件时是对其进程加锁的，使用完毕解锁之后，其他用户才能继续使用。
	close(fd1);
	close(fd2);

	return 0;
}
