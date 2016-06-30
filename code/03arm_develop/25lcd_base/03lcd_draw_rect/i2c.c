#include "config.h"

#define GPD1CON (*(volatile u32 *)0x114000c0)
#define GPD1PUD (*(volatile u32 *)0x114000c8)

#define I2CCON(n) (*(volatile u8*)(0x13860000 + (n)*0x10000))
#define I2CSTAT(n) (*(volatile u8*)(0x13860004 + (n)*0x10000))
#define I2CDS(n) (*(volatile u8*)(0x1386000c + (n)*0x10000))
#define I2CLC(n) (*(volatile u8*)(0x13860010 + (n)*0x10000))

enum {
	PSYS_PCLK = 100000000, //4412手册P446，PERI-L总线
	I2C_CLK = 400000, //<=400khz，大部分i2c器件所需SCL时钟
	CLK_DIV = 0, //0: PCLK/16, 1: PCLK/512
	PRESCALER = ((PSYS_PCLK) / 16 / (I2C_CLK)) - 1,
};

static inline void delay()
{
	size_t i = 0;
	for (i = 0; i < 10000; ++i) {}
}

/* I2CCON[3:0]: prescaler预分频值，用于对源时钟的预分频
 * [6]: 二级分频,16分频和512分频二选一
 * [5]: 中断使能，不管采用轮循还是中断方式，此位必开，
 * 	否则无法触发[4]的改变，那么就连轮循的条件都没有了
 * [4]: 中断标志，同时也是轮循标志。此位为1时，发生中断，
 * 	此时i2c通信处于暂停状态，SCL持续拉低。在相应情况处理后，
 * 	主机将该位写0清0，i2c通信会继续进行下去
 *  在这里，中断标志主要是用来检测从机ack的到来时机
 * [7]: 自动发送ACK的使能开关，打开后，本机会在合适的时机发送ACK到总线
 */

//轮询等待中断的触发
#define WAIT_INT while (!(I2CCON(n) & (1 << 4))) {}
//清除中断标志
#define CLR_INT do {I2CCON(n) &= ~(1 << 4);} while (0)
//开启ACK自动发送功能
#define ACK_ON do {I2CCON(n) |= 1 << 7;} while (0)
//关闭ACK自动发送功能
#define ACK_OFF do {I2CCON(n) &= ~(1 << 7);} while (0)

/* I2CSTAT[3:0]: 产生中断的原因，共4种
 * [4]: 收发使能，如关闭将无法访问I2CDS
 * [5]: 发送start或stop信号，只要对I2CSTAT写操作，必然发送其中一个信号
 * [7:6]: 4种模式选择，主从和发送接收
 * 	发送接收的选择将影响第一个周期，主机在最后一位发送0还是1
 * 	注意，想要通信的从机地址不能写入I2CADD里，而是从I2CDS发出
 * 	从机地址占据I2CDS的高7位，此时最低位无效，发送接受有本两位决定
 */

#define MASTER_TX_START do {I2CSTAT(n) = 3 << 6 | 1 << 5 | 1 << 4;} while (0)
#define MASTER_RX_START do {I2CSTAT(n) = 2 << 6 | 1 << 5 | 1 << 4;} while (0)
#define MASTER_TX_STOP do {I2CSTAT(n) = 3 << 6 | 1 << 4;} while (0)
#define MASTER_RX_STOP do {I2CSTAT(n) = 2 << 6 | 1 << 4;} while (0)

//busnum: 0或1，i2c总线的序号,i2c_no, 电容屏接的是1号总线
void i2c_init(size_t busnum)
{
	//1 GPIO的转换，开启SCL，SDA
	switch (busnum) {
	case 0:
		//GPD1_0-1
		GPD1CON = (GPD1CON & ~0xff) | 0x22;
		GPD1PUD &= ~0xf;
		break;
	case 1:
		//GPD1_2-3
		GPD1CON = (GPD1CON & ~(0xff << 8)) | 0x22 << 8;
		GPD1PUD &= ~(0xf << 4);
		break;
	default:
		break;
	}

	//2 时序设置，通常条件为SCL周期小于400KHZ
	I2CCON(busnum) = (PRESCALER & 0xf) | CLK_DIV << 6;

	//3 打开i2c的中断使能，为了轮询
	I2CCON(busnum) |= 1 << 5;

	//4 读写使能，对I2CDS具备了读写操作的可能
	I2CSTAT(busnum) |= 1 << 4;

	//5 设置sda的延时
	I2CLC(busnum) = 1 << 2 | 3;
}

