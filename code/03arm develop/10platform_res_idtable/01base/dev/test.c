#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

#include <linux/platform_device.h>

static void ldm_release(struct device *dev)
{
	printk("%s %s\n", __FILE__, __FUNCTION__);
}

static struct platform_device ldm_dev = {
	.name = "ldm", //匹配platform_driver的关键词
	.dev = {
		//在当前platform_device对象注销时自动执行，本函数指针不能为空，否则注销时将段错误。当本对象写在内核中，永不注销时，本函数指针无需填充
		.release = ldm_release,
	},
};

static int ldm_init(void)
{
	printk("%s %s\n", __FILE__, __FUNCTION__);

	return platform_device_register(&ldm_dev);
}

static void ldm_exit(void)
{
	printk("%s %s\n", __FILE__, __FUNCTION__);

	platform_device_unregister(&ldm_dev);
}

module_init(ldm_init);
module_exit(ldm_exit);

MODULE_LICENSE("GPL");

MODULE_AUTHOR("uplooking");
MODULE_DESCRIPTION("module test");