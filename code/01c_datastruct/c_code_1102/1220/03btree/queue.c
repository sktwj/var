#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "queue.h"

static inline void push(struct queue *queue, qelem_t data)
{
	struct qnode *qnode = (struct qnode *)malloc(sizeof(struct qnode));
	assert(qnode);
	qnode->data = data;
	LIST_INIT(&qnode->list);
	
	list_add_tail(&queue->head, &qnode->list);
}

static inline qelem_t pop(struct queue *queue)
{
	if (queue->is_empty(queue))	{
		exit (EXIT_FAILURE);
	}

	struct qnode *pnode = container_of(queue->head.next, struct qnode, list);
	qelem_t data = pnode->data;
	del(queue->head.next);
	free(pnode);

	return data;
}

static inline qelem_t top(struct queue *queue)
{
	if (!queue->is_empty(queue)) {
		return container_of(queue->head.next, struct qnode, list)->data;
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

void queue_destroy(struct queue *queue)
{
	while (!queue->is_empty(queue)) {
		queue->pop(queue);
	}
}
