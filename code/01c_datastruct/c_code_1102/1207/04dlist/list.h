#pragma once

typedef int elem_t;
struct node {
	struct node *prev; //指向前一个节点的指针
	struct node *next; //指向后一个节点的指针
	elem_t *p;
};

struct list {
	struct node *head; //表头节点的指针

	void (*node_init)(struct node *);
	void (*add)(struct list *, struct node *);
	void (*add_tail)(struct list *, struct node *);
	void (*for_each)(struct list *, void (*)(struct node *));
	int (*search)(struct list *, struct node *);
	void (*del)(struct list *, struct node *);
	int (*is_empty)(struct list *);
};

void list_init(struct list *);
void list_destroy(struct list *);
