#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "hlist.h"

static inline size_t hlist_alg(const char *key, struct hlist *this)
{
	size_t sum = 0;
	while(*key) {
		sum += *key++;
	}

	return sum % this->arr_size;
}

static inline void build_list(struct hlist *this, char *key, char *content)
{
	size_t hash_val = this->hlist_alg(key, this);

	struct hlist_node *cur = NULL;
	struct node *pnode = NULL;
	//判断链表中是否存在key相同的节点
	hlist_for_each(cur, this->parr + hash_val) {
		pnode = hlist_entry(cur, struct node, list);
		if (!strcmp(key, pnode->key)) { //key相同
			if (!strcmp(content, pnode->content)) {
				return; //key和content都相同，不插入
			} else {	
				strcat(pnode->content, content); //content不同，合并为一个节点
				return;
			}
		}
	}
		
	//key和content都不相同，作为一个新节点插入链表
	struct node *node = (struct node *)malloc(sizeof(struct node));
	assert(node);
	node->key = (char *)malloc(20);
	assert(node->key);
	strcpy(node->key, key);
	node->content = (char *)malloc(100);
	assert(node->content);
	strcpy(node->content, content);

	list_add(this->parr + hash_val, &node->list);
}

static inline void print_list(struct hlist *this) 
{
	int i = 0;
	struct node *ptr = NULL;
	struct hlist_node *cur = NULL;
	for (; i < this->arr_size; i++) {
		printf("第%d条链表：", i);
		hlist_for_each_entry(ptr, cur, this->parr + i, list) {
			printf("%s: %s --> ", ptr->key, ptr->content);
		}
		printf("\n");
	}
}

static inline void del_node(struct hlist *this, char *key)
{
	size_t hash_val = this->hlist_alg(key, this);

	struct node *ptr = NULL;
	struct hlist_node *cur = NULL, *tmp = NULL;

	hlist_for_each_entry_safe(ptr, cur, tmp, this->parr + hash_val, list)	{
		if (!strcmp(ptr->key, key)) {
			list_del(&ptr->list);
			free(ptr->key);
			free(ptr->content);
			free(ptr);
		}
	}
}

static inline struct node *search(struct hlist *this, char *key)
{
	size_t hash_val = this->hlist_alg(key, this);

	struct node *ptr = NULL;
	struct hlist_node *cur = NULL, *tmp = NULL;
	hlist_for_each_safe(cur, tmp, this->parr + hash_val) {
		ptr = hlist_entry(cur, struct node, list);
		if (!strcmp(ptr->key, key)) {
			return ptr;
		}
	}		

	return NULL;
}

static inline int hlist_is_empty(struct hlist *this)
{
	return !this->parr;
}

#if 1
void hlist_destroy(struct hlist *this)
{
	int i = 0;
	struct node *ptr = NULL;
	struct hlist_node *cur = NULL, *tmp = NULL;
	for (; i < this->arr_size; i++) {
		hlist_for_each_entry_safe(ptr, cur, tmp, this->parr + i, list) {
			list_del(&ptr->list);
			free(ptr->key);
			free(ptr->content);
			free(ptr);
		}
	}
	
	free(this->parr);
	free(this);
}
#endif

static inline int single_list_is_empty(struct hlist *this, int i)
{
	return !((this->parr + i)->first);
}

#if 0
//使用下面的这个销毁函数也可以
void hlist_destroy(struct hlist *this)
{
	int i = 0;
	struct node *ptr = NULL;
	struct hlist_node *cur = NULL, *tmp = NULL;
	for (; i < this->arr_size; i++) {
		hlist_for_each_entry_safe(ptr, cur, tmp, this->parr + i, list) {
			if (!single_list_is_empty(this, i)) {
				ptr = hlist_entry((this->parr + i)->first, struct node, list);
				list_del((this->parr + i)->first);
				free(ptr->key);
				free(ptr->content);
				free(ptr);
			}
		}
	}
	free(this->parr);
	free(this);
}
#endif

void hlist_init(struct hlist *this)
{
	this->arr_size = 3;
	
	this->parr = (struct hlist_head *)malloc(sizeof(struct hlist_head) * this->arr_size);
	assert(this->parr);

	//初始化表头
	int i = 0;
	for (; i < this->arr_size; i++) {
		INIT_HLIST_HEAD(this->parr + i);
	}

	this->hlist_alg = hlist_alg;
	this->build = build_list;
	this->for_each = print_list;
	this->del = del_node;
	this->search = search;
}


