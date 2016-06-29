#include <stdio.h>
#include "queue.h"

static inline void push(struct list_head *head, struct list_head *qnode)
{
	list_add_tail(head, qnode);
}

static inline struct list_head *pop(struct queue *queue, void (*todo)(struct list_head *))
{
	if (queue->is_empty(queue))	{
		return NULL;
	}

	struct list_head *save = queue->head.next;
	//del(queue->head.next);
	todo(save);
	return save;
}

static inline struct list_head *top(struct queue *queue)
{
	if (!queue->is_empty(queue)) {
		return queue->head.next;
	}
}

static inline int queue_is_empty(struct queue *queue)
{
	return list_is_empty(&queue->head);
}

void queue_init(struct queue *queue)
{
	LIST_INIT(&queue->head);

	queue->push = push;
	queue->pop = pop;
	queue->top = top;
	queue->is_empty = queue_is_empty;
}

void queue_destroy(struct queue *queue, void (*todo)(struct list_head *))
{
	while (!queue->is_empty(queue)) {
		queue->pop(queue, todo);
	}
	free(queue);
}
