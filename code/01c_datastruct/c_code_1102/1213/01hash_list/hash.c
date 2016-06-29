#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "hash.h"

static inline size_t hash_alg(const char *key)
{
	size_t sum = 0;
	while(*key) {
		sum += *key++;
	}

	return sum % ARR_SIZE;
}

static inline void build_list(struct hash *this, char *key, char *content)
{
	size_t hash_val = this->hash_alg(key);

	struct list_head *cur = NULL;
	struct node *pnode = NULL;
	//判断链表中是否存在key相同的节点
	list_for_each(cur, this->parr + hash_val) {
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
	strcpy(node->key, key);
	strcpy(node->content, content);

	list_add(this->parr + hash_val, &node->list);
}

static inline void print_list(struct hash *this) 
{
	int i = 0;
	struct node *ptr = NULL;
	for (; i < ARR_SIZE; i++) {
		printf("第%d条链表：", i);
		list_for_each_entry(ptr, this->parr + i, list) {
			printf("%s: %s --> ", ptr->key, ptr->content);
		}
		printf("\n");
	}
}

static inline void del_node(struct hash *this, char *key)
{
	size_t hash_val = this->hash_alg(key);

	struct node *ptr = NULL;
	struct list_head *cur = NULL, *tmp = NULL;

#if 0
	//下面这一段代码的问题在于：释放的节点的空间中的struct_list 类型的指针的部分已经被删除了，释放其所在的结构体的堆空间会产生内存非法访问的问题（用valgrind检测得到的提示是invalid read of size 1）
	struct list_head *save = NULL;
	list_for_each_safe(cur, tmp, this->parr + hash_val) {
		ptr = hlist_entry(cur, struct node, list);
		//printf("ptr = %p, cur = %p\n", ptr, cur);
		if (!strcmp(ptr->key, key)) {
			
			save = cur;
			cur->prev->next = cur->next;
			cur->next->prev = cur->prev;
			
			ptr = hlist_entry(save, struct node, list);
			//printf("ptr = %p, save = %p\n", ptr, save);
			free(ptr);
		}
	}
#endif

#if 1
	list_for_each_entry_safe(ptr, tmp, this->parr + hash_val, list)	{
		if (!strcmp(ptr->key, key)) {
			del(&ptr->list);
			free(ptr);
		}
	}
#endif
}

static inline struct node *search(struct hash *this, char *key)
{
	size_t hash_val = this->hash_alg(key);

	struct node *ptr = NULL;
	struct list_head *cur = NULL, *tmp = NULL;
	list_for_each_safe(cur, tmp, this->parr + hash_val) {
		ptr = hlist_entry(cur, struct node, list);
		if (!strcmp(ptr->key, key)) {
			return ptr;
		}
	}		

	return NULL;
}

static inline int hash_is_empty(struct hash *this)
{
	return !this->parr;
}

void hash_destroy(struct hash *this)
{
	int i = 0;
	struct node *ptr = NULL;
	struct list_head *tmp = NULL;
	for (; i < ARR_SIZE; i++) {
		list_for_each_entry_safe(ptr, tmp, this->parr + i, list) {
			//this->del(this, ptr->key); //调用我们写的这个函数有点小问题
			
			del(&ptr->list);
			free(ptr);
		}
	}
	
	free(this->parr);
}

void hash_init(struct hash *this)
{
	this->parr = (struct list_head *)malloc(sizeof(struct list_head) * ARR_SIZE);
	assert(this->parr);

	//初始化表头
	int i = 0;
	for (; i < ARR_SIZE; i++) {
		LIST_INIT(this->parr + i);
	}

	this->hash_alg = hash_alg;
	this->build = build_list;
	this->for_each = print_list;
	this->del = del_node;
	this->search = search;
}
