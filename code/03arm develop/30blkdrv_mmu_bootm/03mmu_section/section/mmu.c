#include "config.h"

//参考arm v7手册P1272
#define CACHE_ON (1 << 12 | 1 << 3 | 1 << 2) //Outer and Inner Write-Back, Write-Allocate
#define CACHE_OFF 0 //strongly ordered

//descriptor type
enum {
	SECTION_DESCRIPTOR = 0b10,
	PAGE_DESCRIPTOR = 0b01,
};

/* 填充一条页表项，参考arm v7手册P1256, P1248
 * @ttb: 页表首地址translation table base addr
 * @mva： 虚拟地址
 * @pa： 物理地址
 * @cach_flag: 所映射的这1M空间是否开缓冲
 */
static void create_section_desc(u32 ttb, u32 mva, u32 pa, int cache_flag)
{
	//页表项所在地址
	u32 addr = ttb | (mva >> 20 << 2);

	*(volatile u32*)addr = (pa & ~0xfffff) | cache_flag | SECTION_DESCRIPTOR;
}

void create_translation_table(u32 ttb)
{
	size_t i = 0;

	//线性映射1G内存空间
	for (i = 0x40000000; i < 0x80000000; i += (1024 * 1024)) {
		create_section_desc(ttb, i, i, CACHE_ON);
	}

	//线性映射SFR寄存器区域
	for (i = 0x10000000; i < 0x14000000; i += (1024 * 1024)) {
		create_section_desc(ttb, i, i, CACHE_OFF);
	}

	//线性映射SFR寄存器区域
	for (i = 0x10000000; i < 0x14000000; i += (1024 * 1024)) {
		create_section_desc(ttb, i + 0x10000000, i,  CACHE_OFF);
	}
}