#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

#include <linux/miscdevice.h> //struct miscdevice
#include <linux/fs.h> //struct file_operations
#include <mach/regs-gpio.h>

#include <linux/interrupt.h> //request_irq
#include <mach/irqs.h> //中断号
//包含了拥有IRQ_EINT()的plat/irqs.h

//========================================
#include <linux/wait.h> //等待队列
#include <linux/sched.h>
//========================================

struct ldm_info {
	struct miscdevice dev;
	struct file_operations ops;
	//========================================
	wait_queue_head_t wait;
	int wait_condition;
	//========================================
};
static struct ldm_info ldm;

static ssize_t ldm_read(struct file *fp, char __user *buf, size_t size, loff_t *offset)
{
	printk("%s, %s,pid=%d\n", __FUNCTION__, current->comm, current->pid);

	//========================================
	//挂起本进程
	ldm.wait_condition = 0;
	//当wait_condition为真时，才会被wake_up_interruptibel唤醒
	wait_event_interruptible(ldm.wait, ldm.wait_condition);
	//========================================

	return 0;
}

static irqreturn_t key_handler(int irqno, void *arg)
{
	printk("%s %s %d\n", __FILE__, __FUNCTION__, (int)arg);

	//========================================
	ldm.wait_condition = 1;
	wake_up_interruptible(&ldm.wait);
	//========================================

	return IRQ_HANDLED;
}

static int ldm_init(void)
{
	printk("%s %s\n", __FILE__, __FUNCTION__);

	int ret = 0;

	//由miscdevice驱动框架自行分配一个当前未被使用并且数字尽量靠前的次设备号。主设备固定为miscdevice框架自有的10
	ldm.dev.minor = MISC_DYNAMIC_MINOR;
	ldm.dev.name = "ldm";
	ldm.dev.fops = &ldm.ops;

	ldm.ops.read = ldm_read;

	ret = request_irq(IRQ_EINT(26), key_handler, IRQF_TRIGGER_FALLING, "ldm", NULL);
	if (ret < 0) {
		printk(KERN_INFO "request_irq failed\n");
		goto err_request_irq;
	}

	ret = misc_register(&ldm.dev);
	if (ret < 0) {
		printk(KERN_WARNING "misc_register failed\n");
		goto err_misc_register;
	}

	//========================================
	init_waitqueue_head(&ldm.wait);
	//========================================

	return 0;

err_misc_register:
	free_irq(IRQ_EINT(26), NULL);
err_request_irq:
	return ret;
}

static void ldm_exit(void)
{
	printk("%s %s\n", __FILE__, __FUNCTION__);

	misc_deregister(&ldm.dev);
	free_irq(IRQ_EINT(26), NULL);
}

module_init(ldm_init);
module_exit(ldm_exit);

MODULE_LICENSE("GPL");

MODULE_AUTHOR("uplooking");
MODULE_DESCRIPTION("module test");