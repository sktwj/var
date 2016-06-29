#include "config.h"

#define WTCON (*(volatile u32*)0x10060000)
#define WTDAT (*(volatile u32*)0x10060004)
#define WTCNT (*(volatile u32*)0x10060008)
#define WTCLRINT (*(volatile u32*)0x1006000c)

enum {
	//手册P446表格，wdt位于PERI-R总线
	WDT_PCLK = 100000000, //100MHZ
	WDT_PRESCALER = 99, //1级分频值是(PRESCALER+1)
	WDT_CLK_DIV = 3, //2级分频值是(16 << CLK_DIV)
	WDT_HZ = WDT_PCLK / (WDT_PRESCALER+1) / (16 << WDT_CLK_DIV),
};

static void wdt_handler()
{
	printf("%s\n", __FUNCTION__);

	WTCLRINT = 0;
}

static void wdt_init(void)
{
	WTDAT = WDT_HZ;
	WTCNT = WDT_HZ;

	request_irq(75, wdt_handler);

	WTCON = 1 << 2 | WDT_CLK_DIV << 3 | 1 << 5 | WDT_PRESCALER << 8;
}



#define GPD0CON (*(volatile u32*)0x114000a0)
#define TCFG0 (*(volatile u32*)0x139d0000)
#define TCFG1 (*(volatile u32*)0x139d0004)
#define TCON (*(volatile u32*)0x139d0008)
#define TCNTB0 (*(volatile u32*)0x139d000c)
#define TCMPB0 (*(volatile u32*)0x139d0010)
#define TCNTB4 (*(volatile u32*)0x139d003c)
#define TINT_CSTAT (*(volatile u32*)0x139d0044)

enum {
	//手册P446表格，pwm位于PERI-L总线
	TIMER_PCLK = 100000000,
	TIMER_PRESCALER0 = 99,
	TIMER0_DIV = 4,
	TIMER0_KHZ = TIMER_PCLK / (TIMER_PRESCALER0 + 1) / (1 << TIMER0_DIV) / 1000,
	TIMER_PRESCALER1 = 99, //1级分频值是(PRESCALER+1)
	TIMER4_DIV = 4, //2级分频值是(1 << TIMER4_DIV)
	TIMER4_HZ = TIMER_PCLK / (TIMER_PRESCALER1 + 1) / (1 << TIMER4_DIV),
};


//1级分频
static void timer_prescaler_init()
{
	//TCFG0 = (TCFG0 & ~0xffff) | TIMER_PRESCALER1 << 8 | TIMER_PRESCALER0;
	TCFG0 = TIMER_PRESCALER0 | TIMER_PRESCALER1 << 8;
}

//用timer0控制蜂鸣器的音调（频率）和音量（占空比）
static void timer0_init()
{
	//关闭定时器0
	TCON &= ~1;

	//GPIO，GPD0_0设为TOUT0
	GPD0CON = (GPD0CON & ~0xf) | 0x2;

	//2级分频
	TCFG1 = (TCFG1 & ~0xf) | TIMER0_DIV;

	TCNTB0 = TIMER0_KHZ;
	TCMPB0 = TIMER0_KHZ / 10;

	/* TCON[0]: 定时器的开关，必须在初始化完成后才能打开
	 * [1]: 手工将TCNTB0和TCMPB0的值写入定时器，
	 * 		只需一瞬间，写入操作就完成了，完成后需要再将该位清0，
	 * 		否则，灌入操作将会不停地执行，定时器将无法开始计数
	 * [2]: 定时器输出电平的极性设置，参考4412手册P1300
	 * [3]: 定时器是周期式运行（auto-reload）;还是单次执行，一个周期后就停止计数
	 */
	//[1]: 更新TCNTB0和TCMPB0的值到定时器内部，作为初值；[3]auto-reload
	TCON |= 1 << 1 | 1 << 3;
	//[1]:关闭TCNTB0和TCMPB0的更新;[2]:输出Inverter Off，也就是输出电平先低后高
	TCON &= ~(1 << 1 | 1 << 2);
	//打开timer0
	TCON |= 1;
}

static void timer4_handler()
{
	static int timer0_stat = 0;

	printf("%s: timer0_stat=%d\n", __FUNCTION__, timer0_stat);

	switch (timer0_stat) {
	case 0:
		TCNTB0 = TIMER0_KHZ;
		TCMPB0 = TCNTB0 / 10;
		break;
	case 1:
		TCNTB0 = TIMER0_KHZ;
		TCMPB0 = TCNTB0 - 10;
		break;
	case 2:
		TCNTB0 = TIMER0_KHZ * 5;
		TCMPB0 = TCNTB0 - 10;
		break;
	case 3:
		TCNTB0 = TIMER0_KHZ * 5;
		TCMPB0 = TCNTB0 / 10;
		break;
	}

	if (timer0_stat >= 3) {
		timer0_stat = 0;
	} else {
		++timer0_stat;
	}


	//清除中断标志
	TINT_CSTAT |= 1 << 9;
}

static void timer4_init()
{
	//关闭timer4
	TCON &= ~(1 << 20);

	TCFG1 = (TCFG1 & ~(0xf << 16)) | TIMER4_DIV << 16;

	TCNTB4 = TIMER4_HZ;

	//清空中断标志，使能中断
	TINT_CSTAT |= 1 << 4 | 1 << 9;
	request_irq(73, timer4_handler);

	//[21]:填充TCNT4的值到定时器中；[22]: 重复触发，自动装填TCNT4
	TCON |= 1 << 21 | 1 << 22;
	//关闭填充动作，否则填充将一直进行，那么TCNT将无法自减以计时
	TCON &= ~(1 << 21);
	//打开定时器
	TCON |= 1 << 20;
}

//module_init(wdt_init);
module_init(timer_prescaler_init);
module_init(timer0_init);
module_init(timer4_init);