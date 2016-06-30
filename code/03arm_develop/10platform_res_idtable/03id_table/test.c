#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

#include <linux/platform_device.h>
#include <mach/irqs.h>

static int ldm_probe(struct platform_device *pdev)
{
	printk("%s %s\n", __FILE__, __FUNCTION__);

	printk("match name %s\n", pdev->id_entry->name);

	int ret = 0;

	struct resource *res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		printk(KERN_ERR "platform_get_resource mem failed\n");
		ret = -ENXIO;
		goto err_platform_get_resource_mem;
	}
	printk("wdt reg addr = 0x%x\n", res->start);

	int irqno = platform_get_irq(pdev, 0);
	if (irqno < 0) {
		ret = irqno;
		printk(KERN_ERR "platform_get_irq failed\n");
		goto err_platform_get_irq;
	}
	if (irqno == IRQ_WDT) {
		printk("get irqno correctly\n");
	}

	return 0;

err_platform_get_irq:
err_platform_get_resource_mem:
	return ret;
}

static int ldm_remove(struct platform_device *pdev)
{
	printk("%s %s\n", __FILE__, __FUNCTION__);
	return 0;
}

//根据platform_bus_type.match成员函数匹配算法，id_table指向的数组对象最后必须以哨兵结尾
static struct platform_device_id ldm_id_table[] = {
	{"hello"}, {"world"}, {"aaaaaa"}, {"s3c2410-wdt"}, {""},
};

static struct platform_driver ldm_drv = {
	.driver = {
		//使用id_table作为匹配关键词时，本成员仍然不能为空，否则依然会段错误
		.name = "aaa",
	},
	.probe = ldm_probe, //匹配成功时自动执行
	.remove = ldm_remove, //pdev和pdrv中其中之一被卸载，即匹配关系不再成立时，自动执行
	//当id_table对象不为NULL时，将使用id_table作为匹配方式
	.id_table = ldm_id_table,
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