#pragma once

struct node {
	struct node *prev;
	struct node *next;
	char par[];
};

struct list {
	struct node head; //头节点
	void (*node_init)(struct node *);
	struct node *(*add)(struct list *, void *src, size_t size);
	struct node *(*add_tail)(struct list *, void *src, size_t size);
	void (*for_each)(struct list *, void (*)(struct node *));
	int (*search)(struct list *, struct node *, 
			int (*cmp)(struct node *, struct node *));
	void (*del)(struct list *, struct node *); 
	int (*is_empty)(struct list *);
	void (*destroy)(struct list *);
};

void list_init(struct list *);
