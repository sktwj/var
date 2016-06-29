#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "list.h"

static inline void __add(struct node *node, struct node *prev, struct node *next)
{
	node->next = next;
	node->prev = prev;
	next->prev = node;
   	prev->next = node;	
}

static inline struct node *add(struct list *list, void *src, size_t size)
{
	struct node *pnode = (struct node *)malloc(sizeof(struct node) + size);
	assert(pnode);
	memcpy(pnode->par, src, size);

	__add(pnode, &list->head, list->head.next);

	return pnode;
}

static inline struct node *add_tail(struct list *list, void *src, size_t size)
{
	struct node *pnode = (struct node *)malloc(sizeof(struct node) + size);
	assert(pnode);
	memcpy(pnode->par, src, size);

	__add(pnode, list->head.prev, &list->head);

	return pnode;
}

static inline void node_init(struct node *node)
{
	node->prev = node;
	node->next = node;
}

static inline void list_for_each(struct list *list, void (*todo)(struct node *))
{
	struct node *cur = list->head.next;
	for (; cur != &list->head; cur = cur->next) {
		todo(cur);
	}
}

static inline int search(struct list *list, struct node *node, int (*cmp)(struct node *, struct node *))
{
	struct node *cur = list->head.next;
	for (; cur != &list->head; cur = cur->next) {
		if (cmp(cur, node)) { //比较当前节点和传入的参数的数据部分是否完全相等
			return 1;
		}
	}

	return 0; //没找到
}

static inline void del(struct list *list, struct node *node)
{
	struct node *cur = &list->head;
	do {
		if (cur->next == node) {
			cur->next = node->next;
			node->next = node;
			free(node);
		} else {
			cur = cur->next;
		}
	} while (cur != &list->head);
}

static inline int list_is_empty(struct list *list)
{
	return list->head.next == &list->head;
}

static inline void list_destroy(struct list *list)
{
	while(!list->is_empty(list)) {
		list->del(list, list->head.next);
	}
}

void list_init(struct list *list)
{
	node_init(&list->head); //初始化表头节点

	list->node_init = node_init;
	list->add = add;
	list->add_tail = add_tail;
	list->for_each = list_for_each;
	list->search = search;
	list->del = del;
	list->is_empty = list_is_empty;
	list->destroy = list_destroy;
}
