#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

#include <mach/regs-clock.h> //EXYNOS4_CLKGATE_IP_PERIR
#include <mach/map.h> //EXYNOS4_PA_WATCHDOG
#include <linux/io.h> //ioremap
#include <linux/interrupt.h> //request_irq
#include <mach/irqs.h> //中断号
//包含了拥有IRQ_EINT()的plat/irqs.h

/* 关闭内核中已经存在的watchdog驱动，否则无法注册到中断号
 * make menuconfig -> Device Drivers -> Watchdog Timer Support -> S3C2410 Watchdog 关闭
 * 重新编译内核
 */

#define CLKGATE_IP_PERIR (*(volatile u32*)EXYNOS4_CLKGATE_IP_PERIR)

struct wdt_reg {
	u32 con, dat, cnt, clrint;
};
static struct wdt_reg *reg;

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
	reg->clrint = 0;
	return IRQ_HANDLED;
}

static int ldm_init(void)
{
	printk("%s %s\n", __FILE__, __FUNCTION__);

	//打开watchdog在总线peri-R上的时钟使能开关
	CLKGATE_IP_PERIR |= 1 << 14;

	//映射wdt的寄存器
	reg = ioremap(EXYNOS4_PA_WATCHDOG, sizeof(struct wdt_reg));

	//初始化wdt
	/* WTCON[0]: 0 关闭reset， 1 打开reset功能
	 * [2]: 0 关闭中断功能， 1 打开中断
	 * [4:3]: 二级分频值的选择,即CLK_SEL
	 * [5]: wdt总开关
	 * [15:8]: 一级分频值PRESCALER
	 */
	reg->con = 1 << 2 | CLK_SEL << 3 | PRESCALER << 8;

	reg->cnt = WDT_HZ;
	reg->dat = WDT_HZ;

	//注册中断
	int ret = request_irq(IRQ_WDT, wdt_handler, IRQF_TRIGGER_NONE, "ldm", NULL);
	if (ret < 0) {
		printk(KERN_INFO "request_irq failed\n");
		goto err_request_irq;
	}

	//打开watchdog
	reg->con |= 1 << 5;

	return 0;

err_request_irq:
	return ret;
}

static void ldm_exit(void)
{
	printk("%s %s\n", __FILE__, __FUNCTION__);

	reg->con &= ~(1 << 5);
	free_irq(IRQ_WDT, NULL);
	iounmap(reg);
	CLKGATE_IP_PERIR &= ~(1 << 14);
}

module_init(ldm_init);
module_exit(ldm_exit);

MODULE_LICENSE("GPL");

MODULE_AUTHOR("uplooking");
MODULE_DESCRIPTION("module test");