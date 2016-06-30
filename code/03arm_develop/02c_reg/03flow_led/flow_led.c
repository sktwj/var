typedef unsigned long u32;
typedef signed long s32;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned char u8;
typedef signed char s8;
typedef u32 size_t;
typedef s32 ssize_t;

#define GPM4CON (*(volatile u32*)0x110002e0)
#define GPM4DAT (*(volatile u8*)0x110002e4)

static inline void led_init(void);
static inline void led_on(u8 led_stat);
static inline void delay(size_t count);

#define DELAY_COUNT 0xfffff

void _start(void)
{
	led_init();

	u8 led_stat = 1;

	while (1) {
		led_on(~led_stat);
		if (led_stat == 0b1000) {
			led_stat = 1;
		} else {
			led_stat <<= 1;
		}
		delay(DELAY_COUNT);
	}
}

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

static inline void delay(size_t count)
{
	size_t i = 0;
	for (i = 0; i < count; ++i) {}
}