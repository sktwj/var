#include "list.h"

//链表排序
struct list_sort {
	
	void (*swap)(struct list_head *, struct list_head *);

	void (*bubble)(struct list_head *, int (*cmp)(struct list_head *, struct list_head *), void (*)(struct list_head *));
	void (*select)(struct list_head *, int (*cmp)(struct list_head *, struct list_head *), void (*)(struct list_head *));
	void (*insert)(struct list_head *, int (*cmp)(struct list_head *, struct list_head *), void (*)(struct list_head *));
};

void list_sort_init(struct list_sort *);
