#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

#include <mach/regs-clock.h> //EXYNOS4_CLKGATE_IP_PERIR
#include <mach/map.h> //EXYNOS4_PA_WATCHDOG
#include <linux/io.h> //ioremap
#include <linux/interrupt.h> //request_irq
#include <mach/irqs.h> //中断号
//包含了拥有IRQ_EINT()的plat/irqs.h

#include <linux/miscdevice.h> //struct miscdevice
#include <linux/fs.h> //struct file_operations

#include "ldm_cmd.h"

/* 关闭内核中已经存在的watchdog驱动，否则无法注册到中断号
 * make menuconfig -> Device Drivers -> Watchdog Timer Support -> S3C2410 Watchdog 关闭
 * 重新编译内核
 */

#define CLKGATE_IP_PERIR (*(volatile u32*)EXYNOS4_CLKGATE_IP_PERIR)

struct wdt_reg {
	u32 con, dat, cnt, clrint;
};

struct ldm_info {
	struct miscdevice dev;
	struct file_operations ops;
	struct wdt_reg *reg;
};
static struct ldm_info ldm;

enum {
	PCLK = 100000000, //100MHZ,见4412手册P446
	PRESCALER = 99, //实际一级分频值为100
	CLK_SEL = 3, //二级分频值是(16 << CLK_SEL)
	//产生一秒计时周期所需的计数值
	WDT_HZ = PCLK / (PRESCALER+1) / (16 << CLK_SEL),
};

static irqreturn_t wdt_handler(int irqno, void *arg)
{
	printk("%s\n", __FUNCTION__);
	ldm.reg->clrint = 0;
	return IRQ_HANDLED;
}

static long ldm_ioctl(struct file *fp, unsigned int cmd, unsigned long data)
{
	printk("%s: cmd=0x%x\n", __FUNCTION__, cmd);

	switch (cmd) {
	case WDT_ON:
		ldm.reg->con |= 1 << 5;
		break;
	case WDT_OFF:
		ldm.reg->con &= ~(1 << 5);
		break;
	case WDT_CFG_INT:
		ldm.reg->con &= ~1; //关reset功能
		ldm.reg->con |= 1 << 2; //开中断功能
		break;
	case WDT_CFG_RESET:
		ldm.reg->con &= ~(1 << 2); //关中断功能
		ldm.reg->con |= 1; //开reset
		break;
	case WDT_SET_CYCLE:
		ldm.reg->cnt = WDT_HZ * data;
		ldm.reg->dat = WDT_HZ * data;
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

	//打开watchdog在总线peri-R上的时钟使能开关
	CLKGATE_IP_PERIR |= 1 << 14;

	//映射wdt的寄存器
	ldm.reg = ioremap(EXYNOS4_PA_WATCHDOG, sizeof(struct wdt_reg));

	//初始化wdt
	/* WTCON[0]: 0 关闭reset， 1 打开reset功能
	 * [2]: 0 关闭中断功能， 1 打开中断
	 * [4:3]: 二级分频值的选择,即CLK_SEL
	 * [5]: wdt总开关
	 * [15:8]: 一级分频值PRESCALER
	 */
	ldm.reg->con = 1 << 2 | CLK_SEL << 3 | PRESCALER << 8;

	ldm.reg->cnt = WDT_HZ;
	ldm.reg->dat = WDT_HZ;

	//注册中断
	ret = request_irq(IRQ_WDT, wdt_handler, IRQF_TRIGGER_NONE, "ldm", NULL);
	if (ret < 0) {
		printk(KERN_INFO "request_irq failed\n");
		goto err_request_irq;
	}

	//由miscdevice驱动框架自行分配一个当前未被使用并且数字尽量靠前的次设备号。主设备固定为miscdevice框架自有的10
	ldm.dev.minor = MISC_DYNAMIC_MINOR;
	ldm.dev.name = "ldm";
	ldm.dev.fops = &ldm.ops;

	ldm.ops.unlocked_ioctl = ldm_ioctl;

	ret = misc_register(&ldm.dev);
	if (ret < 0) {
		printk(KERN_WARNING "misc_register failed\n");
		goto err_misc_register;
	}

	return 0;

err_misc_register:
	ldm.reg->con &= ~(1 << 5);
	free_irq(IRQ_WDT, NULL);
err_request_irq:
	iounmap(ldm.reg);
	CLKGATE_IP_PERIR &= ~(1 << 14);
	return ret;
}

static void ldm_exit(void)
{
	printk("%s %s\n", __FILE__, __FUNCTION__);

	misc_deregister(&ldm.dev);
	ldm.reg->con &= ~(1 << 5);
	free_irq(IRQ_WDT, NULL);
	iounmap(ldm.reg);
	CLKGATE_IP_PERIR &= ~(1 << 14);
}

module_init(ldm_init);
module_exit(ldm_exit);

MODULE_LICENSE("GPL");

MODULE_AUTHOR("uplooking");
MODULE_DESCRIPTION("module test");