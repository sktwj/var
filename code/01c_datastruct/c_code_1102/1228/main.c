#include <stdio.h>
#include "list_sort.h"

typedef int elem_t;
struct node {
	elem_t data;
	struct list_head list;
};

static inline int cmp_data(struct list_head *a, struct list_head *b)
{
	struct node *pa = container_of(a, struct node, list);
	struct node *pb = container_of(b, struct node, list);

	return pa->data - pb->data;
}

static inline void print_node(struct list_head *lnode)
{
	struct node *pnode = container_of(lnode, struct node, list);
	//printf("%d\n", pnode->data);
	printf("%d ", pnode->data);
}

int main(void)
{
	LIST_HEAD;

	struct list_sort sort;
	list_sort_init(&sort);

	//elem_t t[] = {7,0,9,2,8,6,3,1,13,4,5};
	elem_t t[] = {9,0,2,1,6,3,1,-3,4,5};
	int n = sizeof(t)/sizeof(elem_t);
	struct node s[n];
	
	int i = 0;
	for (; i < n; i++) {
		(s + i)->data = *(t + i);
		LIST_INIT(&(s+i)->list);
		list_add_tail(&head, &(s + i)->list);
	}
	struct node *pnode = NULL;
	list_for_each_entry(pnode, &head, list) {
		printf("%d ", pnode->data);
	}
	printf("\n");

#if 0
	printf("after swap: \n");
	sort.swap(&(s + 2)->list, &(s + 7)->list);
	list_for_each_entry(pnode, &head, list) {
		printf("%d ", pnode->data);
	}
	printf("\n");
#endif

	printf("after sort: \n");
	sort.bubble(&head, cmp_data, print_node);
	//sort.select(&head, cmp_data, print_node);
	//sort.insert(&head, cmp_data, print_node);
	list_for_each_entry(pnode, &head, list) {
		printf("%d ", pnode->data);
	}
	printf("\n");

	return 0;
}
