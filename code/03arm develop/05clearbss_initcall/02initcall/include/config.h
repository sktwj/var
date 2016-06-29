#pragma once

typedef unsigned long u32;
typedef unsigned short u16;
typedef unsigned char u8;
typedef signed long s32;
typedef signed short s16;
typedef signed char s8;
typedef u32 size_t;
typedef s32 ssize_t;

//定义一个函数指针指向uboot中的printf的入口，该函数入口地址来自uboot源码中的System.map文件，System.map文件是uboot编译时通过nm工具重定向生成的
static int (*printf)(const char *, ...) = (int (*)(const char *, ...))0xc3e11bc0;

//创建一个全局函数指针，并将它链接到initcall段中。在程序初始阶段会遍历initcall段，挨个执行每一个函数指针
#define module_init(pfunc) static void (*ptr_##pfunc)() __attribute__((__section__(".initcall"))) = pfunc