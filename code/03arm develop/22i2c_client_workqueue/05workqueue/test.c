#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include <mach/irqs.h> //包含了plat/irqs.h，该头文件包含了内核中断号的定义宏
#include <linux/interrupt.h> //request_irq

#include <linux/workqueue.h>
#include <linux/sched.h>

static struct work_struct work;

//本函数将在标准进程上下文中执行，而非中断上下文
static void ldm_work_func(struct work_struct *pwork)
{
	printk(KERN_ALERT "%s\n", __FUNCTION__);

	schedule();
}

static irqreturn_t ldm_handler(int irqno, void *data)
{
	printk("irqno = %d, data = %d\n", irqno, (u32)data);

	//退出中断模式返回到SVC后，立即执行work对象中绑定的func成员函数
	schedule_work(&work);

	return IRQ_HANDLED;
}

static int __init ldm_init(void)
{
	printk(KERN_ALERT "%s\n", __FUNCTION__);

	//申请中断
	/* request_irq
	 * @irq: 中断号，从两个irqs.h中获取描述中断号的宏来填充该参数，不允许直接填入硬件手册上的中断号
	 * @handler：中断处理函数，当中断触发时将自动执行
	 * @flags：外部中断触发方式
	 * @name: 注册中断的名字，可任取，中断注册成功后可从/proc/interrupts中看到
	 * @dev: 传递给回调函数handler的参数，可以从handler函数的第二个参数获取
	 * @return: 错误码
	 */
	int ret = request_irq(IRQ_EINT(26), ldm_handler, IRQF_TRIGGER_FALLING, "ldm", (void*)100);
	if (ret < 0) {
		printk(KERN_ERR "request_irq failed\n");
		goto err_request_irq;
	}

	printk("%s: irqno = %d\n", __FUNCTION__, IRQ_EINT(26));

	//初始化工作队列的对象
	INIT_WORK(&work, ldm_work_func);

	return 0;

err_request_irq:
	return ret;
}

static void __exit ldm_exit(void)
{
	printk(KERN_ALERT "%s\n", __FUNCTION__);

	//释放申请的中断
	free_irq(IRQ_EINT(26), (void*)100);
}

module_init(ldm_init);
module_exit(ldm_exit);

MODULE_LICENSE("GPL");