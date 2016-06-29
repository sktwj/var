#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "queue.h"

//描述的是用户的节点
typedef int elem_t;
struct node {
	elem_t data;
	struct list_head list;
};

static inline void free_mem(struct list_head *qnode)
{
	struct node *pnode = container_of(qnode, struct node, list);
	printf("%d ", pnode->data);
	del(&pnode->list);
	free(pnode);
}

int main(void)
{
	struct queue *queue; //定义队列类型指针变量
	queue = (struct queue *)malloc(sizeof(struct queue));
	assert(queue);
	queue_init(queue);
#define CNT 10
	struct node *s[CNT];

	int i = 0;
	for (; i < CNT; i++) {
		*(s + i) = (struct node *)calloc(1, sizeof(struct node));
		assert(*(s + i));
		(*(s + i))->data = i;
		LIST_INIT(&(*(s + i))->list);

		queue->push(&queue->head, &(*(s + i))->list);
	}

	printf("队头元素是：%d\n", 
	container_of(queue->top(queue), struct node, list)->data);

	printf("出队的顺序：");
	while (!queue->is_empty(queue)) {
		//printf("%d ", container_of(queue->pop(queue, free_mem), struct node, list)->data);
		queue->pop(queue, free_mem);
	}
	printf("\n");

	queue_destroy(queue, free_mem);

	return 0;
}
