#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

//arch/arm/mach-exynos/include/mach/regs-gpio.h
//实际所使用的宏S5P_VA_GPIO2在map-s5p.h中，S5P_VA_GPIO2表示GPIO part2的首地址
#include <mach/regs-gpio.h>

#include <linux/ioport.h> //request_mem_region
#include <linux/cdev.h>
#include <linux/fs.h>

#include <linux/device.h> //class_create


/* device_create将在/sys/class中创建ldm文件夹，在里面的dev文件中包含主设备号和次设备号
 * 系统中的mdev服务将通过扫描sys/class文件夹的信息，来获知新的设备结点的创建请求。mdev将根据这些信息，在/dev下创建相应的设备结点
 */

#define DEVNAME "ldm"

struct ldm_info {
	struct cdev dev;
	struct file_operations ops;
	dev_t devno;
	struct class *cls;
};
static struct ldm_info ldm;

#define GPM4CON (*(volatile u32*)(S5P_VA_GPIO2 + 0x02e0))
#define GPM4DAT (*(volatile u8*)(S5P_VA_GPIO2 + 0x02e4))

static inline void led_init(void)
{
	//将GPM4_0-3这4个管脚设为output
	GPM4CON = (GPM4CON & ~0xffff) | 0x1111;
}

//4个led灯的状态，0亮灯，1灭灯
static inline void led_on(u8 led_stat)
{
	//GPM4_0-3
	GPM4DAT = (GPM4DAT & ~0xf) | (led_stat & 0xf);
}

static ssize_t ldm_write(struct file *fp, const char __user *buf, size_t size, loff_t *offset)
{
	printk("%s: *buf=0x%x\n", __FUNCTION__, *buf);
	led_on(*buf);

	return 1;
}

static int ldm_init(void)
{
	printk("%s %s\n", __FILE__, __FUNCTION__);

	int ret = 0;

	//注册寄存器使用的地址区域，注册后可以在/proc/iomem中看到相应的注册信息。如果之前已被注册过，说明系统中已经存在这个设备的驱动程序，此时函数将返回错误
	if (!request_mem_region(EXYNOS4_PA_GPIO2 + 0x02e0, 8, DEVNAME)) {
		printk(KERN_ERR "request_mem_region failed\n");
		ret = -EBUSY;
		goto err_request_mem_region;
	}

	//1 创建并初始化cdev对象，用cdev_init或cdev_alloc
	cdev_init(&ldm.dev, &ldm.ops);
	ldm.ops.write = ldm_write;

	/* 2 dev_t类型包含主设备号和次设备号
	 * MKDEV，用主设备号和次设备号创建一个完整设备号cdev
	 * MAJOR，从dev_t中抽取主设备号
	 * MINOR，从dev_t中抽取次设备号
	 */

	//申请设备号，查看内核文档Documentation/devices.txt，找出可用的设备号。申请成功后，可在/proc/devices中查看到主设备号的注册信息
	ldm.devno = MKDEV(12, 0);
	ret = register_chrdev_region(ldm.devno, 1, DEVNAME);
	if (ret < 0) {
		printk(KERN_ERR "register_chrdev_region failed\n");
		ret = alloc_chrdev_region(&ldm.devno, 0, 1, DEVNAME);
		if (ret < 0) {
			printk(KERN_ERR "alloc_chrdev_region failed\n");
			goto err_alloc_chrdev_region;
		}
	}
	printk("devno=%d, major=%d, minor=%d\n", ldm.devno, MAJOR(ldm.devno), MINOR(ldm.devno));

	//3 设备注册
	ret = cdev_add(&ldm.dev, ldm.devno, 1);
	if (ret < 0) {
		printk(KERN_ERR "cdev_add failed\n");
		goto err_cdev_add;
	}

	//4 硬件初始化，中断申请
	led_init();

	//5 自动创建设备结点
	ldm.cls = class_create(THIS_MODULE, DEVNAME);
	if (IS_ERR(ldm.cls)) {
		ret = PTR_ERR(ldm.cls);
		goto err_class_create;
	}

	struct device *dev = device_create(ldm.cls, NULL, ldm.devno, NULL, DEVNAME);
	if (IS_ERR(dev)) {
		ret = PTR_ERR(dev);
		goto err_device_create;
	}

	return 0;

err_device_create:
	class_destroy(ldm.cls);
err_class_create:
	cdev_del(&ldm.dev);
err_cdev_add:
	unregister_chrdev_region(ldm.devno, 1);
err_alloc_chrdev_region:
	release_mem_region(EXYNOS4_PA_GPIO2 + 0x02e0, 8);
err_request_mem_region:
	return ret;
}

static void ldm_exit(void)
{
	printk("%s %s\n", __FILE__, __FUNCTION__);

	device_destroy(ldm.cls, ldm.devno);
	class_destroy(ldm.cls);
	cdev_del(&ldm.dev);
	unregister_chrdev_region(ldm.devno, 1);
	release_mem_region(EXYNOS4_PA_GPIO2 + 0x02e0, 8);
}

module_init(ldm_init);
module_exit(ldm_exit);

MODULE_LICENSE("GPL");

MODULE_AUTHOR("uplooking");
MODULE_DESCRIPTION("module test");