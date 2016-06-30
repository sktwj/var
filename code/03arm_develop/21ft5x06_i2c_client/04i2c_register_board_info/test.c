#include <linux/init.h> //module_init
#include <linux/kernel.h> //printk
#include <linux/module.h> //MODULE_LICENSE

#include <linux/i2c.h>

//本注册i2c_client的方法仅限于zImage内执行，而且必须在i2c_bus注册之前执行，否则无效。一般本函数将在板级支持的初始化函数中执行，当前4412平台，在mach-tiny4412.c中的machine_desc类型的主对象中的init_machine成员函数所指向的smdk4x12_machine_init中执行

static struct i2c_board_info __initdata ldm_i2c_board_info[] = {
	{
		//I2C_BOARD_INFO两个参数分别写入.type和.addr中，type就是用于匹配i2c_client的关键词，addr就是从机地址
		I2C_BOARD_INFO("ldm_touchscreen", 0x38),
	},
};

static int test_init(void)
{
	printk("%s:%s\n", __FILE__, __FUNCTION__);

	//i2c_register_board_info无法在模块中执行，必须在内核注册i2c core之前才能用来正确注册i2c client，典型的可以在mach_tiny4412.c的初始化函数中执行
	return i2c_register_board_info(1, ldm_i2c_board_info, ARRAY_SIZE(ldm_i2c_board_info));
}

static void test_exit(void)
{
	printk("%s:%s\n", __FILE__, __FUNCTION__);
}

module_init(test_init);
module_exit(test_exit);

MODULE_LICENSE("GPL");