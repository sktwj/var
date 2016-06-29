#include <stdio.h>

#include <linux/fb.h> // /usr/include/linux/fb.h

/* 测试：
 * 如果在pc机上测试，需要通过ctrl+f1~f6切换一个真实终端，同时可以用ctrl+ f7切会桌面
 */

int main()
{
	//1 打开/dev/fb0

	//2 用ioctl获取var和fix两个对象，并重新计算出像素分辨率

	//3 用mmap将驱动中的显存映射到当前进程空间

	//4 画图

	return 0;
}