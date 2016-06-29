#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

//arch/arm/mach-exynos/include/mach/regs-gpio.h
//实际所使用的宏S5P_VA_GPIO2在map-s5p.h中，S5P_VA_GPIO2表示GPIO part2的首地址
#include <mach/regs-gpio.h>

#define GPM4CON (*(volatile u32*)(S5P_VA_GPIO2 + 0x02e0))
#define GPM4DAT (*(volatile u8*)(S5P_VA_GPIO2 + 0x02e4))

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

static int ldm_init(void)
{
	printk("%s %s\n", __FILE__, __FUNCTION__);

	led_init();
	led_on(0b1001);

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