#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

#include <linux/interrupt.h> //request_irq
#include <mach/irqs.h> //中断号
//包含了拥有IRQ_EINT()的plat/irqs.h

/* android内核中的按键驱动已经注册了这四个按键的中断，所以需要把这个驱动关闭
 * make menuconfig -> Device Drivers -> Input device support -> Miscellaneous devices -> GPIO driver support 关闭
 * 重新编译内核，生成的zImage在arch/arm/boot中
 */

static irqreturn_t key_handler(int irqno, void *arg)
{
	printk("%s %s %d\n", __FILE__, __FUNCTION__, (int)arg);
	return IRQ_HANDLED;
}

static irqreturn_t key_handler_dummy(int irqno, void *arg)
{
	printk("%s %s %d\n", __FILE__, __FUNCTION__, (int)arg);
	return IRQ_HANDLED;
}

static int ldm_init(void)
{
	printk("%s %s\n", __FILE__, __FUNCTION__);

	/* @IRQ_EINT(26): 中断号，从plat/irqs.h读取，和硬件手册上的中断号不同，是由内核重新规划的
	 * @key_handler：中断处理函数
	 * @IRQF_TRIGGER_FALLING: 下降沿触发
	 * @IRQF_SHARED: 共享中断号，可以针对同一个中断号注册多个不同的中断处理函数，当该中断触发时，这些函数都会被执行
	 * @"ldm"：在/proc/interrupts中显示的中断注册信息的名字
	 * @(void*)1234: 传入回调函数的参数, 对应回调函数第二个参数，可以填NULL。但是如果使用共享中断机制的话，该参数将作为注册于同一个中断号上不同中断处理函数区分的标志，不能为NULL
	 */
	int ret = request_irq(IRQ_EINT(26), key_handler, IRQF_TRIGGER_FALLING | IRQF_SHARED, "ldm", (void*)1234);
	if (ret < 0) {
		printk(KERN_INFO "request_irq failed\n");
		goto err_request_irq;
	}

	ret = request_irq(IRQ_EINT(26), key_handler_dummy, IRQF_TRIGGER_FALLING | IRQF_SHARED, "ldm_dummy", (void*)5678);
	if (ret < 0) {
		printk(KERN_INFO "request_irq dummy failed\n");
		goto err_request_irq_dummy;
	}

	return 0;

err_request_irq_dummy:
	free_irq(IRQ_EINT(26), (void*)1234);
err_request_irq:
	return ret;
}

static void ldm_exit(void)
{
	printk("%s %s\n", __FILE__, __FUNCTION__);

	free_irq(IRQ_EINT(26), (void*)5678);
	free_irq(IRQ_EINT(26), (void*)1234);
}

module_init(ldm_init);
module_exit(ldm_exit);

MODULE_LICENSE("GPL");

MODULE_AUTHOR("uplooking");
MODULE_DESCRIPTION("module test");