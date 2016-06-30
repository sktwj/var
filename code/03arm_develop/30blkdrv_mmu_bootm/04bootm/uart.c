#include "config.h"

#if 1
#define GPA0CON (*(volatile u32*)0x11400000)

#define ULCON (*(volatile u32*)0x13800000)
#define UCON (*(volatile u32*)0x13800004)
#define UFCON (*(volatile u32*)0x13800008)
#define UTRSTAT (*(volatile u32*)0x13800010)
#define UFSTAT (*(volatile u32*)0x13800018)
#define UTXH (*(volatile u8*)0x13800020)
#define URXH (*(volatile u8*)0x13800024)
#define UBRDIV (*(volatile u32*)0x13800028)
#define UFRACVAL (*(volatile u32*)0x1380002c)
#define UINTP (*(volatile u32*)0x13800030)
#define UINTSP (*(volatile u32*)0x13800034)

#else

#define GPA0CON (*(volatile u32*)0x21400000)

#define ULCON (*(volatile u32*)0x23800000)
#define UCON (*(volatile u32*)0x23800004)
#define UFCON (*(volatile u32*)0x23800008)
#define UTRSTAT (*(volatile u32*)0x23800010)
#define UFSTAT (*(volatile u32*)0x23800018)
#define UTXH (*(volatile u8*)0x23800020)
#define URXH (*(volatile u8*)0x23800024)
#define UBRDIV (*(volatile u32*)0x23800028)
#define UFRACVAL (*(volatile u32*)0x2380002c)
#define UINTP (*(volatile u32*)0x23800030)
#define UINTSP (*(volatile u32*)0x23800034)

#endif

#define SCLK_UART 100000000 //100MHZ，手册P1393，串口时钟
#define BAUD_RATE 115200

static void send_ch(u8 ch)
{
	while (!(UTRSTAT & (1 << 1))) {}
	//判断UTRSTAT
	UTXH = ch;
}

//串口协议遵循windows对于回车的标准，串口的一个回车是由\n\r两个字符组成
void print_ch(u8 ch)
{
	if (ch == '\n') {
		send_ch('\r');
	} else if (ch == '\r') {
		send_ch('\n');
	}
	send_ch(ch);
}

void print_str(const char *str)
{
	for (; *str; ++str) {
		print_ch(*str);
	}
}

u8 uart_recv_poll()
{
	while (!(UTRSTAT & 1)) {}
	//根据UTRSTAT来判断是否收到数据
	return URXH;
}

static void uart_init()
{
	//1 管脚GPIO设置GPA0_0和GPA0_1
	GPA0CON = (GPA0CON & ~0xff) | 0x22;

	//2 初始化寄存器
	/* ULCON: 8N1 */
	ULCON = 3;

	/* UCON
	 * [3:0]: 中断或轮询，非DMA模式
	 * [4]: 是否发送停止信号，不发送
	 * [5]: 非环回模式，环回loop-back即自发自收
	 * [6]: 错误中断使能，关
	 * [7]: 接收超时是否产生接收中断，如果工作在中断模式则必开
	 * [9:8]: 中断触发类型，只能是电平触发
	 * [10]: 接收超时的dma休眠使能
	 * [11]: 与[7]联动，中断模式必开
	 * [15:12]: 接收超时中断的触发间隔时间
	 * [18:16][22:20]: dma burst size
	 */
	UCON = 0b0101;

	//fifo关闭
	UFCON = 0;

	/* UBRDIV UFRACVAL, 分频得到波特率，详情和公式见手册P1410 */
	//DIV_VAL = SCLK_UART / (bps * 16) - 1 = 100MHZ / (115200 * 16) - 1 = 53.25
	//DIV_VAL = UBRDIV + UFRACVAL/16
	//UBRDIV是DIV_VAL的整数部分53，UFRACVAL/16是小数部分，UFRACVAL = 0.25 * 16 = 4
	UBRDIV = 53;
	UFRACVAL = 4;
}

static void uart_test()
{
	print_str("uart_test\n");

	while (1) {
		print_ch(uart_recv_poll());
	}
}

module_init(uart_init);
//module_init(uart_test);