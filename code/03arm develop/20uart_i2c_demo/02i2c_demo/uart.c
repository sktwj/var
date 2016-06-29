#include "config.h"

#define GPA0CON (*(volatile u32*)0x11400000)
#define ULCON (*(volatile u32*)0x13800000)
#define UCON (*(volatile u32*)0x13800004)
#define UFCON (*(volatile u32*)0x13800008)
#define UTRSTAT (*(volatile u32*)0x13800010)
#define UFSTAT (*(volatile u32*)0x13800018)
#define UTXH (*(volatile u8*)0x13800020)
#define URXH (*(volatile u8*)0x13800024)
#define UBRDIV (*(volatile u16*)0x13800028)
#define UFRACVAL (*(volatile u8*)0x1380002c)
#define UINTP (*(volatile u8*)0x13800030)
#define UINTM (*(volatile u8*)0x13800038)

enum {
	UART_CLK = 100000000, //PERI_L 100MHZ
	UART_BAUDRATE = 115200, //串口波特率
};

//中断模式下，目前可以发送数据
static volatile int uart_sendable = 1;

#if 1
static void uart_send_ch_poll(u8 ch)
{
	while (!(UTRSTAT & 1 << 1)) {}
	UTXH = ch;
}
#endif

static void uart_send_ch_int(u8 ch)
{
	//发送fifo已满
	if (UFSTAT & (1 << 24)) {
		uart_sendable = 0;
		UINTM &= ~(1 << 2); //开启发送中断
	} else { //发送fifo不满，还有剩余空间
		uart_sendable = 1;
	}

	while (!(uart_sendable)) {}
	UINTM |= 1 << 2; //关闭发送中断

	UTXH = ch;
}

static void print_ch(u8 ch)
{
	switch (ch) {
	case '\n':
		uart_send_ch_int('\r');
		break;
	case '\r':
		uart_send_ch_int('\n');
		break;
	}

	uart_send_ch_int(ch);
}

static void print_ch_poll(u8 ch)
{
	switch (ch) {
	case '\n':
		uart_send_ch_poll('\r');
		break;
	case '\r':
		uart_send_ch_poll('\n');
		break;
	}

	uart_send_ch_poll(ch);
}

static void print_str(const char *str)
{
	while (*str) {
		print_ch(*str++);
	}
}

static u8 uart_recv_ch()
{
	while (!(UTRSTAT & 1)) {}
	return URXH;
}


/* 接收中断产生的两个时机：
 * 1 超时，当连续接收数据过程中，突然有一段时间收不到数据，表示一整块数据的接收告一段落，此时触发超时中断
 * 2 接收缓冲区剩余空间不足，即使没有出现明显的接收时间间隔，但是接收fifo空间不足，仍然产生中断来清空接收fifo
 */

 //发送中断触发时机：发送fifo中有足够剩余空间
static void uart_handler()
{
	u8 int_stat = UINTP;
	size_t i = 0;

	//uart_send_ch_poll('z');

	//接收中断
	if (int_stat & 1) {
		//for (i = 0; i < UFSTAT & 0xff; ++i) {
		//	print_ch(URXH);
		//}
		//接收中断触发条件：1接收缓冲区内数据大于等于设定阈值；
		//2收到数据后经过预定时间后再无收到后续数据
		while ( (UFSTAT & (1 << 8)) || (UFSTAT & 0xff) ) {

			//===========================================
			//开启发送中断后，如果同时又在接收中断中使用依赖发送中断才能运行的函数时，会引起中断嵌套，所以要改用轮询版本
			print_ch_poll(URXH);
			//===========================================
		}
		UINTP = 1;
	//发送中断
	} else if (int_stat & (1 << 2)) {
		uart_sendable = 1;
		UINTP = 1 << 2;
	}
}

