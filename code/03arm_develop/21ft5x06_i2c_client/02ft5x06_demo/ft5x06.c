#include "config.h"

enum {
	TS_I2CBUS_NUM = 1,
	FT5X06_SLAVE_ADDR = 0x38,
};

enum {
	ID_G_AUTO_CLB_MODE = 0xa0,
};

struct ts_id {
	u8 auto_calibration_mode;
	u8 firmware_lib_ver_H;
	u8 firmware_lib_ver_L;
	u8 chip_vendor_ID;
	u8 interrupt_mode;
};

struct ts_info {
	struct ts_id id;
};
static struct ts_info info;

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
}

module_init(ft5x06_init);