/* @n: i2c总线的编号
 * @slave_addr: 本次通信的目标从机地址
 * @buf: 发送的数据的首地址
 * @size: 发送的数据的字节数
 */
void i2c_write(size_t n, u8 slave_addr, const void *buf, size_t size)
{
	I2CDS(n) = slave_addr << 1;
	MASTER_TX_START;
	//开启i2c通信，把I2CDS里的数据通过总线发出去
	CLR_INT;

	//当中断发生后，通信会暂停，等待我们的下一步操作。在ack中断到来前，不要轻举妄动。收到ACK后，i2c仍然处于暂停状态
	WAIT_INT;

	const u8 *data = buf;
	size_t i = 0;
	for (; i < size; ++i) {
		I2CDS(n) = data[i];
		//清除中断标志，使通信继续下去，使得I2CDS中的1个字节数据发送出去
		CLR_INT;
		WAIT_INT;
	}

	MASTER_TX_STOP;
	//发出去
	CLR_INT;

	//等待停止信号生效
	delay();
}

void i2c_read(size_t n, u8 slave_addr, void *buf, size_t size)
{
	if (size == 1) {
		ACK_OFF;
	} else {
		ACK_ON;
	}

	I2CDS(n) = slave_addr << 1;
	MASTER_RX_START;
	CLR_INT;

	//收到ACK之后，需要继续开启通信，否则i2c控制会一直拉低SCL管脚，让通信处于暂停状态
	WAIT_INT;

	//第一次ACK到达后，重新开启通信，接收从机第一个字节的数据
	CLR_INT;
	WAIT_INT;

	u8 *data = buf;
	size_t i = 0;
	for (; i < size; ++i) {
		data[i] = I2CDS(n);
		//在收到最后一个字节之前，也就是在倒数第二个字节的接收时，将ACK自动发送功能关闭
		if (i >= size - 2) {
			ACK_OFF;
		}
		//清除中断标志，使通信继续下去
		CLR_INT;
		WAIT_INT;
	}

	MASTER_RX_STOP;
	CLR_INT;

	//等待停止信号生效
	delay();
}

/* @n: i2c总线的编号
 * @slave_addr: 本次通信的目标从机地址
 * @buf: 发送的数据的首地址
 * @size: 发送的数据的字节数
 * @data_addr: i2c设备内部的地址
 */
void i2c_block_write(size_t n, u8 slave_addr, const void *buf, size_t size, u8 data_addr)
{
	I2CDS(n) = slave_addr << 1;
	MASTER_TX_START;
	//开启i2c通信，把I2CDS里的数据通过总线发出去
	CLR_INT;

	//当中断发生后，通信会暂停，等待我们的下一步操作。在ack中断到来前，不要轻举妄动。收到ACK后，i2c仍然处于暂停状态
	WAIT_INT;

	//发送data_addr，也就是i2c从机设备内部的地址
	I2CDS(n) = data_addr;
	CLR_INT;
	WAIT_INT;

	const u8 *data = buf;
	size_t i = 0;
	for (; i < size; ++i) {
		I2CDS(n) = data[i];
		//清除中断标志，使通信继续下去，使得I2CDS中的1个字节数据发送出去
		CLR_INT;
		WAIT_INT;
	}

	MASTER_TX_STOP;
	//发出去
	CLR_INT;

	//等待停止信号生效
	delay();
}

void i2c_block_read(size_t n, u8 slave_addr, void *buf, size_t size, u8 data_addr)
{
	//定位从机设备内部的操作指针的位置
	i2c_write(n, slave_addr, &data_addr, sizeof(data_addr));

	//从上次操作的位置开始进行读操作
	i2c_read(n, slave_addr, buf, size);
}