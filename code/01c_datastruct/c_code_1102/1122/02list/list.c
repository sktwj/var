#include <stdio.h>
#include <stdlib.h>
#include "list.h"

//头插
static inline void add(struct list *list, elem_t data)
{
	struct node *node = (struct node *)malloc(sizeof(struct node));
	if (!node) {
		printf("开辟节点堆空间失败！\n");
		exit (1);
	}
	node->data = data;
	node->next = NULL;

	//判断链表是否为空，插入的是第一个节点
	if (list->head == NULL) {
		list->head = node;
	} else {
		node->next = list->head;
		list->head = node;
	}
}

static inline void add_tail(struct list *list, elem_t data)
{
	struct node *node = (struct node *)malloc(sizeof(struct node));
	if (!node) {
		printf("开辟节点堆空间失败！\n");
		exit (1);
	}
	node->data = data;
	node->next = NULL;

	//判断链表是否为空，插入的是第一个节点
	if (list->head == NULL) {
		list->head = node;
	} else {
		struct node *cur = list->head;
		for (; cur->next; cur = cur->next) {} //找到最后一个节点
		cur->next = node;
	}
}

static inline void list_for_each(struct list *list, void (*todo)(struct node *))
{
	struct node *cur = list->head;
	for (; cur; cur = cur->next) {
		//printf("%d ", cur->data);
		todo(cur);
	}
	printf("\n");
}

static inline struct node *search(struct list *list, elem_t data)
{
	if (!list->head) {
		return NULL;
	}

	struct node *cur = list->head;
	for (; cur; cur = cur->next) {
		if (cur->data == data) {
			return cur;
		}
	}

	return NULL;
}

static inline void del(struct list *list, elem_t data)
{
	if (!list->head) {
		return;
	}
	
	struct node *tmp = NULL, *cur = list->head;
	while (cur) {
		if (cur->data == data) { //当前节点的值和要删除的值相同
			if (cur == list->head) {//要删除的节点是第一个节点
				list->head = cur->next;
				cur->next = NULL;
				free(cur);
				cur = list->head;
			} else {//要删除的是除第一个节点之外的其他节点
				tmp->next = cur->next;
				cur->next = NULL;
				free(cur);
				cur = tmp->next;
			}
		} else {
			tmp = cur;
			cur = cur->next;
		}
	}
}

static inline void destroy(struct list *list)
{
	while (list->head) { //链表不为空时，执行删除
		del(list, (list->head)->data);
	}
}

void list_init(struct list *list)
{
	list->head = NULL;
	list->add = add;
	list->add_tail = add_tail;
	list->list_for_each = list_for_each;
	list->search = search;
	list->del = del;
	list->destroy = destroy;
}
