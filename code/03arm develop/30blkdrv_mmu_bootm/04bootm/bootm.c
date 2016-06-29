#include "config.h"

//参考内核文档Documentation/arm/Booting

enum {
	SDRAM_START		= 0x40000000,
	SDRAM_END		= 0x80000000,
	//KERNEL_ENTRY	= SDRAM_START + 0x8000,
	TAGLIST_ENTRY	= SDRAM_START + 0x3000,
	//arch/arm/mach-exynos/mach-tiny4412.c最后，MACHINE_START宏的第一个参数是TINY4412, 将被拼接成MACH_TYPE_TINY4412这个宏，作为struct machine_desc __mach_desc_TINY4412对象的.nr成员，该成员是用于匹配bootloader启动内核的第二个参数machine_type。struct machine_desc将被链接到.arch.info.init段中，在内核启动时遍历查找
	//bootloader启动内核时传递给内核的machine_type必须和nr成员一致，才能匹配，匹配成功后执行machine_desc中用于板级初始化的成员函数。如果无法匹配.arch.info.init段中任何machine_desc，内核将拒绝启动
	//MACH_TYPE_TINY4412这个宏定义在arch/arm/tools/mach-types中，即4608
	MACHINE_TYPE	= 4608,
};

#define CMDLINE "noinitrd root=/dev/nfs rw nfsroot=192.168.0.90:/home/yq/work/nfsroot ip=192.168.0.100 console=ttySAC0,115200 init=/linuxrc lcd=S70"

enum {
	ATAG_CORE	= 0x54410001,
	ATAG_MEM	= 0x54410002,
	ATAG_CMDLINE	= 0x54410009,
	ATAG_NONE	= 0x00000000,
};

struct tag_header {
	u32 size; //word数
	u32 tag;
};

struct tag_core {
	struct tag_header header;
	u32 flags;		/* bit 0 = read-only */
	u32 pagesize;
	u32 rootdev;
};

//描述物理内存的地址范围区间
struct tag_mem32 {
	struct tag_header header;
	u32	size;
	u32	start;	/* physical start address */
};

//启动参数，包含根文件系统挂载的方式
struct tag_cmdline {
	struct tag_header header;
	char	cmdline[((sizeof(CMDLINE) + 3) & ~3)];	/* this is the minimum size */
};

//哨兵
struct tag_end {
	struct tag_header header;
};

struct tag_list {
	struct tag_core core;
	struct tag_mem32 mem;
	struct tag_cmdline cmdline;
	struct tag_end end;
};

void bootm(u32 kernel_entry)
{
	printk("%s\n", __FUNCTION__);
	//填充taglist，参考u-boot/arch/arm/lib/bootm.c中的do_bootm_linux函数
	struct tag_list *tag = (struct tag_list*)(TAGLIST_ENTRY);

	tag->core.header.size = sizeof(struct tag_core) >> 2;
	tag->core.header.tag = ATAG_CORE;
	tag->core.flags = 0;		/* bit 0 = read-only */
	tag->core.pagesize = 0;
	tag->core.rootdev = 0;

	tag->mem.header.size = sizeof(struct tag_mem32) >> 2;
	tag->mem.header.tag = ATAG_MEM;
	tag->mem.size = SDRAM_END - SDRAM_START;
	tag->mem.start = SDRAM_START;

	tag->cmdline.header.size = sizeof(struct tag_cmdline) >> 2;
	tag->cmdline.header.tag = ATAG_CMDLINE;
	strcpy(tag->cmdline.cmdline, CMDLINE);

	tag->end.header.size = 0;
	tag->end.header.tag = ATAG_NONE;

	printk("cmdline=%s\n", tag->cmdline.cmdline);

	irq_off();
	mmu_off();

	void (*boot_kernel)(u32, u32, u32) = (void (*)(u32, u32, u32))kernel_entry;

	boot_kernel(0, MACHINE_TYPE, TAGLIST_ENTRY);
}