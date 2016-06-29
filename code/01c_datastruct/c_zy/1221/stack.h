#pragma once
#include "list.h"

typedef int selem_t;
struct snode {
	selem_t data;
	struct list_head list;
};

//描述栈的类型
struct stack {
	struct list_head head; 

	void (*push)(struct stack *, selem_t); //入栈
	selem_t (*pop)(struct stack *); //出栈
	selem_t (*top)(struct stack *); //访问栈顶元素
	int (*is_empty)(struct stack *); //判断栈是否为空
};

void stack_init(struct stack *);
void stack_destroy(struct stack *);
