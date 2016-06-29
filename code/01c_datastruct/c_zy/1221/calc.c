#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "calc.h"

//进行运算的函数
static inline void compute(struct calc *this)
{
	num_t b = this->num_stack->pop(this->num_stack);
	num_t a = this->num_stack->pop(this->num_stack);

	char opr = this->opr_stack->pop(this->opr_stack);

	num_t ret;
	switch (opr) {
	case '+':
		ret = a + b;
		break;
	case '-':
		ret = a - b;
		break;
	case '*':
		ret = a * b;
		break;
	case '/':
		ret = a / b;
		break;
	case '%':
		ret = a % b;
		break;
	}

	printf("%d %c %d = %d\n", a, opr, b, ret); //打印每一步的中间结果
	this->num_stack->push(this->num_stack, ret); //将计算结果压入数字栈
}

static inline void num_handler(struct calc *this)
{
	//处理个位数字
	//this->num_stack->push(this->num_stack, *this->ptr - '0');

	//处理多位数字
	num_t tmp = 0;
	for (; *this->ptr >= '0' && *this->ptr <= '9'; this->ptr++) {
		tmp = tmp*10 + *this->ptr - '0';
	}
	this->ptr--;

	this->num_stack->push(this->num_stack, tmp);

	this->flag = NUM;
}

static inline int priority(char c)
{
	int level = 0;
	switch (c) {
	case '+':
	case '-':
		return level + 1;
	case '*':
	case '/':
	case '%':
		return level + 2;
	case '(':
	case ')':
		return level;
	default:
		return level;
	}
}

static inline void opr_handler(struct calc *this)
{
#if 0
	if ( ((*this->ptr == '-') && (this->flag == OPR))
		|| ((*this->ptr == '+') && (this->flag == OPR)) ) { //如果当前字符是'-'或'+', 并且上一次处理的是符号
		this->num_stack->push(this->num_stack, 0);
		this->opr_stack->push(this->opr_stack, *this->ptr);
		return;
	} 
#endif

#if 1
	if ((*this->ptr == '+') || (*this->ptr == '-')) {
			if (this->flag == OPR) {
				this->num_stack->push(this->num_stack, 0);
				this->opr_stack->push(this->opr_stack, *this->ptr);
				return;
			} 
	}
#endif

	//如果符号栈为空或者如果当前ptr指针指向的字符的优先级大于符号栈栈顶元素的优先级时，当前ptr指针指向的字符无条件入栈
	if ((this->opr_stack->is_empty(this->opr_stack)) || (priority(*this->ptr) >= priority(this->opr_stack->top(this->opr_stack)))) { //当前ptr指针指向的字符入栈
		this->opr_stack->push(this->opr_stack, *this->ptr);
	} else {
		compute(this);
	}
			
	this->flag = OPR;
}

static inline void bracket_handler(struct calc *this)
{
	if (*this->ptr == '(') { //如果当前ptr指针指向的字符是'('，当前*ptr无条件入栈
	
		this->opr_stack->push(this->opr_stack, *this->ptr);
		this->flag = OPR;	
	} else { //当前是')'，')'不入栈，出栈运算，直到遇到'('为止，之后'('出栈
		while (this->opr_stack->top(this->opr_stack) != '(') {
			compute(this); //出栈运算
		}
		this->opr_stack->pop(this->opr_stack); //左括号出栈
	}
}

static inline num_t analyze_expr(struct calc *this, const char *expr)
{
	for (; *this->ptr; this->ptr++) {
		switch (*this->ptr) {
			case '0' ... '9':
				num_handler(this); //进入数字处理函数
				break;
			case '+':
			case '-':
			case '*':
			case '/':
			case '%':
				opr_handler(this); //进入符号处理函数
				break;
			case '(':
			case ')':
				bracket_handler(this); //进入括号处理函数
				break;
			default:
				;
		}
	}

	//当符号栈为空时，得到处理结果
	while (!this->opr_stack->is_empty(this->opr_stack)) { //符号栈不为空
		compute(this); //进行运算
	}

	return this->num_stack->top(this->num_stack); //返回数字栈栈顶元素的值
}

struct calc *calc_init(struct calc *this, const char *expr)
{
	this = (struct calc *)malloc(sizeof(struct calc));
	assert(this);

	this->expr = expr;
	this->ptr = expr;
	this->flag = OPR;

	this->num_stack = (struct stack *)malloc(sizeof(struct stack));
	assert(this->num_stack);
	this->opr_stack = (struct stack *)malloc(sizeof(struct stack));
	assert(this->opr_stack);
	stack_init(this->num_stack);
	stack_init(this->opr_stack);

	this->analyze = analyze_expr;
}

void calc_destroy(struct calc *this)
{
	stack_destroy(this->num_stack);
	stack_destroy(this->opr_stack);
	free(this->num_stack);
	free(this->opr_stack);

	free(this);
}
