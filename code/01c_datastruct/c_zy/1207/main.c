#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "list.h"

struct info {
	char *name;
	int age;
};

static inline void print_node(struct node *node)
{
	printf("%10s: %-d\n", 
			((struct info *)(node->par))->name,
			((struct info *)(node->par))->age);
}

static inline int cmp_value(struct node *a, struct node *b)
{
	return (!strcmp(((struct info *)(a->par))->name, ((struct info *)(b->par))->name)) 
		&& (((struct info *)(a->par))->age == 
		((struct info *)(b->par))->age);
}

int main(void)
{
	struct list list;
	list_init(&list);

#define CNT 10
	struct info s[] = {
		{"tom", 20},{"mike", 21},
		{"rose", 20},{"candy", 22},
		{"fred", 24},{"katemoss", 19},
		{"henry", 20},{"lili", 21},
		{"jimmy", 23},{"kate", 20},
	};

	int i = 0;
	struct node *p1, *p2 = NULL;
	for (; i < CNT; i++) {
		//list.add(&list, s + i, sizeof(struct info));
		p1 = list.add_tail(&list, s + i, sizeof(struct info));
		if (i == 9) {
			p2 = p1;
		}
	}
	list.for_each(&list, print_node);

	printf("\nsearch: \n");
	struct node *psnode = (struct node *)malloc(sizeof(struct node) + sizeof(struct info));
	assert(psnode);
	struct info sn = {"tom", 20};
	memcpy(psnode->par, &sn, sizeof(sn));

	if (list.search(&list, psnode, cmp_value)) {
		printf("找到了！\n");
	} else {
		printf("没找到！\n");
	}

	printf("\ndel: \n");
	list.del(&list, p2);
	list.for_each(&list, print_node);

	list.destroy(&list);
	free(psnode);

	return 0;
}
