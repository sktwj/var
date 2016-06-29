#include "list.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <assert.h>

#define LEN 10
struct node {
	char name[LEN];
	int score;
	struct list_head list;
};

int main(void)
{
	LIST_HEAD; //定义链表的表头节点

	struct node *ps[LEN];
	int i = 0;
	for (; i < LEN; i++) {
		*(ps + i) = malloc(sizeof(struct node));
		assert(*(ps + i));
		sprintf((*(ps + i))->name, "%s%d", "tom", i);
	  	(*(ps + i))->score = rand()%100 + 1;
		LIST_INIT(&(*(ps + i))->list);

		//list_add(&head, &(*(ps + i))->list);
		list_add_tail(&head, &(*(ps + i))->list);
	}
	
#if 0
	struct node *ps = (struct node *)malloc(sizeof(struct node) * LEN);*/
	assert(ps);

	time_t t = time(NULL);
	srand(t);
	int i = 0;
	for (; i < LEN; i++) {
		ps + i = malloc(sizeof(struct node));
		assert(ps + i);
		sprintf((ps + i)->name, "%s%d", "tom", i);
	  	(ps + i)->score = /*rand()%100 + 1*/ i;
		LIST_INIT(&(ps + i)->list);

		list_add(&head, &(ps + i)->list);
		//list_add_tail(&head, &(ps + i)->list);
	}
#endif

	struct node *ptr = NULL;
	struct list_head *cur = NULL;
	i = 0;
#if 1
	list_for_each(cur, &head) {
		ptr = container_of(cur, struct node, list);
		printf("name = %s, \t score = %d\n", 
				//ptr->name, ptr->score);
				ptr->name, ptr->score);
		
		i++;
	}
#endif

	printf("after del score < 60: \n");
	struct list_head *tmp = NULL;
	list_for_each_safe(cur, tmp, &head) {
		ptr = list_entry(cur, struct node, list);
		if (ptr->score < 60) {
			del(cur);
			free(ptr);
		}
	}
	
	list_for_each_entry(ptr, &head, list) {
		printf("name = %s, \t score = %d\n", 
				ptr->name, ptr->score);
	}

	printf("del all node: \n");
	struct node *ptmp = NULL;
	//list_for_each_entry_safe(ptr, tmp, &head, list) {
	list_for_each_entry_safe_2(ptr, ptmp, &head, list) {
		del(&(ptr->list));
		free(ptr);
	}
	
	list_for_each_entry(ptr, &head, list) {
		printf("name = %s, \t score = %d\n", 
				ptr->name, ptr->score);
	}

	return 0;
}
