#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

#include <linux/fb.h> // /usr/include/linux/fb.h
#include "bmp.h"

/* 测试：
 * 如果在pc机上测试，需要通过ctrl+f1~f6切换一个真实终端，同时可以用ctrl+ f7切会桌面
 */

#define DEVNAME "/dev/fb0"
//#define DEVNAME "/dev/graphics/fb0"

#define RGB888(r, g, b) ( (r) << 16 | (g) << 8 | (b) )

typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

struct ldm_info {
	int fd;
	struct fb_fix_screeninfo fix; //固定参数
	struct fb_var_screeninfo var; //可变参数
	size_t x, y; //真实的显存分辨率
	size_t pixel_size; //每个像素占多少字节
	void *fb_addr; //映射到当前进程空间的显存首地址
	//参数color只支持RGB888这个宏提供的颜色格式
	void (*draw_pixel)(struct ldm_info *, size_t x, size_t y, u32 color);
};

/* 按照32bit色深格式画一个像素点
 * @info: framebuffer设备主对象
 * @x, y: 像素点坐标
 * @color: RGB888格式的颜色值
 */
static void draw_pixel_32bpp(struct ldm_info *info, size_t x, size_t y, u32 color)
{
	u32 red = (color >> 16) & 0xff;
	u32 green = (color >> 8) & 0xff;
	u32 blue = color & 0xff;
	*((u32*)info->fb_addr + y * info->x + x) = red << info->var.red.offset | green << info->var.green.offset | blue << info->var.blue.offset;
}

/* 画一个矩形
 * @info: framebuffer设备主对象
 * @x, y: 矩形左上角的坐标
 * @xlen, ylen: 矩形的边长
 * @color: RGB888格式的颜色值
 */
static void draw_rect(struct ldm_info *info, size_t x, size_t y, size_t xlen, size_t ylen, u32 color)
{
	size_t ix, iy;
	for (iy = 0; iy < ylen; ++iy) {
		for (ix = 0; ix < xlen; ++ix) {
			info->draw_pixel(info, x + ix, y + iy, color);
		}
	}
}

static void clear_screen(struct ldm_info *info, u32 color)
{
	draw_rect(info, 0, 0, info->x - 1, info->y - 1, color);
}

static int dislplay_bmp(const char *filename, struct ldm_info *info)
{
	FILE *fp = fopen(filename, "r");
	if (!fp) {
		perror("fopen bmp");
		goto err_fopen_bmp;
	}

	struct bmp_head *head = (struct bmp_head*)malloc(sizeof(struct bmp_head));
	if (!head) {
		perror("malloc bmp head");
		goto err_malloc_bmp_head;
	}

	//读取bmp的文件头
	fread(head, sizeof(struct bmp_head), 1, fp);

	//将bmp文件的操作指针移动到像素数据的入口
	fseek(fp, head->file.offset, SEEK_SET);

	//将像素数据读到显存里
	//fread(info->fb_addr, head->info.sizeimage, 1, fp);

	//保存bmp文件一行的数据的缓冲区
	u8 *line_buf = (u8*)malloc(head->info.width * (head->info.count / 8));
	if (!line_buf) {
		perror("malloc line buff");
		goto err_malloc_line_buff;
	}

	u32 pixel_buf = 0;

	size_t x, y;
	for (y = 0; y < head->info.height; ++y) {

		//bmp文件的像素数据是以y轴颠倒的，所以我们从最后一行开始读到第0行，倒着显示
		fseek(fp, head->file.offset + (info->y - y - 1) * (head->info.count / 8 * head->info.width), SEEK_SET);
		//读出一行数据
		fread(line_buf, head->info.width * (head->info.count / 8), 1, fp);

		for (x = 0; x < head->info.width; ++x) {

			switch (head->info.count) {
			case 32:
				pixel_buf = *(u32*)((u8*)line_buf + x * (head->info.count / 8));
				pixel_buf >>= 8;
				break;
			case 24:
				pixel_buf = *(u32*)((u8*)line_buf + x * (head->info.count / 8));
				break;
			case 16: //RGB565
				pixel_buf = *(u16*)((u8*)line_buf + x * (head->info.count / 8));
				//将RGB565格式转换成RGB888
				u32 red = (pixel_buf >> 11 << 3) & 0xff;
				u32 green = (pixel_buf >> 5 << 2) & 0xff;
				u32 blue = (pixel_buf << 3) & 0xff;
				pixel_buf = red << 16 | green << 8 | blue;
				break;
			}

			info->draw_pixel(info, x, y, pixel_buf);
		}
	}

	free(line_buf);
	free(head);
	fclose(fp);

	return 0;

err_malloc_line_buff:
	free(head);
err_malloc_bmp_head:
	fclose(fp);
err_fopen_bmp:
	return -1;
}

int main(int argc, char **argv)
{
	if (argc > 2) {
		fprintf(stderr, "Usage: cmd [bmp file]\n");
		return -1;
	}

	struct ldm_info *info = (struct ldm_info*)calloc(1, sizeof(struct ldm_info));
	if (!info) {
		return -1;
	}

	//1 打开/dev/fb0
	info->fd = open(DEVNAME, O_RDWR);
	if (info->fd < 0) {
		perror("open fd");
		goto err_open;
	}

	//2 用ioctl获取var和fix两个对象，并重新计算出像素分辨率
	ioctl(info->fd, FBIOGET_VSCREENINFO, &info->var);
	ioctl(info->fd, FBIOGET_FSCREENINFO, &info->fix);

	info->pixel_size = info->var.bits_per_pixel / 8;

	info->x = info->fix.line_length / info->pixel_size;
	info->y = info->fix.smem_len / info->fix.line_length;

	info->x /= info->var.xres_virtual / info->var.xres;
	info->y /= info->var.yres_virtual / info->var.yres;

	//================var===============
	printf("xres=%d, yres=%d, xres_virtual=%d, yres_virtual=%d\n", info->var.xres, info->var.yres, info->var.xres_virtual, info->var.yres_virtual);
	printf("bits_per_pixel=%d\n", info->var.bits_per_pixel);
	printf("red: offset=%d, length=%d, msb_right=%d\n", info->var.red.offset, info->var.red.length, info->var.red.msb_right);
	printf("green: offset=%d, length=%d, msb_right=%d\n", info->var.green.offset, info->var.green.length, info->var.green.msb_right);
	printf("blue: offset=%d, length=%d, msb_right=%d\n", info->var.blue.offset, info->var.blue.length, info->var.blue.msb_right);

	//===============fix================

	printf("x=%ld, y=%ld, pixel_size=%ld\n", info->x, info->y, info->pixel_size);

	//3 用mmap将驱动中的显存映射到当前进程空间
	info->fb_addr = mmap(NULL, info->fix.smem_len, PROT_READ | PROT_WRITE, MAP_SHARED, info->fd, 0);
	if (info->fb_addr == MAP_FAILED) {
		perror("mmap");
		goto err_mmap;
	}

	//挂接画一个像素点的底层成员方法
	switch (info->pixel_size) {
	case 2:
		break;
	case 4:
		info->draw_pixel = draw_pixel_32bpp;
		break;
	default:
		break;
	};

	//4 画图
	if (argc == 1) {
		clear_screen(info, RGB888(0, 0, 255));
		draw_rect(info, 100, 100, info->x - 200, info->y - 200, RGB888(255, 0, 0));
	} else {
		dislplay_bmp(argv[1], info);
	}


	pause();

	munmap(info->fb_addr, info->fix.smem_len);
	close(info->fd);
	free(info);

	return 0;

err_mmap:
	close(info->fd);
err_open:
	free(info);
	return -1;
}