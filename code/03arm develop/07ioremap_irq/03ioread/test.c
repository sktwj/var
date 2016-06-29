#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

#include <linux/io.h> //ioremap ioread iowrite，实际在arch/arm/include/asm/io.h

static u32 *reg;

/* readb readw readl writeb writew writel已被read8 read16 read32 write8 write16 write32替代 */

static inline void led_init(void)
{
	//将GPM4_0-3这4个管脚设为output
	//reg->gpm4con = (reg->gpm4con & ~0xffff) | 0x1111;

	u32 gpm4con = ioread32(reg);
	gpm4con = (gpm4con & ~0xffff) | 0x1111;
	iowrite32(gpm4con, reg);
}

//4个led灯的状态，0亮灯，1灭灯
static inline void led_on(u8 led_stat)
{
	//GPM4_0-3
	//reg->gpm4dat = (reg->gpm4dat & ~0xf) | (led_stat & 0xf);

	u8 gpm4dat = ioread8(reg + 1);
	gpm4dat = (gpm4dat & ~0xf) | (led_stat & 0xf);
	iowrite8(gpm4dat, reg + 1);
}

static int ldm_init(void)
{
	printk("%s %s\n", __FILE__, __FUNCTION__);

	//将物理地址映射为虚拟地址后，才能访问。针对同一个物理地址，可以反复多次执行ioremap得到不同的虚拟地址，每个虚拟地址都可以用来访问该物理地址
	reg = ioremap(0x110002e0, 8);

	led_init();
	led_on(0b0110);

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