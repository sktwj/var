#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

#include <linux/platform_device.h>
#include <linux/clk.h>

#include <mach/regs-clock.h> //EXYNOS4_CLKGATE_IP_PERIR

//linux/include/clk.h中含有clk_get;
//arch/arm/plat-samsung/clock.c中包含大量时钟操作函数的实现，clk_enbale(), clk_disable(), clk_get_rate()
//clk对象的申明arch/arm/mach-exynos/clock-exynos4.c中

#define CLKGATE_IP_PERIR (*(volatile u32*)EXYNOS4_CLKGATE_IP_PERIR)

static struct clk *wdt_clk;

static int ldm_probe(struct platform_device *pdev)
{
	printk("%s %s\n", __FILE__, __FUNCTION__);
	//通过关键词"watchdog"获取clk对象，关键词是通过对clock_exynos4.c搜索而得
	wdt_clk= clk_get(&pdev->dev, "watchdog");

	//使能watchdog所在总线上的时钟开关
	clk_enable(wdt_clk);

	if (CLKGATE_IP_PERIR & (1 << 14)) {
		printk("CLKGATE_IP_PERIR[14] enable\n");
	}

	unsigned long rate = clk_get_rate(wdt_clk);
	printk("rate = %ld\n", rate);

	return 0;
}

static int ldm_remove(struct platform_device *pdev)
{
	printk("%s %s\n", __FILE__, __FUNCTION__);
	clk_disable(wdt_clk);

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