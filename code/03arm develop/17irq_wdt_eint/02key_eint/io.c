#include "config.h"

#define GPX3CON (*(volatile u32*)0x11000c60)
#define EXT_INT43CON (*(volatile u32*)0x11000e0c)
#define EXT_INT43_FLTCON0 (*(volatile u32*)0x11000e98)
#define EXT_INT43_FLTCON1 (*(volatile u32*)0x11000e9c)
#define EXT_INT43_MASK (*(volatile u32*)0x11000f0c)
#define EXT_INT43_PEND (*(volatile u32*)0x11000f4c)

static void key_handler()
{
	printf("%s: ", __FUNCTION__);

	u32 key_stat = (EXT_INT43_PEND >> 2) & 0xf;

	//判断中断源
	switch (key_stat) {
	case 1:
		printf("key0\n");
		//清除中断标志，写1清0
		EXT_INT43_PEND |= 1 << 2;
		break;
	case 2:
		printf("key1\n");
		EXT_INT43_PEND |= 1 << 3;
		break;
	case 4:
		printf("key2\n");
		EXT_INT43_PEND |= 1 << 4;
		break;
	case 8:
		printf("key3\n");
		EXT_INT43_PEND |= 1 << 5;
		break;
	default:
		printf("key_stat=%d\n", key_stat);
		//清除中断标志，写1清0
		EXT_INT43_PEND |= 0xf << 2;
		break;
	}
}

static void key_init()
{
	//将GPX3_2-5设为外部中断功能
	GPX3CON |= 0xffff << 8;

	//下降沿触发中断
	EXT_INT43CON = (EXT_INT43CON & ~(0xffff << 8)) | 2 << 8 | 2 << 12 | 2 << 16 | 2 << 20;
	//延时去抖
	EXT_INT43_FLTCON0 |= 0xffff << 16;
	EXT_INT43_FLTCON0 |= 0xffff;
	//清除中断标志，写1清0
	EXT_INT43_PEND |= 0xf << 2;
	//打开中断使能
	EXT_INT43_MASK &= ~(0xf << 2);

	//EINT26-29同属于64号中断号
	request_irq(64, key_handler);
}
module_init(key_init);