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

static void uart_send_ch(u8 ch)
{
	while (!(UTRSTAT & 1 << 1)) {}
	UTXH = ch;
}

static void print_ch(u8 ch)
{
	switch (ch) {
	case '\n':
		uart_send_ch('\r');
		break;
	case '\r':
		uart_send_ch('\n');
		break;
	}

	uart_send_ch(ch);
}

static u8 uart_recv_ch()
{
	while (!(UTRSTAT & 1)) {}
	return URXH;
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
	UCON = 1 | 1 << 2;

	/* UFCON[10:8]: 发送fifo中的数据慢慢根据波特率传输出去，当fifo中的数据量空间低于某值时，说明可以继续向fifo中写入更多数据，所以触发发送中断，从而开启发送操作
	 * [6:4]: 接收fifo，接收到pc发送过来的数据，暂存在fifo中，当数据量达到和超过本值时，触发接收中断让cpu从fifo中取走数据。当然，如果是小量数据到来，即使接收fifo内数据量很小，也一样会在超时后触发接收中断,以保证数据接收的实时性
	 * [2][1]: 发送和接收fifo的重置，重置完成后自动清零
	 * [0]：使能
	 */
	UFCON = 0;

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


	//===================test=================
	u8 buf = 0;
	while (1) {
		buf = uart_recv_ch();
		print_ch(buf);
	}

}


module_init(uart_init);
