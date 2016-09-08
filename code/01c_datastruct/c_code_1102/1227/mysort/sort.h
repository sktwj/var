#pragma once
#include <stdio.h>

//排序的分类
//1 交换排序： 冒泡  快排
//2 选择排序： 一般选择 堆排序
//3 插入排序： 一般插入排序 希尔排序
//4 归并排序： 两路归并， 多路归并




typedef int elem_t;

struct sort{
    void (*bubble)(elem_t *, size_t);
    void (*select)(elem_t *, size_t);
    void (*insert)(elem_t *, size_t);

    void (*quick)(elem_t *, size_t);
    void (*heap)(elem_t *, size_t);
    void (*shell)(elem_t *, size_t);

    void (*merge)(elem_t *, size_t);
};

void sort_init(struct sort *);
void sort_destroy(struct sort *);
