#include <stdio.h>
#include "stack.h"

static inline void push(struct list_head *head, struct list_head *snode)
{
	list_add(head, snode);
}

static inline struct list_head *pop(struct stack *stack)
{
	if (stack->is_empty(stack))	{
		return NULL;
	}

	struct list_head *save = stack->head.next;
	del(stack->head.next);
	return save;
}

static inline struct list_head *top(struct stack *stack)
{
	if (!stack->is_empty(stack)) {
		return stack->head.next;
	}
}

static inline int stack_is_empty(struct stack *stack)
{
	return list_is_empty(&stack->head);
}

void stack_init(struct stack *stack)
{
	LIST_INIT(&stack->head);

	stack->push = push;
	stack->pop = pop;
	stack->top = top;
	stack->is_empty = stack_is_empty;
}

void stack_destroy(struct stack *stack)
{
	while (!stack->is_empty(stack)) {
		stack->pop(stack);
	}
}
