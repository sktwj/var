#include <stdio.h>
#include <stdlib.h>

//无头非循环单向链表

typedef int elem_t;
struct node {
	elem_t data;
	struct node *next;
};

//头插
static inline void add(struct node **head, elem_t data)
{
	struct node *node = (struct node *)malloc(sizeof(struct node));
	if (!node) {
		printf("开辟节点堆空间失败！\n");
		exit (1);
	}
	node->data = data;
	node->next = NULL;

	//判断链表是否为空，插入的是第一个节点
	if (*head == NULL) {
		*head = node;
	} else {
		node->next = *head;
		*head = node;
	}
}

static inline void add_tail(struct node **head, elem_t data)
{
	struct node *node = (struct node *)malloc(sizeof(struct node));
	if (!node) {
		printf("开辟节点堆空间失败！\n");
		exit (1);
	}
	node->data = data;
	node->next = NULL;

	//判断链表是否为空，插入的是第一个节点
	if (*head == NULL) {
		*head = node;
	} else {
		struct node *cur = *head;
		for (; cur->next; cur = cur->next) {} //找到最后一个节点
		cur->next = node;
	}
}

static inline void list_for_each(struct node *head)
{
	struct node *cur = head;
	for (; cur; cur = cur->next) {
		printf("%d ", cur->data);
	}
	printf("\n");
}

static inline struct node *search(struct node *head, elem_t data)
{
	if (!head) {
		return NULL;
	}

	struct node *cur = head;
	for (; cur; cur = cur->next) {
		if (cur->data == data) {
			return cur;
		}
	}

	return NULL;
}

static inline void del(struct node **head, elem_t data)
{
	if (!*head) {
		return;
	}
	
	struct node *tmp = NULL, *cur = *head;
	while (cur) {
		if (cur->data == data) { //当前节点的值和要删除的值相同
			if (cur == *head) {//要删除的节点是第一个节点
				*head = cur->next;
				cur->next = NULL;
				free(cur);
				cur = *head;
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

static inline void destroy(struct node **head)
{
	while (*head) { //链表不为空时，执行删除
		del(head, (*head)->data);
	}
}

int main(void)
{	
	struct node *head = NULL; //头指针

	int i = 0;
	for (; i < 10; i++) {
		add(&head, i);
		//add_tail(&head, i);
	}
	add(&head, 0);
	add(&head, 0);
	list_for_each(head);

	printf("search: \n");
	if (search(head, 9)) {
		printf("找到了！\n");
	} else {
		printf("没找到！\n");
	}

#if 0
	printf("del: \n");
	del(&head, 5);
	list_for_each(head);
#endif

	destroy(&head);

	return 0;
}
