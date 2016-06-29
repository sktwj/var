#pragma once
#include <stdio.h>

//排序的分类：
//1、交换排序：冒泡排序，快速排序
//2、选择排序：一般选择排序，堆排序
//3、插入排序：一般插入排序，希尔排序
//4、归并排序：两路归并，多路归并

typedef int elem_t;
struct sort {
	void (*bubble)(elem_t *, size_t); //冒泡排序
	void (*select)(elem_t *, size_t); //选择排序
	void (*insert)(elem_t *, size_t); //插入排序
	
	void (*quick)(elem_t *, size_t); //快速排序
	void (*heap)(elem_t *, size_t); //堆排序
	void (*shell)(elem_t *, size_t); //希尔排序
	
	void (*merge)(elem_t *, size_t); //两路归并排序
};

void sort_init(struct sort *);
void sort_destroy(struct sort *);
