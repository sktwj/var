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
	SECTOR_SIZE = 512, //扇区的大小
	DEV_SIZE = 1024 * 100 * SECTOR_SIZE, //设备的总存储量
};

#define DEVNAME "ldm_blk"

struct ldm_info {
	struct gendisk *disk;
	struct block_device_operations fops;
	void *addr; //设备的访问地址
	spinlock_t lock; //提供给blk_init_queue使用
};
static struct ldm_info ldm;

//从请求队列上获取请求操作对象，从请求对象中获得操作参数：读写操作的起始sector和操作字节数，然后将所需的操作执行到硬件上去
//本函数是由blk驱动框架来自动调用的，调用时机由电梯算法调度决定
static void do_ldm_req(struct request_queue *q)
{
	//从请求队列上获取一个请求对象
	struct request *req = blk_fetch_request(q);
	while (req) {
		//从第几个扇区开始操作
		u32 start = blk_rq_pos(req) * SECTOR_SIZE;
		//获得当前请求操作的字节数
		u32 len = blk_rq_cur_bytes(req);

		//检查本次request操作是否越界
		int err = 0;
		if (start + len > DEV_SIZE) {
			printk(KERN_ERR "request region is out of device capacity\n");
			err = -EIO;
			goto err_request;
		}

		//rq_data_dir获得当前请求的操作方向
		//建议在memcpy前后加上打印语句，以便观察读写操作的调度时机
		//数据从内核传输到应用
		if (rq_data_dir(req) == READ) {
			memcpy(req->buffer, (u8*)ldm.addr + start, len);
			printk("read from %d, size %d\n", start, len);
		} else { //数据从应用层传输到内核并写入
			memcpy((u8*)ldm.addr + start, req->buffer, len);
			printk("write from %d, size %d\n", start, len);
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

	int ret = 0;

	//1 创建块设备gendisk的主对象，函数参数是最大分区数
	ldm.disk = alloc_disk(10);
	if (!ldm.disk) {
		ret = -ENOMEM;
		printk(KERN_ERR "alloc_disk failed\n");
		goto err_alloc_disk;
	}

	//2 注册申请块设备的主设备号，可以从Documentation/devices.txt中寻找空闲的设备号,也可以在第一个参数处填入0，让系统自动分配。注册成功后可在/proc/devices中查询到注册信息
	if (register_blkdev(12, DEVNAME) < 0) {
		ret = register_blkdev(0, DEVNAME);
		if (ret < 0) {
			printk(KERN_ERR "register_blkdev failed\n");
			goto err_register_blkdev;
		} else {
			ldm.disk->major = ret;
		}
	} else {
		ldm.disk->major = 12;
	}
	printk("blkdev major num = %d\n", ldm.disk->major);


	//3 填充gendisk成员
	ldm.disk->fops = &ldm.fops; //包含fdisk分区所需的操作方法
	sprintf(ldm.disk->disk_name, DEVNAME);
	//填充gendisk中描述存储容量的对象成员，注意单位是扇区数(扇区大小一般是512字节)
	set_capacity(ldm.disk, DEV_SIZE / SECTOR_SIZE);

	//4 创建块设备的存储空间
	//分配设备的存储空间，我们用内存来虚拟设备。vmalloc适合分配大块空间，分配的空间取自内存碎片的拼接，在物理地址上非连续，在虚拟地址上连续，比较节省内存空间
	ldm.addr = vzalloc(DEV_SIZE);
	if (!ldm.addr) {
		printk(KERN_ERR "vzalloc failed\n");
		ret = -ENOMEM;
		goto err_vmalloc;
	}

	//5 注册块设备所需的操作队列,提供框架所需的自旋锁对象
	spin_lock_init(&ldm.lock);
	ldm.disk->queue = blk_init_queue(do_ldm_req, &ldm.lock);
	if (!ldm.disk->queue) {
		printk(KERN_ERR "blk_init_queue failed\n");
		ret = -ENOMEM;
		goto err_blk_init_queue;
	}

	//6 注册块设备对象
	add_disk(ldm.disk);

	return 0;

	//blk_cleanup_queue(ldm.disk->queue);
err_blk_init_queue:
	vfree(ldm.addr);
err_vmalloc:
	unregister_blkdev(ldm.disk->major, DEVNAME);
err_register_blkdev:
	del_gendisk(ldm.disk);
err_alloc_disk:
	return ret;
}

static void __exit ldm_exit(void)
{
	printk(KERN_ALERT "%s\n", __FUNCTION__);

	put_disk(ldm.disk);
	blk_cleanup_queue(ldm.disk->queue);
	vfree(ldm.addr);
	unregister_blkdev(ldm.disk->major, DEVNAME);
	del_gendisk(ldm.disk);
}

module_init(ldm_init);
module_exit(ldm_exit);

MODULE_LICENSE("GPL");