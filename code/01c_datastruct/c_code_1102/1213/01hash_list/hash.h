#pragma once
#include <stdio.h>
#include "list.h"

// 哈希链表上的每一个节点的类型
#define NAME_LEN 20
#define CONT_LEN 100
#define ARR_SIZE 3
struct node {
	char key[NAME_LEN]; //关键字
	char content[CONT_LEN];
	struct list_head list;
};

struct hash {
	struct list_head *parr; //表头节点的指针
	
	size_t (*hash_alg)(const char *key); //哈希函数
	void (*build)(struct hash *, char *key, char *content); //建立链表
	void (*for_each)(struct hash *); //遍历链表
	void (*del)(struct hash *, char *key); //删除节点
	struct node *(*search)(struct hash *, char *key); //查找节点
	int (*is_empty)(struct hash *); //判断链表是否为空
};

void hash_init(struct hash *);
void hash_destroy(struct hash *);
