#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/fb.h>
#include <linux/dma-mapping.h> //dma_alloc_writecombine
#include <mach/regs-clock.h> //EXYNOS4_CLKGATE_IP_LCD0
#include <linux/io.h> //ioremap

//加载驱动前必须先去除内核中自带的framebuffer驱动
//make menuconfig -> Device Drivers -> Graphics support -> Samsung S3C framebuffer support，按空格去掉选项前面的*
//去掉该选项后，drivers/video/Kconfig中，对应选项关闭后会造成FB_S3C_NR_BUFFERS也被关闭，而该宏在内核板级支持文件mach-tiny4412.c中被使用，该宏的消失将造成该板级文件无法编译，所以需要想办法保持该宏的存在

#define CLKGATE_IP_LCD0 (*(volatile u32*)EXYNOS4_CLKGATE_IP_LCD0)

#define CLK_SRC_LCD0 (*(volatile u32*)EXYNOS4_CLKSRC_LCD0)
#define CLK_DIV_LCD (*(volatile u32*)EXYNOS4_CLKDIV_LCD0)
#define LCDBLK_CFG (*(volatile u32*)(ldm->lcdblk_reg))

#define GPF0CON (*(volatile u32*)(S5P_VA_GPIO + 0x0180))
#define GPF1CON (*(volatile u32*)(S5P_VA_GPIO + 0x01a0))
#define GPF2CON (*(volatile u32*)(S5P_VA_GPIO + 0x01c0))
#define GPF3CON (*(volatile u32*)(S5P_VA_GPIO + 0x01e0))

