#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

/* 测试驱动
 * exec 0</dev/tty1
 * 将当前终端的标准输入重定向到本驱动提供的终端设备结点上
 */

static key_handler()
{
	input_report_key();
	input_sync();
}

static int ldm_init(void)
{
	printk("%s %s\n", __FILE__, __FUNCTION__);

	//1 创建input_dev主对象
	input_allocate_device();

	//2 填充input_dev成员
	set_bit(evbit, keybit);

	//3 注册input_dev对象，在/proc/bus/input/devices中可以查看注册信息
	input_register_device();

	//4 初始化硬件，注册中断
	request_irq();

	return 0;
}

static void ldm_exit(void)
{
	printk("%s %s\n", __FILE__, __FUNCTION__);
}

module_init(ldm_init);
module_exit(ldm_exit);

MODULE_LICENSE("GPL");

MODULE_AUTHOR("uplooking");
MODULE_DESCRIPTION("module test");