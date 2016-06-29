#include "list.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

static inline void list_add(struct list *list, elem_t data)
{
	struct node *node = (struct node *)malloc(sizeof(struct node));
	assert(node);

	//初始化新节点
	node->data = data;
	node->next = NULL;

	//插入链表
	node->next = list->head.next;
	list->head.next = node;
}

static inline void list_add_tail(struct list *list, elem_t data)
{
	struct node *node = (struct node *)malloc(sizeof(struct node));
	assert(node);

	//初始化新节点
	node->data = data;
	node->next = NULL;

	struct node *cur = NULL; //cur用来找到最后一个节点
	for (cur = &list->head; cur->next; cur = cur->next) {}
	cur->next = node;
}

static inline void list_for_each(struct list *list, void (*todo)(struct node *))
{
	struct node *cur = list->head.next;
	for (; cur; cur = cur->next) {
		todo(cur);
	}
}	

static inline struct node *search(struct list *list, elem_t data)
{
	struct node *cur = list->head.next;
	for (; cur && cur->data != data; cur = cur->next) {}

	if (!cur) {
		printf("没找到！\n");
		return NULL;
	} else {
		printf("找到了！\n");
		return cur;
	}
}

static inline void del(struct list *list, elem_t data)
{
	/*
	   //试图用一个指针实现：
	struct node *cur = &list->head;
	for (; cur->next && cur->next->data != data; cur = cur->next ) {}

	if (!cur->next) {
		printf("没有符合条件的节点可以被删除！\n");
	} else {
		cur->next = cur->next->next;
		//因为要删除的节点是存放在堆空间的，而我们用一个指针的方式是无法保存要删除的节点的地址的，这和你传入的第二个参数的类型是有关系的；这就导致用一个指针删除节点的想法无法实现了，因为该节点空间因此就会泄露了。
	}
	*/

	//用两个指针实现
	struct node *tmp = &list->head;
	struct node *cur = tmp->next;
	int flag = 0;

loop:
	for (; cur && cur->data != data; tmp = cur, cur = cur->next) {}

	if (cur && cur->data == data) {
		flag = 1;
		tmp->next = cur->next;
		free(cur);
		cur = tmp->next;
		goto loop;
	} else {
		if (!flag) {
			printf("没有符合条件的节点可以被删除！\n");
		}
	}
}	

static inline int list_is_empty(struct list *list) //判断链表是否为空
{
	return !list->head.next;
}

static inline void destroy(struct list *list) //销毁整条链表
{
	while (!list->is_empty(list)) {
		list->del(list, list->head.next->data);
	}
	free(list);
}

void list_init(struct list *list)
{
	list->head.next = NULL; //对表头节点初始化
	list->add = list_add;
	list->add_tail = list_add_tail;
	list->for_each = list_for_each;
	list->search = search;
	list->del = del;
	list->is_empty = list_is_empty;
	list->destroy = destroy;
}
