#include <stdio.h>
#include "stack.h"

int main(void)
{
	struct stack stack; //定义栈变量
	stack_init(&stack);
#define CNT 10
	struct snode s[CNT];

	int i = 0;
	for (; i < CNT; i++) {
		(s + i)->data = i;
		LIST_INIT(&(s + i)->list);

		stack.push(&stack, (s + i)->data);
	}

	printf("栈顶元素是：%d\n", stack.top(&stack));

	printf("出栈的顺序：");
	while (!stack.is_empty(&stack)) {
		printf("%d ", stack.pop(&stack));
	}
	printf("\n");

	stack_destroy(&stack);

	return 0;
}
