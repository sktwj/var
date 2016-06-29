#include <stdio.h>
#include "stack.h"

//描述的是用户的节点
typedef int elem_t;
struct node {
	elem_t data;
	struct list_head list;
};

int main(void)
{
	struct stack stack; //定义栈变量
	stack_init(&stack);
#define CNT 10
	struct node s[CNT];

	int i = 0;
	for (; i < CNT; i++) {
		(s + i)->data = i;
		LIST_INIT(&(s + i)->list);

		stack.push(&stack.head, &(s + i)->list);
	}

	printf("栈顶元素是：%d\n", 
	container_of(stack.top(&stack), struct node, list)->data);

	printf("出栈的顺序：");
	while (!stack.is_empty(&stack)) {
		printf("%d ", container_of(stack.pop(&stack), struct node, list)->data);
	}
	printf("\n");

	stack_destroy(&stack);

	return 0;
}
