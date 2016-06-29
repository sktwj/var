#include <stdio.h>
#include "list.h"

static inline void print_node(struct node *node)
{
	printf("%d ", node->data);
}

int main(void)
{	
	struct list list; //在stack内
	list_init(&list);

	int i = 0;
	for (; i < 10; i++) {
		list.add(&list, i);
		//list.add_tail(&list, i);
	}

	list.add(&list, 0);
	list.add(&list, 0);
	list.list_for_each(&list, print_node);

	printf("search: \n");
	if (list.search(&list, 9)) {
		printf("找到了！\n");
	} else {
		printf("没找到！\n");
	}

	printf("del: \n");
	list.del(&list, 5);
	list.list_for_each(&list, print_node);

	list.destroy(&list);

	return 0;
}
