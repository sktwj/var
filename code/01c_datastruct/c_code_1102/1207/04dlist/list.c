#include "list.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

static inline void node_init(struct node *node)
{
	node->prev = node;
	node->next = node;
}

static inline void __add(struct node *node, struct node *prev, struct node *next)
{
	prev->next = node;
	next->prev = node;
	node->next = next;
	node->prev = prev;
}

static inline void add(struct list *list, struct node *node)
{
	__add(node, list->head, list->head->next);
}

static inline void add_tail(struct list *list, struct node *node)
{
	__add(node, list->head->prev, list->head);
}

//该函数的返回值用来判断是否查找成功，为0表示失败，为1表示成功
static inline int search(struct list *list, struct node *node)
{
	struct node *cur = list->head->next;

	for (; cur != list->head; cur = cur->next) {
		if (cur == node) { //找到了
			return 1;
		}
	}

	return 0; //没找到
}

static inline void del(struct list *list, struct node *node)
{
	struct node *cur = list->head->next;
	while (cur != list->head && cur != node) {
			cur = cur->next;
	}
	if (cur == list->head) {
		printf("没有找到要删除的节点！\n");
		return;
	}

	node->prev->next = node->next;
	node->next->prev = node->prev;

	node_init(node);
}

static inline void list_for_each(struct list *list, 
		void (*todo)(struct node *))
{
	struct node *cur = list->head->next;

	for (; cur != list->head; cur = cur->next) {
		todo(cur);
	}
}	

static inline int list_is_empty(struct list *list)
{
	return list->head->next == list->head;
}

void list_init(struct list *list)
{
	list->head = (struct node *)malloc(sizeof(struct node));
	assert(list->head);

	list->node_init = node_init;
	list->node_init(list->head); //初始化表头节点

	list->add = add;
	list->add_tail = add_tail;
	list->for_each = list_for_each;
	list->search = search;
	list->del = del;
	list->is_empty = list_is_empty;
}

void list_destroy(struct list *list)
{
	while (!list->is_empty(list)) {
		list->del(list, list->head->next);
	}
	free(list->head);
}

