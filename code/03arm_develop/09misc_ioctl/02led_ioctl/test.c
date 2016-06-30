#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

#include <linux/miscdevice.h> //struct miscdevice
#include <linux/fs.h> //struct file_operations
#include <mach/regs-gpio.h>

#include "ldm_cmd.h"

#define GPM4CON (*(volatile u32*)(S5P_VA_GPIO2 + 0x02e0))
#define GPM4DAT (*(volatile u8*)(S5P_VA_GPIO2 + 0x02e4))

struct ldm_info {
	struct miscdevice dev;
	struct file_operations ops;
};
static struct ldm_info ldm;

static inline void led_init(void)
{
	//将GPM4_0-3这4个管脚设为output
	GPM4CON = (GPM4CON & ~0xffff) | 0x1111;
}

//4个led灯的状态，0亮灯，1灭灯
static inline void led_on(u8 led_stat)
{
	//GPM4_0-3
	GPM4DAT = (GPM4DAT & ~0xf) | (led_stat & 0xf);
}

#if 0
static ssize_t ldm_write(struct file *fp, const char __user *buf, size_t size, loff_t *offset)
{
	printk("%s: &buf=0x%x\n", __FUNCTION__, &buf);
	led_on(*buf);

	return 1;
}
#endif

static long ldm_ioctl(struct file *fp, unsigned int cmd, unsigned long data)
{
	printk("%s: cmd=0x%x\n", __FUNCTION__, cmd);

	switch (cmd) {
	case LED_ON:
		led_on(data);
		break;
	case LED_OFF:
		led_on(0xf);
		break;
	default:
		break;
	}

	return 0;
}

static int ldm_init(void)
{
	printk("%s %s\n", __FILE__, __FUNCTION__);

	int ret = 0;

	//由miscdevice驱动框架自行分配一个当前未被使用并且数字尽量靠前的次设备号。主设备固定为miscdevice框架自有的10
	ldm.dev.minor = MISC_DYNAMIC_MINOR;
	ldm.dev.name = "ldm";
	ldm.dev.fops = &ldm.ops;

	//ldm.ops.write = ldm_write;
	ldm.ops.unlocked_ioctl = ldm_ioctl;

	led_init();

	ret = misc_register(&ldm.dev);
	if (ret < 0) {
		printk(KERN_WARNING "misc_register failed\n");
		goto err_misc_register;
	}

	return 0;

err_misc_register:
	return ret;
}

static void ldm_exit(void)
{
	printk("%s %s\n", __FILE__, __FUNCTION__);

	led_on(0xf);
	misc_deregister(&ldm.dev);
}

module_init(ldm_init);
module_exit(ldm_exit);

MODULE_LICENSE("GPL");

MODULE_AUTHOR("uplooking");
MODULE_DESCRIPTION("module test");