#include "config.h"

#define LEVEL2_PAGE_TABLE_BASE 0x47000000

//参考arm v7手册P1272
#define CACHE_ON (1 << 6 | 1 << 3 | 1 << 2) //Outer and Inner Write-Back, Write-Allocate
#define CACHE_OFF 0 //strongly ordered

//descriptor type
enum {
	SECTION_DESCRIPTOR = 0b10,
	PAGE_LEVEL1_DESCRIPTOR = 0b01,
	PAGE_LEVEL2_DESCRIPTOR = 0b10,
};

//参考arm v7手册P1258， P1250
//填充一条2级页表项，映射4K空间
static void create_page_level2_desc(u32 ttb, u32 mva, u32 pa, int cache_flag)
{
	//2级页表项的地址
	u32 addr = ((mva >> 20) * 1024 + LEVEL2_PAGE_TABLE_BASE) | ((mva >> 12) & 0xff) << 2;

	*(volatile u32*)addr = (pa & ~0xfff) | cache_flag | PAGE_LEVEL2_DESCRIPTOR;
}

/* 填充一条1级页表项以及所对应的1个二级页表，映射1M空间，参考arm v7手册P1258, P1248
 * @ttb: 页表首地址translation table base addr
 * @mva： 虚拟地址
 * @pa： 物理地址
 * @cach_flag: 所映射的这1M空间是否开缓冲
 */
static void create_page_desc(u32 ttb, u32 mva, u32 pa, int cache_flag)
{
	//1 填充一条一级页表项
	//1.1 1级页表项所在地址
	u32 addr = ttb | (mva >> 20 << 2);

	//二级页表将按顺序，连续存放在LEVEL2_PAGE_TABLE_BASE之后，每个2级页表占据1K大小，二级页表基地址的[9:0]是0
	//将1级页表和所有2级页表的组合都看成数组，那么1级页表项和2级页表都分别是两个数组的元素，两者的对应关系，意味着对应的两者，数组下标应该相同，元素大小不同，前者4个字节，后者1K
	*(volatile u32*)addr = ((mva >> 20) * 1024 + LEVEL2_PAGE_TABLE_BASE) | PAGE_LEVEL1_DESCRIPTOR;

	//2 循环填充对应的2级页表的所有256个页表项
	size_t i = 0;
	for (i = 0; i < 256; ++i) {
		create_page_level2_desc(ttb, mva + i * 4096, pa + i * 4096, cache_flag);
	}
}

//创建1级和2级页表
void create_translation_table(u32 ttb)
{
	size_t i = 0;

	//线性映射1G内存空间
	for (i = 0x40000000; i < 0x80000000; i += (1024 * 1024)) {
		create_page_desc(ttb, i, i, CACHE_ON);
	}

	//线性映射SFR寄存器区域
	for (i = 0x10000000; i < 0x14000000; i += (1024 * 1024)) {
		create_page_desc(ttb, i, i, CACHE_OFF);
	}

	//线性映射SFR寄存器区域
	for (i = 0x10000000; i < 0x14000000; i += (1024 * 1024)) {
		create_page_desc(ttb, i + 0x10000000, i,  CACHE_OFF);
	}
}