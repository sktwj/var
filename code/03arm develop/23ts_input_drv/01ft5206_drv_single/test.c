#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

#include <linux/i2c.h>

#include <linux/input.h>
#include <linux/bitops.h> //set_bit tools/perf/util/include/linux/bitops.h
#include <mach/irqs.h>
#include <linux/interrupt.h>
#include <mach/regs-gpio.h>

#include <linux/workqueue.h>
#include <linux/sched.h>

/* 需要预先关闭内核中的触摸屏驱动，否则注册i2c_client时将出现冲突，驱动本身也冲突
 * i2c的读写操作，请参考内核文档Documentation/i2c/smbus-protocol
 */

//echo ldm_ts 0x38 > /sys/bus/i2c/devices/i2c-1/new_device

enum {
	X_SIZE = 800,
	Y_SIZE = 480,
};

enum {
	TD_STATUS	= 2,
	TOUCH1_XH	= 3,
};

enum {
	PUT_DOWN	= 0,
	PUT_UP		= 1,
	CONTACT		= 2,
};

struct ts_raw_data {
	u8 xh, xl, yh, yl;
	//u8 dummy[2];
};

struct ldm_info {
	struct input_dev *dev;
	struct i2c_client *client;
	u8 num_of_pt; //触摸点数量
	struct work_struct work;
	struct ts_raw_data pt_raw_data; //从ft5x06中读取的数据，未经处理
	u8 event_flag;
	size_t x, y;
};
static struct ldm_info ldm;

//本函数将在标准进程上下文中执行，而非中断上下文
static void ldm_work_func(struct work_struct *pwork)
{
	//printk(KERN_ALERT "%s\n", __FUNCTION__);

	i2c_smbus_read_i2c_block_data(ldm.client, TD_STATUS, sizeof(ldm.num_of_pt), &ldm.num_of_pt);

	//printk("num of touch point = %d\n", ldm.num_of_pt);

	//读取第一个触摸的数据
	i2c_smbus_read_i2c_block_data(ldm.client, TOUCH1_XH, sizeof(ldm.pt_raw_data), (u8*)&ldm.pt_raw_data);

	//处理数据
	ldm.x = (ldm.pt_raw_data.xh & 0xf) << 8 | ldm.pt_raw_data.xl;
	ldm.y = (ldm.pt_raw_data.yh & 0xf) << 8 | ldm.pt_raw_data.yl;
	ldm.event_flag = ldm.pt_raw_data.xh >> 6;

	//printk("x=%d,y=%d,event_flag=%d\n", ldm.x, ldm.y, ldm.event_flag);

	//发送event
	input_report_abs(ldm.dev, ABS_X, ldm.x);
	input_report_abs(ldm.dev, ABS_Y, ldm.y);

	switch (ldm.event_flag) {
	case PUT_DOWN:
	case CONTACT:
		input_report_key(ldm.dev, BTN_TOUCH, 1);
		input_report_abs(ldm.dev, ABS_PRESSURE, 1);
		break;
	case PUT_UP:
		input_report_key(ldm.dev, BTN_TOUCH, 0);
		input_report_abs(ldm.dev, ABS_PRESSURE, 0);
		break;
	}

	//哨兵报告
	input_sync(ldm.dev);
}

static irqreturn_t ts_handler(int irqno, void *arg)
{
	//printk("%s\n", __FUNCTION__);

	//退出中断模式返回到SVC后，立即执行work对象中绑定的func成员函数
	schedule_work(&ldm.work);

	return IRQ_HANDLED;
}

static int ldm_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	printk("%s %s\n", __FILE__, __FUNCTION__);

	printk("matched i2c_client is %s, 0x%x\n", id->name, client->addr);

	int ret = 0;

	ldm.client = client;

	//1 创建input_dev主对象
	ldm.dev = input_allocate_device();
	if (!ldm.dev) {
		printk(KERN_ERR "input_allocate_device failed\n");
		ret = -ENOMEM;
		goto err_input_allocate_device;
	}

	//2 填充input_dev成员
	//作为/proc/bus/input/devices中的设备注册信息中的标识
	ldm.dev->name = id->name;

	//注册输入设备允许发到应用层设备结点上的event数据包类型
	set_bit(EV_KEY, ldm.dev->evbit); //按键类型
	set_bit(EV_ABS, ldm.dev->evbit); //绝对坐标类型

	set_bit(BTN_TOUCH, ldm.dev->keybit);
	input_set_abs_params(ldm.dev, ABS_X, 0, X_SIZE - 1, 0, 0);
	input_set_abs_params(ldm.dev, ABS_Y, 0, Y_SIZE - 1, 0, 0);
	input_set_abs_params(ldm.dev, ABS_PRESSURE, 0, 1, 0, 0);

	//3 注册input_dev对象
	ret = input_register_device(ldm.dev);
	if (ret < 0) {
		printk(KERN_ERR "input_register_device failed\n");
		goto err_input_register_device;
	}

	//4 初始化硬件，注册中断
	ret = request_irq(IRQ_EINT(14), ts_handler, IRQF_TRIGGER_FALLING, ldm.dev->name, NULL);
	if (ret < 0) {
		printk(KERN_ERR "request_irq failed\n");
		goto err_request_irq;
	}

	//5 初始化工作队列的对象
	INIT_WORK(&ldm.work, ldm_work_func);

	return 0;

	free_irq(IRQ_EINT(14), NULL);
err_request_irq:
	input_unregister_device(ldm.dev);
err_input_register_device:
	input_free_device(ldm.dev);
err_input_allocate_device:
	return ret;
}

static int ldm_remove(struct i2c_client *client)
{
	printk("%s %s\n", __FILE__, __FUNCTION__);

	free_irq(IRQ_EINT(14), NULL);
	input_unregister_device(ldm.dev);
	input_free_device(ldm.dev);

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