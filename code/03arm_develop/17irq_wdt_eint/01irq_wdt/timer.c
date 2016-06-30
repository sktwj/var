#include "config.h"

#define WTCON (*(volatile u32*)0x10060000)
#define WTDAT (*(volatile u32*)0x10060004)
#define WTCNT (*(volatile u32*)0x10060008)
#define WTCLRINT (*(volatile u32*)0x1006000c)

enum {
	//手册P446表格，wdt位于PERI-R总线
	PCLK = 100000000, //100MHZ
	PRESCALER = 99, //1级分频值是(PRESCALER+1)
	CLK_DIV = 3, //2级分频值是(16 << CLK_DIV)
	WDT_HZ = PCLK / (PRESCALER+1) / (16 << CLK_DIV),
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

	WTCON = 1 << 2 | CLK_DIV << 3 | 1 << 5 | PRESCALER << 8;
}
module_init(wdt_init);