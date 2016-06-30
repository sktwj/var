#include <linux/init.h> //module_init __init
#include <linux/kernel.h> //printk.h container_of
#include <linux/module.h> //MODULE_LICENSE

//测试方法
//insmod ldm.ko 动态加载模块
//lsmod 查看当前系统已经加载的所有模块
//rmmod ldm 卸载模块，注意参数是模块名而不是文件名，所以不要加扩展名

static int ldm_init(void)
{
	printk("%s %s\n", __FILE__, __FUNCTION__);
	return 0;
}

static void ldm_exit(void)
{
	printk("%s %s\n", __FILE__, __FUNCTION__);
}

//以下两个宏有两种实现
//以模块方式加载时，系统自动定义MODULE宏，本宏实现方式是直接执行目标函数并继续执行init_module或cleanup_module
//以zImage的一部分执行时，分别创建两个全局函数指针指向两个函数，并且函数指针被链接到指定的initcall段中，在内核初始化过程中自动执行
module_init(ldm_init);
module_exit(ldm_exit);

//必须指定发布协议，否则内核中某些函数无法调用
MODULE_LICENSE("GPL");

//可选项
MODULE_AUTHOR("uplooking");
MODULE_DESCRIPTION("module test");