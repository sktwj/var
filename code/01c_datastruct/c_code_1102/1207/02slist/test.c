#include "list.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

static void print_node(struct node *node)
{
	printf("%d ", node->data);
}

int main(void)
{
	struct list *list = (struct list *)calloc(1, sizeof(struct list));
	assert(list);

	list_init(list); //初始化链表对象

#define CNT 10
	int i = 0;
	for (; i < CNT; i++) {
		//list->add(list, i);
		if (i == 5) {
			list->add_tail(list, 0);
		}
		list->add_tail(list, i);
	}
	list->add_tail(list, 0);
	list->for_each(list, print_node);
	printf("\n");

	printf("search: \n");
	list->search(list, 10);

	printf("del: \n");
	list->del(list, 10);
	list->for_each(list, print_node);
	printf("\n");

	list->destroy(list);

	return 0;
}

