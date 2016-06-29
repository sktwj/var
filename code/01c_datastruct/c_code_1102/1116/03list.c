#include <stdio.h>
#include <stdlib.h>

//有头循环单向链表

typedef int elem_t;
//链表上的节点的类型
struct node {
	elem_t data; //数据域
	struct node *next; //指针域
};

//头插建立单链表
static inline void add(struct node *head, struct node *node)
{
	node->next = head->next;
	head->next = node;
}

//尾插建立单链表
static inline void add_tail(struct node *head, struct node *node)
{
	struct node *cur = head;
	for (; cur->next != head; cur = cur->next) {} //找到最后一个节点
	cur->next = node;
	node->next = head;
}

//遍历
static inline void list_for_each(struct node *head)
{
	struct node *cur = head->next;
	for (; cur != head; cur = cur->next) {
		printf("%d ", cur->data);
	}
	printf("\n");
}

//查找
static inline struct node *search(struct node *head, struct node *node)
{
#if 0
	struct node *cur = head;
	for (; cur->next != node; cur = cur->next) {
		if (cur->next == head) {
			return NULL;
		}
	}

	return  cur->next; //找到了
#endif

#if 0
	struct node *cur = head->next;
	for (; cur != node; cur = cur->next) {
		if (cur == head) {
			return NULL;
		}
	}
	return cur;
#endif

	struct node *cur = head->next;
	for (; cur != head; cur = cur->next) {
		if (cur == node) {
			return cur;
		}
	}
	return NULL;
}

static inline void del(struct node *head, struct node *node)
{
	struct node *tmp = head;
	struct node *cur = head->next;
	for (; cur != node; tmp = cur, cur = cur->next) {
		if (cur == head) {
			return;
		}
	}

	tmp->next = cur->next;
	cur->next = NULL;
}

//只用一个指针实现删除节点
static inline void del_node(struct node *head, struct node *node)
{
	struct node *cur = head;
	for (; cur->next != node; cur = cur->next) {
		if (cur->next == head) {
			return;
		}
	}

	if (cur->next == head) {
		return;
	}
	cur->next = node->next;
	node->next = NULL;
}

//销毁
static inline void destroy(struct node *head)
{
	while (head->next != head) { //说明链表除了头节点之外还有其他节点
		del(head, head->next);
	}
	free(head);
}

int main(void)
{
#if 1
	struct node *head = (struct node *)malloc(sizeof(struct node));
	if (!head) {
		printf("申请头节点空间失败！\n");
		return 1;
	}
	head->next = head; //初始化头节点

	struct node s[10];

	int i = 0;
	for (; i < 10; i++) {
		(s + i)->data = i;
		(s + i)->next = s + i;

		//add(head, s + i);
		add_tail(head, s + i);
	}
	list_for_each(head);

#endif
#if 0
	struct node head = {.next = &head,};

	struct node s[10];

	int i = 0;
	for (; i < 10; i++) {
		(s+i)->data = i;
		(s+i)->next = s+i;

		add(&head, s+i);
	}
	list_for_each(&head);
#endif

	printf("查找：\n");
	struct node *find = search(head, s + 9);
	if (find) {
		printf("找到了，节点的值是：%d\n", find->data);	
	} else {
		printf("没找到！\n");
	}
	
	printf("删除：\n");
	del_node(head, s + 9);
	list_for_each(head);

	destroy(head);

	return 0;
}
