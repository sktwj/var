#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

static int ldm_init(void)
{
	printk("%s %s\n", __FILE__, __FUNCTION__);
	return 0;
}

static void ldm_exit(void)
{
	printk("%s %s\n", __FILE__, __FUNCTION__);
}

module_init(ldm_init);
module_exit(ldm_exit);

MODULE_LICENSE("GPL");

MODULE_AUTHOR("uplooking");
MODULE_DESCRIPTION("module test");