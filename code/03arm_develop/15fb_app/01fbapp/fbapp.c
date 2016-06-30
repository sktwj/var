#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

#include <linux/fb.h> // /usr/include/linux/fb.h

/* 测试：
 * 如果在pc机上测试，需要通过ctrl+f1~f6切换一个真实终端，同时可以用ctrl+ f7切会桌面
 */

//#define DEVNAME "/dev/fb0"
#define DEVNAME "/dev/graphics/fb0"

#define RGB888(r, g, b) ( (r) << 16 | (g) << 8 | (b) )

typedef unsigned int u32;

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

int main()
{
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
	clear_screen(info, RGB888(0, 0, 255));
	draw_rect(info, 100, 100, info->x - 200, info->y - 200, RGB888(255, 0, 0));

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