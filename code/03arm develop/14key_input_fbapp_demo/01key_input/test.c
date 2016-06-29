#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

#include <linux/input.h>
#include <linux/bitops.h> //set_bit tools/perf/util/include/linux/bitops.h
#include <mach/irqs.h>
#include <linux/interrupt.h>
#include <mach/regs-gpio.h>

/* 测试驱动
 * exec 0</dev/tty1
 * 将当前终端的标准输入重定向到本驱动提供的终端设备结点上
 */

//输入设备注册成功后将会在/proc/bus/input/devices中可见对应信息

#define GPX3DAT (*(volatile u8*)(S5P_VA_GPIO2 + 0x0c64))

#define DEVNAME "LDM Key Input"

struct key_info {
	int code; //键值
	int irqno;
	const char *name;
};
static struct key_info ldm_key_info[] = {
	{KEY_L, IRQ_EINT(26), "KEY_L"},
	{KEY_S, IRQ_EINT(27), "KEY_S"},
	{KEY_ENTER, IRQ_EINT(28), "KEY_ENTER"},
	{KEY_BACKSPACE, IRQ_EINT(29), "KEY_BACKSPACE"},
};

struct ldm_info {
	struct input_dev *dev;
	struct key_info *key;
};
static struct ldm_info ldm;

static irqreturn_t key_handler(int irqno, void *data)
{
	struct key_info *key = (struct key_info*)data;

	int key_stat = GPX3DAT & (1 << (key - ldm.key + 2));

	//printk("%s:%s, key_stat=%d\n", __FUNCTION__, key->name, key_stat);

	if (key_stat) { //抬起
		input_report_key(ldm.dev, key->code, 0);
	} else { //按下
		input_report_key(ldm.dev, key->code, 1);
	}
	//哨兵事件
	input_sync(ldm.dev);

	return IRQ_HANDLED;
}

static int ldm_init(void)
{
	printk("%s %s\n", __FILE__, __FUNCTION__);

	int ret = 0;
	ssize_t i = 0;

	//1 创建input_dev主对象
	ldm.dev = input_allocate_device();
	if (!ldm.dev) {
		printk(KERN_ERR "input_allocate_device failed\n");
		ret = -ENOMEM;
		goto err_input_allocate_device;
	}

	//2 填充input_dev成员
	//作为/proc/bus/input/devices中的设备注册信息中的标识
	ldm.dev->name = DEVNAME;
	ldm.key = ldm_key_info;

	//注册输入设备允许发到应用层设备结点上的event数据包类型
	set_bit(EV_KEY, ldm.dev->evbit); //按键类型
	set_bit(EV_REP, ldm.dev->evbit); //连发

	for (i = 0; i < ARRAY_SIZE(ldm_key_info); ++i) {
		set_bit(ldm.key[i].code, ldm.dev->keybit);
	}

	//3 注册input_dev对象
	ret = input_register_device(ldm.dev);
	if (ret < 0) {
		printk(KERN_ERR "input_register_device failed\n");
		goto err_input_register_device;
	}

	//4 初始化硬件，注册中断
	for (i = 0; i < ARRAY_SIZE(ldm_key_info); ++i) {
		ret = request_irq(ldm.key[i].irqno, key_handler, IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING, ldm.key[i].name, ldm.key + i);
		if (ret < 0) {
			printk(KERN_ERR "request_irq failed\n");
			goto err_request_irq;
		}
	}

	return 0;

err_request_irq:
	for (i = i - 1; i >= 0; --i) {
		free_irq(ldm.key[i].irqno, ldm.key + i);
	}
	input_unregister_device(ldm.dev);
err_input_register_device:
	input_free_device(ldm.dev);
err_input_allocate_device:
	return ret;
}

static void ldm_exit(void)
{
	printk("%s %s\n", __FILE__, __FUNCTION__);

	ssize_t i = 0;
	for (i = ARRAY_SIZE(ldm_key_info) - 1; i >= 0; --i) {
		free_irq(ldm.key[i].irqno, ldm.key + i);
	}
	input_unregister_device(ldm.dev);
	input_free_device(ldm.dev);
}

module_init(ldm_init);
module_exit(ldm_exit);

MODULE_LICENSE("GPL");

MODULE_AUTHOR("uplooking");
MODULE_DESCRIPTION("module test");