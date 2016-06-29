#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

#include <linux/platform_device.h>

static int ldm_probe(struct platform_device *pdev)
{
	printk("%s %s\n", __FILE__, __FUNCTION__);
	return 0;
}

static int ldm_remove(struct platform_device *pdev)
{
	printk("%s %s\n", __FILE__, __FUNCTION__);
	return 0;
}

static struct platform_driver ldm_drv = {
	.driver = {
		.name = "ldm", //匹配platform_device的关键词
	},
	.probe = ldm_probe, //匹配成功时自动执行
	.remove = ldm_remove, //pdev和pdrv中其中之一被卸载，即匹配关系不再成立时，自动执行
};

static int ldm_init(void)
{
	printk("%s %s\n", __FILE__, __FUNCTION__);

	return platform_driver_register(&ldm_drv);
}

static void ldm_exit(void)
{
	printk("%s %s\n", __FILE__, __FUNCTION__);

	platform_driver_unregister(&ldm_drv);
}

module_init(ldm_init);
module_exit(ldm_exit);

MODULE_LICENSE("GPL");

MODULE_AUTHOR("uplooking");
MODULE_DESCRIPTION("module test");