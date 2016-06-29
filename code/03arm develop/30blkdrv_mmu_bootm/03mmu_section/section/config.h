#pragma once

typedef unsigned int u32;
typedef signed int s32;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned char u8;
typedef signed char s8;
typedef u32 size_t;
typedef s32 ssize_t;

#define NULL ((void*)0)

#define module_init(pfunc) static void (*ptr_##pfunc)() __attribute__((__section__(".initcall"))) = pfunc