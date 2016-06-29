#include <stdio.h>
#include "list_sort.h"

static inline void swap(struct list_head *a, struct list_head *b)
{
	struct list_head tmp;
	add(&tmp, a, a->next);
	del(a);
	add(a, b, b->next);
	del(b);
	add(b, &tmp, tmp.next);
	del(&tmp);
}

static inline void bubble_sort(struct list_head *head,
	int (*cmp)(struct list_head *, struct list_head *),
	void (*todo)(struct list_head *))
{
	struct list_head *cur, *tmp, *k;
	for (cur = head/*->next*/; cur != head->prev; cur = cur->next) {
		for (tmp = head->prev->prev; tmp != cur/*head*/; tmp = tmp->prev) {
			if (cmp(tmp->next, tmp) < 0) {
				//todo(tmp);
				swap(tmp, tmp->next);
				//todo(tmp);
			
				tmp = tmp->prev;
			}
		}
	
#if 1
		list_for_each(k, head) {
			todo(k);
		}
		printf("\n");	
#endif
	}
}

static inline void select_sort(struct list_head *head,
	int (*cmp)(struct list_head *, struct list_head *),
	void (*todo)(struct list_head *))
{
	struct list_head *cur, *tmp, *save; //save用来指向值最小的节点
	for (cur = head->next; cur != head->prev; cur = cur->next) {
		for (save = cur, tmp = cur->next; tmp != head; tmp = tmp->next) {
			if (cmp(tmp, save) < 0) {
				save = tmp;
			}
		}
		if (save != cur) {
			swap(save, cur);
			cur = save; //指针归位
		}
	}
}

static inline void insert_sort(struct list_head *head,
	int (*cmp)(struct list_head *, struct list_head *),
	void (*todo)(struct list_head *))
{
	struct list_head *cur, *tmp, *save; //save用来指向即将被删除掉的节点
	for (cur = head->next->next; cur != head; cur = cur->next) {
		for (/*save = cur*, */ tmp = cur->prev; tmp != head; tmp = tmp->prev) {

#if 1
			if (cmp(tmp, cur) > 0) {
				del(tmp);
				add(tmp, cur, cur->next);
				//cur = save;
				tmp = cur;
			}
#endif
		}
	}
}

void list_sort_init(struct list_sort *this)
{
	this->swap = swap;
	this->bubble = bubble_sort;
	this->select = select_sort;
	this->insert = insert_sort;
}
