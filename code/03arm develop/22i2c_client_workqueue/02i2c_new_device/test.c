#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

#include <linux/i2c.h>

static struct i2c_board_info ldm_ts_info = {
	I2C_BOARD_INFO("ldm", 0x39),
};

static struct i2c_client *ldm_ts_client;

static int ldm_init(void)
{
	printk("%s %s\n", __FILE__, __FUNCTION__);

	//获得表示1号i2c总线的adapter对象
	struct i2c_adapter *i2c_adap = i2c_get_adapter(1);

	ldm_ts_client = i2c_new_device(i2c_adap, &ldm_ts_info);
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