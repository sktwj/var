#pragma once
#include <stdio.h>
#include "list.h"

struct node {
	char *key;
	char *content;
	struct hlist_node list;
};

struct hlist {
	struct hlist_head *parr;
	size_t arr_size;

	size_t (*hlist_alg)(const char *, struct hlist *);
	void (*build)(struct hlist *, char *, char *);
	void (*for_each)(struct hlist *);
	struct node *(*search)(struct hlist *, char *);
	void (*del)(struct hlist *, char *);
	int (*is_empty)(struct hlist *);
};

void hlist_init(struct hlist *);
void hlist_destroy(struct hlist *);

