#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/blkdev.h>
#include <linux/vmalloc.h>
#include <linux/spinlock.h> //自旋锁，不会造成休眠
#include <linux/fs.h> //READ宏定义

/* 测试驱动
 * 1 使用mkdosfs来格式化设备
 * mkdosfs /dev/XXX
 * 2 使用mount来挂载该设备
 * mount dev/XXX /mnt
 */

//参考内核驱动drivers/block/z2ram.c
//分区操作fdisk需要block_device_operations中提供getgeo，获取sectors，cylinders，headers。因为分区最小单位是cylinder
//getgeo的写法参考内核驱动drivers/block/hd.c
//测试分区操作fdisk /dev/XXXX

enum {
	SECTOR_SIZE = 512,
};

//从请求队列上获取请求操作对象，从请求对象中获得操作参数：读写操作的起始sector和终止sector，然后将所需的操作执行到硬件上去
//本函数是由blk驱动框架来自动调用的，调用时机由电梯算法调度决定
static void do_ldm_req(struct request_queue *q)
{
	//从请求队列上获取一个请求对象
	req = blk_fetch_request(q);
	while (req) {
		//从第几个扇区开始操作
		start = blk_rq_pos() << 9;
		//获得当前请求操作的字节数
		len = blk_rq_cur_bytes()

		//检查本次request操作是否越界
		int err = 0;
		if (start + len > ldm.size) {
			printk(KERN_ERR "request region is out of device capacity\n");
			err = -EIO;
			goto err_request;
		}

		//rq_data_dir获得当前请求的操作方向
		//建议在memcpy前后加上打印语句，以便观察读写操作的调度时机
		if (rq_data_dir(req) == READ) {
			memcpy();
			printk();
		} else {
			memcpy();
			printk();
		}

		//__blk_end_request_cur：返回false表示当前req的所有操作都完成了，于是下面试图调用blk_fetch_request再从队列上获取新的请求，如果获取不到，则req得到NULL将退出循环；
		//返回true的话说明当前req操作还没完成，继续循环执行
		//err参数可以独立改变__blk_end_request_cur的返回值，err<0时，函数返回false。当发生其他错误时可以用err参数来结束当前req请求，从请求队列上获取新的请求
err_request:
		if (!__blk_end_request_cur(req, err)) {
			req = blk_fetch_request(q);
		}
	}
}

static int __init ldm_init(void)
{
	printk(KERN_ALERT "%s\n", __FUNCTION__);

	//1 注册申请主设备号，可以从Documentation/devices.txt中寻找空闲的设备号,也可以在第一个参数处填入0，让系统自动分配。注册成功后可在/proc/devices中查询到注册信息
	register_blkdev();

	//2 创建块设备gendisk的主对象，函数参数是最大分区数
	alloc_disk();

	//3 填充gendisk成员
	.major = ;
	.fops = ; //包含fdisk分区所需的操作方法
	sprintf(.disk_name, " ");
	//填充gendisk中描述存储容量的对象成员，注意单位是扇区数(扇区大小一般是512字节)
	set_capacity();

	//4 创建块设备的存储空间
	//分配设备的存储空间，我们用内存来虚拟设备。vmalloc适合分配大块空间，分配的空间取自内存碎片的拼接，在物理地址上非连续，在虚拟地址上连续，比较节省内存空间
	vmalloc();

	//5 注册块设备所需的操作队列,提供框架所需的自旋锁对象
	spin_lock_init();
	blk_init_queue(do_ldm_req, );

	//6 注册块设备对象
	add_disk();

	return 0;
}

static void __exit ldm_exit(void)
{
	printk(KERN_ALERT "%s\n", __FUNCTION__);
}

module_init(ldm_init);
module_exit(ldm_exit);

MODULE_LICENSE("GPL");