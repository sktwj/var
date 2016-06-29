#include "config.h"

#if 0
#define GPM4CON (*(volatile unsigned int*)0x110002e0)
#define GPM4DAT (*(volatile unsigned char*)0x110002e4)

#else

#define GPM4CON (*(volatile u32*)0x210002e0)
#define GPM4DAT (*(volatile u8*)0x210002e4)

#endif

static void delay(size_t n)
{
	while (n--) {}
}

void led_init()
{
	GPM4CON = (GPM4CON & ~0xffff) | 1 | 1 << 4 | 1 << 8 | 1 << 12;
}

static void led_on(u8 n)
{
	GPM4DAT = (GPM4DAT & ~0xf) | (n & 0xf);
}

void led_test()
{
	while (1) {
		led_on(0b1010);
		delay(0xfffff);
		led_on(0b0101);
		delay(0xfffff);
	}
}

module_init(led_init);
//module_init(led_test);