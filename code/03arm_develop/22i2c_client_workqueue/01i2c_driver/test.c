#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

#include <linux/i2c.h>

static int ldm_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	printk("%s %s\n", __FILE__, __FUNCTION__);

	printk("matched i2c_client is %s, 0x%x\n", id->name, client->addr);

	return 0;
}

static int ldm_remove(struct i2c_client *client)
{
	printk("%s %s\n", __FILE__, __FUNCTION__);

	return 0;
}

//根据i2c_bus_type.match成员函数匹配算法，id_table指向的数组对象最后必须以哨兵结尾
static struct i2c_device_id ldm_id_table[] = {
	{"hello"}, {"world"}, {"aaaaaa"}, {"ldm_ts"}, {""},
};

static struct i2c_driver ldm_driver = {
	//匹配成功时自动执行
	.probe = ldm_probe,
	//i2c_client和i2c_driver中其中之一被卸载，即匹配关系不再成立时，自动执行
	.remove = ldm_remove,
	.driver = {
		//使用id_table作为匹配关键词时，本成员仍然不能为空，否则依然会段错误
		.name = "aaa",
	},
	//当id_table对象不为NULL时，将使用id_table作为匹配方式
	.id_table = ldm_id_table,
};

static int ldm_init(void)
{
	printk("%s %s\n", __FILE__, __FUNCTION__);

	int ret = i2c_add_driver(&ldm_driver);
	if (ret < 0) {
		printk(KERN_ERR "i2c_add_driver failed\n");
		goto err_i2c_add_driver;
	}

	return 0;

err_i2c_add_driver:
	return ret;
}

static void ldm_exit(void)
{
	printk("%s %s\n", __FILE__, __FUNCTION__);

	i2c_del_driver(&ldm_driver);
}

module_init(ldm_init);
module_exit(ldm_exit);

MODULE_LICENSE("GPL");

MODULE_AUTHOR("uplooking");
MODULE_DESCRIPTION("module test");