#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

#include <linux/io.h> //ioremap
#include <linux/interrupt.h> //request_irq

#include <linux/miscdevice.h> //struct miscdevice
#include <linux/fs.h> //struct file_operations

#include <linux/platform_device.h>
#include <linux/clk.h>

#include "ldm_cmd.h"

/* 关闭内核中已经存在的watchdog驱动，否则无法注册到中断号
 * make menuconfig -> Device Drivers -> Watchdog Timer Support -> S3C2410 Watchdog 关闭
 * 重新编译内核
 */

//linux/include/clk.h中含有clk_get;
//arch/arm/plat-samsung/clock.c中包含大量时钟操作函数的实现，clk_enbale(), clk_disable(), clk_get_rate()
//clk对象的申明arch/arm/mach-exynos/clock-exynos4.c中

struct wdt_reg {
	u32 con, dat, cnt, clrint;
};

struct ldm_info {
	struct miscdevice dev;
	struct file_operations ops;
	struct wdt_reg *reg;
	int irqno;
	struct clk *clk;
	unsigned long rate; //watchdog所在总线的频率
};
static struct ldm_info ldm;

enum {
	//PCLK = 100000000, //100MHZ,见4412手册P446
	PRESCALER = 99, //实际一级分频值为100
	CLK_SEL = 3, //二级分频值是(16 << CLK_SEL)
	//产生一秒计时周期所需的计数值
};
#define WDT_HZ (ldm.rate / (PRESCALER+1) / (16 << CLK_SEL))

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

static int ldm_probe(struct platform_device *pdev)
{
	printk("%s %s\n", __FILE__, __FUNCTION__);

	int ret = 0;

	//通过关键词"watchdog"获取clk对象，关键词是通过对clock_exynos4.c搜索而得
	ldm.clk= clk_get(&pdev->dev, "watchdog");
	if (IS_ERR(ldm.clk)) {
		printk(KERN_ERR "clk_get watchdog clock failed\n");
		ret = PTR_ERR(ldm.clk);
		goto err_clk_get;
	}

	//打开watchdog在总线peri-R上的时钟使能开关
	ret = clk_enable(ldm.clk);
	if (ret < 0) {
		printk(KERN_ERR "clk_enable watchdog clock failed\n");
		goto err_clk_enable;
	}

	ldm.rate = clk_get_rate(ldm.clk);
	printk("watchdog PCLK rate = %ld\n", ldm.rate);

	//从pdev的resource中获取寄存器物理首地址
	struct resource *res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		printk(KERN_ERR "platform_get_resource mem failed\n");
		ret = -ENXIO;
		goto err_platform_get_resource_mem;
	}

	//映射wdt的寄存器
	ldm.reg = ioremap(res->start, sizeof(struct wdt_reg));

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

	//从pdev的resource中获取中断号
	ldm.irqno = platform_get_irq(pdev, 0);
	if (ldm.irqno < 0) {
		ret = ldm.irqno;
		printk(KERN_ERR "platform_get_irq failed\n");
		goto err_platform_get_irq;
	}

	//注册中断
	ret = request_irq(ldm.irqno, wdt_handler, IRQF_TRIGGER_NONE, "ldm", NULL);
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
	free_irq(ldm.irqno, NULL);
err_request_irq:
err_platform_get_irq:
	iounmap(ldm.reg);
err_platform_get_resource_mem:
	clk_disable(ldm.clk);
err_clk_enable:
err_clk_get:
	return ret;
}

static int ldm_remove(struct platform_device *pdev)
{
	printk("%s %s\n", __FILE__, __FUNCTION__);

	misc_deregister(&ldm.dev);
	ldm.reg->con &= ~(1 << 5);
	free_irq(ldm.irqno, NULL);
	iounmap(ldm.reg);
	clk_disable(ldm.clk);

	return 0;
}

//根据platform_bus_type.match成员函数匹配算法，id_table指向的数组对象最后必须以哨兵结尾
static struct platform_device_id ldm_id_table[] = {
	{"hello"}, {"world"}, {"aaaaaa"}, {"s3c2410-wdt"}, {""},
};

static struct platform_driver ldm_drv = {
	.driver = {
		//使用id_table作为匹配关键词时，本成员仍然不能为空，否则依然会段错误
		.name = "aaa",
	},
	.probe = ldm_probe, //匹配成功时自动执行
	.remove = ldm_remove, //pdev和pdrv中其中之一被卸载，即匹配关系不再成立时，自动执行
	//当id_table对象不为NULL时，将使用id_table作为匹配方式
	.id_table = ldm_id_table,
};

static int ldm_init(void)
{
	printk("%s %s\n", __FILE__, __FUNCTION__);

	return platform_driver_register(&ldm_drv);
}

static void ldm_exit(void)
{
	printk("%s %s\n", __FILE__, __FUNCTION__);

	platform_driver_unregister(&ldm_drv);
}

module_init(ldm_init);
module_exit(ldm_exit);

MODULE_LICENSE("GPL");

MODULE_AUTHOR("uplooking");
MODULE_DESCRIPTION("module test");