#pragma once

//有头非循环单向链表

typedef int elem_t;
struct node {
	elem_t data;
	struct node *next;
};

//描述整条链表的
struct list {
	struct node head; //数据成员（成员数据，属性）
	//函数成员（成员函数，方法）
	void (*add)(struct list *, elem_t);
	void (*add_tail)(struct list *, elem_t);
	void (*for_each)(struct list *, void (*)(struct node *));
	struct node *(*search)(struct list *, elem_t);
	void (*del)(struct list *, elem_t);
	
	int (*is_empty)(struct list *); //判断链表是否为空
	void (*destroy)(struct list *); //销毁整条链表
};

void list_init(struct list *); //初始化链表

