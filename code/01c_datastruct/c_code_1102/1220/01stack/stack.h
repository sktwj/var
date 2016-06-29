#include "list.h"

//描述栈的类型
struct stack {
	struct list_head head; 

	void (*push)(struct list_head *, struct list_head *); //入栈
	struct list_head *(*pop)(struct stack *); //出栈
	struct list_head *(*top)(struct stack *); //访问栈顶元素
	int (*is_empty)(struct stack *); //判断栈是否为空
};

void stack_init(struct stack *);
void stack_destroy(struct stack *);
