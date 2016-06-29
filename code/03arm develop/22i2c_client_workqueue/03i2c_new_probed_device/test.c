#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

#include <linux/i2c.h>

/* 注册i2c_client时，会与内核中已经存在的i2c_client冲突，所以需要先关闭内核中的触摸屏驱动
 * make menuconfig -> Device Drivers -> Input device support -> Touchscreens -> FocalTech ft5x0x TouchScreen driver
 * 关闭
 */

//使用i2c_new_probed_device来注册i2c_client时，i2c_board_info中的从机地址无效，只采用匹配关键词。从机地址由addr_list来决定
static struct i2c_board_info ldm_ts_info = {
	I2C_BOARD_INFO("ldm_ts", 0x39), //从机地址0x39将不参与注册
};

//i2c_new_probed_device将依次遍历所有从机地址，直到找到一个和硬件上存在的设备吻合的从机地址
static const unsigned short addr_list[] = { 0x40, 0x38, 0x70, I2C_CLIENT_END };

static struct i2c_client *ldm_ts_client;

static int ldm_init(void)
{
	printk("%s %s\n", __FILE__, __FUNCTION__);

	//获得表示1号i2c总线的adapter对象
	struct i2c_adapter *i2c_adap = i2c_get_adapter(1);

	//最后一个参数是函数指针，可用于自定义匹配从机地址的算法，如果传NULL，将使用默认匹配算法，也就是发起一次i2c通信，只传从机地址，等待ack，如果得到ack回应则说明有对应从机设备
	ldm_ts_client = i2c_new_probed_device(i2c_adap, &ldm_ts_info, addr_list, NULL);
	if (!ldm_ts_client) {
		printk(KERN_ERR "i2c_new_device failed\n");
		goto err_i2c_new_device;
	}

	//销毁i2c_adapter
	i2c_put_adapter(i2c_adap);

	return 0;

err_i2c_new_device:
	i2c_put_adapter(i2c_adap);
	return -1;
}

static void ldm_exit(void)
{
	printk("%s %s\n", __FILE__, __FUNCTION__);

	i2c_unregister_device(ldm_ts_client);
}

module_init(ldm_init);
module_exit(ldm_exit);

MODULE_LICENSE("GPL");

MODULE_AUTHOR("uplooking");
MODULE_DESCRIPTION("module test");