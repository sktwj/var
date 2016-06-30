#include "config.h"

#define GPX1CON (*(volatile u32*)0x11000c20)
#define EXT_INT41CON (*(volatile u32*)0x11000e04)
#define EXT_INT41_MASK (*(volatile u32*)0x11000f04)
#define EXT_INT41_PEND (*(volatile u32*)0x11000f44)

enum {
	TS_I2CBUS_NUM = 1,
	FT5X06_SLAVE_ADDR = 0x38,
};

enum {
	TD_STATUS			= 2,
	TOUCH1_XH			= 3,
	ID_G_AUTO_CLB_MODE	= 0xa0,
};

struct ts_id {
	u8 auto_calibration_mode;
	u8 firmware_lib_ver_H;
	u8 firmware_lib_ver_L;
	u8 chip_vendor_ID;
	u8 interrupt_mode;
};

struct ts_raw_data {
	u8 xh, xl, yh, yl;
	u8 dummy[2];
};

struct ts_info {
	struct ts_id id;
	size_t num_of_pt; //触摸点数量
	struct ts_raw_data pt_raw_data[5];
	u8 event_flag[5];
	size_t x[5], y[5];
};
static struct ts_info info;

static void ts_handler()
{
	//读取触摸点的数量
	i2c_block_read(TS_I2CBUS_NUM, FT5X06_SLAVE_ADDR, &info.num_of_pt, sizeof(info.num_of_pt), TD_STATUS);

	info.num_of_pt &= 0xf;
	if (!info.num_of_pt) {
		return;
	}

	printf("num of pointer = %d\n", info.num_of_pt);

	//读取触摸点的坐标
	i2c_block_read(TS_I2CBUS_NUM, FT5X06_SLAVE_ADDR, info.pt_raw_data, sizeof(struct ts_raw_data) * info.num_of_pt, TOUCH1_XH);

	//处理得到的数据
	size_t i = 0;
	for (i = 0; i < info.num_of_pt; ++i) {
		info.event_flag[i] = info.pt_raw_data[i].xh >> 6;

		info.x[i] = ((u32)(info.pt_raw_data[i].xh & 0xf) << 8) | info.pt_raw_data[i].xl;
		info.y[i] = ((u32)(info.pt_raw_data[i].yh & 0xf) << 8) | info.pt_raw_data[i].yl;

		printf("%d: x=%d, y=%d ", i, info.x[i], info.y[i]);

		switch (info.event_flag[i]) {
		case 0:
			printf("Put Down\n");
			break;
		case 1:
			printf("Put Up\n");
			break;
		case 2:
			printf("Contact\n");
			break;
		}
	}

	//清除中断标志
	EXT_INT41_PEND = 1 << 6;
}

static void ft5x06_check_mode()
{
	i2c_block_read(TS_I2CBUS_NUM, FT5X06_SLAVE_ADDR, &info.id, sizeof(struct ts_id), ID_G_AUTO_CLB_MODE);

	switch (info.id.auto_calibration_mode) {
	case 0:
		printf("enable auto calibration\n");
		break;
	case 0xff:
		printf("disable auto calibration\n");
		break;
	default:
		printf("unknown value of auto_calibration_mode 0x%x\n", info.id.auto_calibration_mode);
		break;
	}

	if (info.id.interrupt_mode) {
		printf("Trigger mode\n");
	} else {
		printf("Polling mode\n");
	}

	printf("lib_version = 0x%x, chip_id = 0x%x\n", info.id.firmware_lib_ver_H << 8 | info.id.firmware_lib_ver_L, info.id.chip_vendor_ID);
}

static void ft5x06_init()
{
	printf("%s\n", __FUNCTION__);

	i2c_init(TS_I2CBUS_NUM);

	ft5x06_check_mode();

	//注册外部中断
	request_irq(62, ts_handler);
	//GPX1_6设为外部中断功能
	GPX1CON |= 0xf << 24;
	//下降沿触发
	EXT_INT41CON = (EXT_INT41CON & ~(7 << 24)) | 2 << 24;
	//清除中断标志
	EXT_INT41_PEND = 1 << 6;
	//打开中断使能
	EXT_INT41_MASK &= ~(1 << 6);
}

//module_init(ft5x06_init);