static void uart_init()
{
	//1 RXD和TXD对应的GPIO管脚的设置,GPA0_0-1
	GPA0CON = (GPA0CON & ~0xff) | 0x22;

	ULCON = 3; //8n1

	/* UCON[3:2][1:0]: 发送接收模式，选用中断或轮询都是0b01
	 * [4]: 是否发送停止信号，不发
	 * [5]: 环回模式，自己的发送和接收内部连在一起，也就是自发自收，仅用于测试，不采用
	 * [6]: 在遇到接收错误时是否触发错误中断，非必须
	 * [7]: 接收超时中断，在使用接收fifo中断时必须打开。当接收到一串数据后，在之后的限定时间里，再没有收到过数据，即可判定上一次传输已经完成，直接触发接收中断
	 * [9:8]: 发送和接收中断的触发方式，看手册小字，必须选择电平触发
	 * [10]: dma相关，不使用
	 * [11]: 接收fifo为空时，是否使能超时中断
	 * [15:12]: 接收超时中断的时间间隔设置
	 * [22:20][18:16]: dma相关，不使用
	 */
	UCON = 1 | 1 << 2 | 1 << 7 | 1 << 8 | 1 << 9 | 3 << 12;

	/* UFCON[10:8]: 发送fifo中的数据慢慢根据波特率传输出去，当fifo中的数据量空间低于某值时，说明可以继续向fifo中写入更多数据，所以触发发送中断，从而开启发送操作
	 * [6:4]: 接收fifo，接收到pc发送过来的数据，暂存在fifo中，当数据量达到和超过本值时，触发接收中断让cpu从fifo中取走数据。当然，如果是小量数据到来，即使接收fifo内数据量很小，也一样会在超时后触发接收中断,以保证数据接收的实时性
	 * [2][1]: 发送和接收fifo的重置，重置完成后自动清零
	 * [0]：使能
	 */
	//UFCON = 1 | 1 << 1 | 1 << 2 | 7 << 4 | 7 << 8;
	UFCON |= 1 << 1;
	UFCON |= 1; //打开fifo

	/* UTRSTAT: 关闭fifo关中断的情况下，可通过轮询[1:0]来获取发送接收的时机
	 * fifo关闭的情况，缓冲区只有一个字节
	 * 发送时，1个字节的缓冲区里写入数据后，串口将自动按照波特率进行发送操作。在整个发送过程中，发送缓冲区始终是满的，也就是UTRSTAT[1]为0，此时不可向发送缓冲区写入数据，会覆盖正在发送的字节
	 * 接收时，一直轮询UTRSTAT[0]，直到收到数据，一个字节的数据将自动进入接收缓冲区，UTRSTAT[0]会自动置1，此时才可以去读取接收缓冲区的数据
	 */

	/* UERSTAT: 开启接收错误中断使能（UCON[6]）时，本寄存器表明错误类型
	 */

	/* UFSTAT: fifo开启时才有用
	 * [7:0]: 接收fifo中当前拥有多少字节数据，注意空和满时，都为0，需要再 通过[8]来进一步判断是空还是满
	 * [9]: 是否发生接收错误
	 * [23:16]: 发送缓冲区已拥有多少字节数据，为0时，可能是满也可能是空，根据[24]进一步判断
	 */

	/* UTXH URXH： 发送和接收的入口，建议设为8位类型来使用，发送时，连续依次反复对UTXH进行写操作，接收时连续依次反复对URXH，每次读写访问一个字节 */

	/* 比特率配置, 设置一个分频值，将总线时钟分频为115200这样的串口波特率，其中UBRDIV保存分频值的整数部分， UFRACVAL保存分频值的小数部分，看4412手册P1410的例子 */
	/* DIV_VAL = (UART_CLK / (UART_BAUDRATE * 16)) - 1
	 * 		= (100000000 / (115200 * 16)) - 1
	 * 		= 53.25
	 * UBRDIVn = 53, UFRACVAL/16 = 0.25, UFRACVAL = 4
	 */
	UBRDIV = 53;
	UFRACVAL = 4;

	//清除中断标志
	UINTP = 0xf;
	//打开接收中断，其余3个中断全关
	UINTM = 0b1110;
	request_irq(84, uart_handler);

	//===================test=================
	print_str("uart test hello world\n");

#if 0
	u8 buf = 0;
	while (1) {
		buf = uart_recv_ch();
		print_ch(buf);
	}
#endif

	while (1) {}
}


module_init(uart_init);