#define LCD_BASE (ldm->reg)
#define VIDCON0 	(*(volatile u32 *)(LCD_BASE + 0x0000))
#define VIDCON1 	(*(volatile u32 *)(LCD_BASE + 0x0004))
#define VIDCON2 	(*(volatile u32 *)(LCD_BASE + 0x0008))
#define VIDCON3 	(*(volatile u32 *)(LCD_BASE + 0x000C))
#define VIDTCON0 	(*(volatile u32 *)(LCD_BASE + 0x0010))
#define VIDTCON1 	(*(volatile u32 *)(LCD_BASE + 0x0014))
#define VIDTCON2 	(*(volatile u32 *)(LCD_BASE + 0x0018))
#define VIDTCON3 	(*(volatile u32 *)(LCD_BASE + 0x001C))
#define WINCON0 	(*(volatile u32 *)(LCD_BASE + 0x0020))
#define WINCON1 	(*(volatile u32 *)(LCD_BASE + 0x0024))
#define WINCON2 	(*(volatile u32 *)(LCD_BASE + 0x0028))
#define WINCON3 	(*(volatile u32 *)(LCD_BASE + 0x002C))
#define WINCON4 	(*(volatile u32 *)(LCD_BASE + 0x0030))
#define SHADOWCON 	(*(volatile u32 *)(LCD_BASE + 0x0034))
#define WINCHMAP2 	(*(volatile u32 *)(LCD_BASE + 0x003C))
#define VIDOSD0A 	(*(volatile u32 *)(LCD_BASE + 0x0040))
#define VIDOSD0B 	(*(volatile u32 *)(LCD_BASE + 0x0044))
#define VIDOSD0C 	(*(volatile u32 *)(LCD_BASE + 0x0048))
#define VIDOSD1A 	(*(volatile u32 *)(LCD_BASE + 0x0050))
#define VIDOSD1B 	(*(volatile u32 *)(LCD_BASE + 0x0054))
#define VIDOSD1C 	(*(volatile u32 *)(LCD_BASE + 0x0058))
#define VIDOSD1D 	(*(volatile u32 *)(LCD_BASE + 0x005C))
#define VIDOSD2A 	(*(volatile u32 *)(LCD_BASE + 0x0060))
#define VIDOSD2B 	(*(volatile u32 *)(LCD_BASE + 0x0064))
#define VIDOSD2C 	(*(volatile u32 *)(LCD_BASE + 0x0068))
#define VIDOSD2D 	(*(volatile u32 *)(LCD_BASE + 0x006C))
#define VIDOSD3A 	(*(volatile u32 *)(LCD_BASE + 0x0070))
#define VIDOSD3B 	(*(volatile u32 *)(LCD_BASE + 0x0074))
#define VIDOSD3C 	(*(volatile u32 *)(LCD_BASE + 0x0078))
#define VIDOSD4A 	(*(volatile u32 *)(LCD_BASE + 0x0080))
#define VIDOSD4B 	(*(volatile u32 *)(LCD_BASE + 0x0084))
#define VIDOSD4C 	(*(volatile u32 *)(LCD_BASE + 0x0088))
#define VIDW00ADD0B0 	(*(volatile u32 *)(LCD_BASE + 0x00A0))
#define VIDW00ADD0B1 	(*(volatile u32 *)(LCD_BASE + 0x00A4))
#define VIDW00ADD0B2 	(*(volatile u32 *)(LCD_BASE + 0x20A0))
#define VIDW01ADD0B0 	(*(volatile u32 *)(LCD_BASE + 0x00A8))
#define VIDW01ADD0B1 	(*(volatile u32 *)(LCD_BASE + 0x00AC))
#define VIDW01ADD0B2 	(*(volatile u32 *)(LCD_BASE + 0x20A8))
#define VIDW02ADD0B0 	(*(volatile u32 *)(LCD_BASE + 0x00B0))
#define VIDW02ADD0B1 	(*(volatile u32 *)(LCD_BASE + 0x00B4))
#define VIDW02ADD0B2 	(*(volatile u32 *)(LCD_BASE + 0x20B0))
#define VIDW03ADD0B0 	(*(volatile u32 *)(LCD_BASE + 0x00B8))
#define VIDW03ADD0B1 	(*(volatile u32 *)(LCD_BASE + 0x00BC))
#define VIDW03ADD0B2 	(*(volatile u32 *)(LCD_BASE + 0x20B8))
#define VIDW04ADD0B0 	(*(volatile u32 *)(LCD_BASE + 0x00C0))
#define VIDW04ADD0B1 	(*(volatile u32 *)(LCD_BASE + 0x00C4))
#define VIDW04ADD0B2 	(*(volatile u32 *)(LCD_BASE + 0x20C0))
#define VIDW00ADD1B0 	(*(volatile u32 *)(LCD_BASE + 0x00D0))
#define VIDW00ADD1B1 	(*(volatile u32 *)(LCD_BASE + 0x00D4))
#define VIDW00ADD1B2 	(*(volatile u32 *)(LCD_BASE + 0x20D0))
#define VIDW01ADD1B0 	(*(volatile u32 *)(LCD_BASE + 0x00D8))
#define VIDW01ADD1B1 	(*(volatile u32 *)(LCD_BASE + 0x00DC))
#define VIDW01ADD1B2 	(*(volatile u32 *)(LCD_BASE + 0x20D8))
#define VIDW02ADD1B0 	(*(volatile u32 *)(LCD_BASE + 0x00E0))
#define VIDW02ADD1B1 	(*(volatile u32 *)(LCD_BASE + 0x00E4))
#define VIDW02ADD1B2 	(*(volatile u32 *)(LCD_BASE + 0x20E0))
#define VIDW03ADD1B0 	(*(volatile u32 *)(LCD_BASE + 0x00E8))
#define VIDW03ADD1B1 	(*(volatile u32 *)(LCD_BASE + 0x00EC))
#define VIDW03ADD1B2 	(*(volatile u32 *)(LCD_BASE + 0x20E8))
#define VIDW04ADD1B0 	(*(volatile u32 *)(LCD_BASE + 0x00F0))
#define VIDW04ADD1B1 	(*(volatile u32 *)(LCD_BASE + 0x00F4))
#define VIDW04ADD1B2 	(*(volatile u32 *)(LCD_BASE + 0x20F0))
#define VIDW00ADD2 	(*(volatile u32 *)(LCD_BASE + 0x0100))
#define VIDW01ADD2 	(*(volatile u32 *)(LCD_BASE + 0x0104))
#define VIDW02ADD2 	(*(volatile u32 *)(LCD_BASE + 0x0108))
#define VIDW03ADD2 	(*(volatile u32 *)(LCD_BASE + 0x010C))
#define VIDW04ADD2 	(*(volatile u32 *)(LCD_BASE + 0x0110))
#define VIDINTCON0 	(*(volatile u32 *)(LCD_BASE + 0x0130))
#define VIDINTCON1 	(*(volatile u32 *)(LCD_BASE + 0x0134))
#define W1KEYCON0 	(*(volatile u32 *)(LCD_BASE + 0x0140))
#define VIDW0ALPHA0 	(*(volatile u32 *)(LCD_BASE + 0x021C))
#define VIDW0ALPHA1 	(*(volatile u32 *)(LCD_BASE + 0x0220))
#define VIDW1ALPHA0 	(*(volatile u32 *)(LCD_BASE + 0x0224))
#define VIDW1ALPHA1 	(*(volatile u32 *)(LCD_BASE + 0x0228))
#define VIDW2ALPHA0 	(*(volatile u32 *)(LCD_BASE + 0x022C))
#define VIDW2ALPHA1 	(*(volatile u32 *)(LCD_BASE + 0x0230))
#define VIDW3ALPHA0 	(*(volatile u32 *)(LCD_BASE + 0x0234))
#define VIDW3ALPHA1 	(*(volatile u32 *)(LCD_BASE + 0x0238))
#define VIDW4ALPHA0 	(*(volatile u32 *)(LCD_BASE + 0x023C))
#define VIDW4ALPHA1 	(*(volatile u32 *)(LCD_BASE + 0x0240))

