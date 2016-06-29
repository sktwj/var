#include "config.h"

//背光芯片的控制信息因为采用了cpld加密，所以无法直接控制。可以先加载一次内核，点亮背光后，再按reset重启，此时背光已打开，可看见lcd上的内容

#define CLK_SRC_LCD0 (*(volatile u32*)0x1003c234)
#define CLK_DIV_LCD (*(volatile u32*)0x1003c534)
#define LCDBLK_CFG (*(volatile u32*)0x10010210)

#define GPF0CON (*(volatile u32*)0x11400180)
#define GPF1CON (*(volatile u32*)0x114001a0)
#define GPF2CON (*(volatile u32*)0x114001c0)
#define GPF3CON (*(volatile u32*)0x114001e0)

#define LCD_BASE 0x11C00000
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

enum {
	//分辨率
	XSIZE = ,
	YSIZE =

	LCD_SCLK_SRC = 800000000, //时钟源800MHZ，取自MPLL
	//lcd控制器时序图P1816，lcd屏手册时序图P13
	//以下参数查阅lcd屏的手册
	VCLK = , //像素时钟，该时钟每个周期传输一个像素的24bit数据

	//极性
	IVCLK =
	IHSYNC =
	IVSYNC =
	IVDEN =

	//时序
	VBPD =,
	VFPD = ,
	VSPW = ,
	HBPD = ,
	HFPD = ,
	HSPW =
};

struct win_info {
#define RGB565_BPP_MODE 0b0101
#define RGB888_BPP_MODE 0b1011
#define PAL8_BPP_MODE 0b0011
	u8 bpp;
	size_t pixel_size; //每个像素占用的字节数
#define RGB565_SWAP_MODE 0b0010
#define PAL8_SWAP_MODE 0b0100
	u8 swap_mode; //相邻的几个像素的排列顺序,看手册, 如果是RGB565，看P1779,如果是RGB888，P1774
	u32 start_addr; //显存起始地址
	u8 alpha_val; //4位alpha值,数值越小，透明度越高

	/* @x, y: 画出的对应像素点的坐标
	 * @color: RGB888格式的颜色值
	 */
	void (*draw_pixel)(struct win_info *, ssize_t x, ssize_t y, u32 color);
};

static struct win_info win[] = {
	[0] = {

	},
};

static void lcd_off()
{
	//关闭window
	WINCON0 &= ~1;
	WINCON1 &= ~1;
	WINCON2 &= ~1;
	WINCON3 &= ~1;
	WINCON4 &= ~1;

	//关闭lcd控制器
	VIDCON0 &= ~0x3;
}

static void lcd_on()
{

}

static void lcd_init()
{
	//1 GPIO功能转换

	//2 时钟设置
	//2.1 打开gate寄存器, 裸机环境下，总线开关其实时钟的打开的，进入内核后，内核为了节省功耗，会全部关闭这些设备，需要对应的驱动程序来打开
	CLK_GATE_IP_LCD ;

	//2.2 配置输入时钟sclk, 手册P457右上角图
	//选择MPLL输出做时钟源,800MHZ
	CLK_SRC_LCD0 = (CLK_SRC_LCD0 & ~0xf) | 0b0110;
	//选择FIMD的输入时钟源分频值,不分频
	CLK_DIV_LCD &= ~0xf;

	//3 配置lcd系统时钟由FIMD来使用, LCDBLK_CFG[1], 手册P884
	LCDBLK_CFG |= 1 << 1;

	//4 lcd控制器初始化
	//关闭lcd控制器和所有window，在初始化完毕后再打开
	lcd_off();

	/* VIDCON0[1:0]: lcd控制器开关，基本上都是同时置1或清0的
	 * [5]: 像素时钟VCLK的开关是否受lcd控制器开关的影响，无所谓
	 * [13:6]: 分频值，LCD_SCLK_SRC / (CLKVAL+1) = VCLK, VCLK的值从lcd屏的手册上查找
	 * [16]: 如果改变了分频值，那么新的分频值什么时候生效，无所谓
	 * [18]: RGB总线的接法是串行还是并行
	 * [25:20]: i80相关，么用
	 * [28:26]: 总线类型，RGB interface
	 */
	VIDCON0 = ;

	/* VIDCON1[7:4]： 主要是时序管脚的极性设置，lcd控制器的管脚极性必须和lcd屏的参数接近或吻合，才能匹配, lcd控制器时序图P1816，lcd屏手册时序图P13
	 * [14:13]: 可读出当前时刻在时序的哪一个位置,可用于轮询式的双缓冲
	 */
	VIDCON1 = ;

	//设置时序，注意单位。HSPW， HFPD, HBPD, VSPW, VFPD, VBPD
	//lcd控制器时序图P1816，lcd屏手册时序图P13
	VIDTCON0 = ;
	VIDTCON1 = ;
	//分辨率，从时序图上看
	VIDTCON2 = ;

	//打开chanel0，即window0的数据通道，需要打开其他数据通道所绑定的chanel和window时，设置相应位即可。chanel和window之间的连接关系由， WINCHMAP2,只负责连接关系的确认。Local Path需要关闭，和dma冲突
	SHADOWCON = 0x1f;

	//=================window0==============
	//初始化window，任选一个window，注意window0作为最底下的window，不能设置透明度
	/* WINCON0[0]: window使能开关
	 * [1][6]: window0不能设alpha，所以无效
	 * [5:2]: 色深
	 * [10:9]: dma的burst模式，连续传输数据，设最大值
	 * [13]: 色彩空间选择,RGB
	 * [14][21:19][31:30]: 多重显存指定机制，无所谓，一般不使用
	 * [18:15]:swap相关，看手册, 如果是RGB565，看P1779,如果是RGB888，P1774
	 * [22]: dma
	 */
	WINCON0 = win[0].bpp << 2 | win[0].swap_mode << 15;

	//指定window显示区域的坐标范围
	VIDOSD0A = ; //显示区域左上角坐标
	VIDOSD0B = ; //右下角
	//指定的显示区域的大小,注!意!单!位!
	VIDOSD0C = ;

	//指定显存的首末地址
	//W00表示windows0，ADD0表示起始地址，ADD1表示显存终止地址，B0中的0表示buf0的显存首地址
	VIDW00ADD0B0 = win[0].start_addr;
	//显存终止地址,如果需要支持横向滚动的话，终止地址中将包含offsize的大小，如果不支持横向滚动，offsize就是0
	VIDW00ADD1B0 = ;
	//手册P1813，有关横向移动显示区域相关，OFFSIZE指允许的横向移动范围, PAGEWIDTH指X方向上的字节数（即一行像素所占的字节数）。一般不用支持横向移动。如果不打算支持横向移动，那么OFFSIZE_F = 0
	VIDW00ADD2 = XSIZE * win[0].pixel_size;

	//===============================================

	//打开lcd控制器
	lcd_on();
}

module_init(lcd_init);