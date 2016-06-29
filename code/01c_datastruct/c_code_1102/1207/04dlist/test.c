#include "list.h"
#include <stdio.h>

static void print_node(struct node *node) 
{
	printf("%d ", *(node->p));
}

int main(void)
{
	struct list list; 
	list_init(&list);

	typedef int elem_t;
#define CNT 10

	struct node s[CNT];

	elem_t t[CNT] = {
		0, 1, 2, 3, 4,
		5, 6, 7, 8, 9,
	};

	int i = 0;
	for (; i < CNT; i++) {
		list.node_init(s + i);
		(s + i)->p = t + i;
		//list.add(&list, s + i);
		list.add_tail(&list, s + i);
	}
	list.for_each(&list, print_node);
	printf("\n");

	printf("search: \n");
	int value;
	if (list.search(&list, s + 19)) {
		printf("该节点的值 ＝ %d\n", (s + 19)->p);
	} else {
		printf("没找到！\n");
	}

	printf("del: \n");
	list.del(&list, s + 10);
	list.for_each(&list, print_node);
	printf("\n");

	list_destroy(&list);

	return 0;
}
