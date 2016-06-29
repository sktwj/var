#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

#include <linux/io.h> //ioremap

struct led_reg {
	u32 gpm4con;
	u8 gpm4dat;
};
static struct led_reg *reg;

static inline void led_init(void)
{
	//将GPM4_0-3这4个管脚设为output
	reg->gpm4con = (reg->gpm4con & ~0xffff) | 0x1111;
}

//4个led灯的状态，0亮灯，1灭灯
static inline void led_on(u8 led_stat)
{
	//GPM4_0-3
	reg->gpm4dat = (reg->gpm4dat & ~0xf) | (led_stat & 0xf);
}

static int ldm_init(void)
{
	printk("%s %s\n", __FILE__, __FUNCTION__);

	//将物理地址映射为虚拟地址后，才能访问。针对同一个物理地址，可以反复多次执行ioremap得到不同的虚拟地址，每个虚拟地址都可以用来访问该物理地址
	reg = ioremap(0x110002e0, 8);

	led_init();
	led_on(0b1010);

	return 0;
}

static void ldm_exit(void)
{
	printk("%s %s\n", __FILE__, __FUNCTION__);
	//解除映射
	iounmap(reg);
}

module_init(ldm_init);
module_exit(ldm_exit);

MODULE_LICENSE("GPL");

MODULE_AUTHOR("uplooking");
MODULE_DESCRIPTION("module test");