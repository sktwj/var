typedef unsigned long u32;
typedef signed long s32;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned char u8;
typedef signed char s8;

#define GPM4CON (*(volatile u32*)0x110002e0)
#define GPM4DAT (*(volatile u8*)0x110002e4)
#define GPX3CON (*(volatile u32*)0x11000c60)
#define GPX3DAT (*(volatile u8*)0x11000c64)

void led_init(void);
void key_init(void);
void led_on(u8 led_stat);
u8 key_stat(void);

void led_init(void)
{
	//将GPM4_0-3这4个管脚设为output
	GPM4CON = (GPM4CON & ~0xffff) | 0x1111;
}

void key_init(void)
{
	//将GPX3_2-5设为输入
	GPX3CON &= ~(0xffff << 8);
}

//4个led灯的状态，0亮灯，1灭灯
void led_on(u8 led_stat)
{
	//GPM4_0-3
	GPM4DAT = (GPM4DAT & ~0xf) | (led_stat & 0xf);
}

//4个按键的状态，0按下，1抬起
u8 key_stat(void)
{
	//GPX3_2-5
	return (GPX3DAT >> 2) & 0xf;
}