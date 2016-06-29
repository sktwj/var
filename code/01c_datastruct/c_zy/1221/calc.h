#pragma once
#include "stack.h"

enum state_flag {
	OPR, NUM,
};

typedef int num_t;

struct calc {
	const char *expr; //待处理的表达式
	const char *ptr; //用于遍历待处理的表达式的指针

	enum state_flag flag; //用于判断上一次处理的符号还是数字

	struct stack *num_stack; //存放运算数的栈
	struct stack *opr_stack; //存放运算符的栈

	//用于解析表达式的主函数
	num_t (*analyze)(struct calc *, const char *expr);
};

struct calc *calc_init(struct calc *, const char *);
void calc_destroy(struct calc *);
