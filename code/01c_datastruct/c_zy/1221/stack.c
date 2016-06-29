#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "stack.h"

static inline void push(struct stack *stack, selem_t data)
{
	struct snode *snode = (struct snode *)malloc(sizeof(struct snode));
	assert(snode);
	snode->data = data;
	LIST_INIT(&snode->list);
	
	list_add(&stack->head, &snode->list);
}

static inline selem_t pop(struct stack *stack)
{
	if (stack->is_empty(stack))	{
		exit (EXIT_FAILURE);
	}

	struct snode *pnode = container_of(stack->head.next, struct snode, list);
	selem_t data = pnode->data;
	del(stack->head.next);
	free(pnode);

	return data;
}

static inline selem_t top(struct stack *stack)
{
	if (!stack->is_empty(stack)) {
		return container_of(stack->head.next, struct snode, list)->data;
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