#define WPALCON_L 	(*(volatile u32 *)(LCD_BASE + 0x01a0))
#define WIN0PAL(n) 	(*(volatile u32 *)(LCD_BASE + 0x2400 + (n) * 4))
#define WIN1PAL(n) 	(*(volatile u32 *)(LCD_BASE + 0x2800 + (n) * 4))
#define WIN2PAL(n) 	(*(volatile u32 *)(LCD_BASE + 0x2c00 + (n) * 4))
#define WIN3PAL(n) 	(*(volatile u32 *)(LCD_BASE + 0x3000 + (n) * 4))
#define WIN4PAL(n) 	(*(volatile u32 *)(LCD_BASE + 0x3400 + (n) * 4))

//私有数据空间类型
struct ldm_info {
	struct fb_ops ops;
	u32 lcdblk_reg, reg;
};

static struct fb_info *fb;

static int __init ldm_init(void)
{
	printk(KERN_ALERT "%s\n", __FUNCTION__);

	request_mem_region();

	//1 创建fb_info对象和私有数据空间
	fb = framebuffer_alloc(sizeof(struct ldm_info), NULL);
	if (!fb) {

	}

	//2 填充对象成员
	fb->var.xres = ;
	fb->var.yres = ;
	fb->var.xres_virtual = ;
	fb->var.yres_virtual = ;
	fb->var.bits_per_pixel = ;
	fb->var.red
	fb->var.green
	fb->var.blue

	fb->fix.smem_len = ;
	fb->fix.line_length = ;

	fb->screen_size = fb->fix.smem_len;

	//3 创建显存。为了保证cpu和dma在同时访问显存时的同步问题，需要特殊的cpu缓冲机制，使用dma_alloc_writecombine来创建显存
	//参数dma_handle将返回显存的物理地址， 函数返回值是虚拟地址
	fb->screen_base = dma_alloc_writecombine(NULL, fb->screen_size, (dma_addr_t*)&fb->fix.smem_start, );
	if (!fb->screen_base) {

	}

	//4 挂接操作方法
	fb->fbops =
	//下面三个方法已经在内核中实现，分别在内核源码中的drivers/video中的cfbcopyarea.c，cfbfillrect.c， cfbimgblt.c
	//但是三个函数所属的.c文件需要被编译链接进内核，我们才能访问这三个全局函数
	//内核中自带的framebuffer驱动一旦被去除后，默认将不会编译链接这3个文件，所以我们需要重新确保3个文件被编译链接
	fb->fbops->fb_fillrect = cfb_fillrect;
	fb->fbops->fb_copyarea = cfb_copyarea;
	fb->fbops->fb_imageblit = cfb_imageblit;

	//5 硬件的初始化
	//5.1 所在总线上的设备时钟的使能，手册P556
	CLK_GATE_IP_LCD[0]
	//5.2 时钟初始化
	ioremap
	//5.3 lcd控制器的初始化
	ioremap
	hardware_init();

	//6 提交注册fb_info
	register_framebuffer();

	return 0;
}

static void __exit ldm_exit(void)
{
	printk(KERN_ALERT "%s\n", __FUNCTION__);

	unregister_framebuffer();
	dma_free_writecombine();
	framebuffer_release(fb);
}

module_init(ldm_init);
module_exit(ldm_exit);

MODULE_LICENSE("GPL");