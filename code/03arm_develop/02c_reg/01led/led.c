typedef unsigned long u32;
typedef signed long s32;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned char u8;
typedef signed char s8;

#define GPM4CON (*(volatile u32*)0x110002e0)
#define GPM4DAT (*(volatile u8*)0x110002e4)

void _start(void)
{
	//将GPM4_0-3这4个管脚设为output
	GPM4CON = (GPM4CON & ~0xffff) | 0x1111;

	//GPM4_0-3清零，4灯全亮
	GPM4DAT &= ~0xf;
}
