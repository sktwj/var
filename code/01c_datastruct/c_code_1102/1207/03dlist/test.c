#include "list.h"
#include <stdio.h>

int main(void)
{
	struct list list; 
	list_init(&list);

#define CNT 10
	struct node s[CNT] = {
		{0}, {1}, {2}, {3}, {4},
		{5}, {6}, {7}, {8}, {9},
	};

	int i = 0;
	for (; i < CNT; i++) {
		//list.add(&list, s + i);
		list.add_tail(&list, s + i);
	}
	list.for_each(&list);

	printf("search: \n");
	int value;
	if (list.search(&list, s + 19)) {
		printf("该节点的值 ＝ %d\n", (s + 19)->data);
	} else {
		printf("没找到！\n");
	}

	printf("del: \n");
	list.del(&list, s + 10);
	list.for_each(&list);

	list_destroy(&list);

	return 0;
}
