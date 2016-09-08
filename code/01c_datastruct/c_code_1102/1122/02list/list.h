#pragma once

typedef int elem_t;
struct node {
	elem_t data;
	struct node *next;
};

//描述整个链表的对象的类型
struct list {
	//属性
	struct node *head; //头指针
	
	//方法
	void (*add)(struct list *, elem_t);
	void (*add_tail)(struct list *, elem_t);
	void (*list_for_each)(struct list *, void (*)(struct node *));
	struct node *(*search)(struct list *, elem_t);
	void (*del)(struct list *, elem_t);
	void (*destroy)(struct list *); //析构函数
};

void list_init(struct list *); //构造函数，用来初始化链表对象的
