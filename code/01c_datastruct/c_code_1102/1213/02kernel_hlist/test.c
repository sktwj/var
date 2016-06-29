#include <assert.h>
#include <stdlib.h>
#include "hlist.h"

int main(void)
{
	struct hlist *this = (struct hlist *)malloc(sizeof(struct hlist));
	assert(this);

	hlist_init(this);

#define CNT 10

	struct info {
		char *key;
		char *content;
	};

	struct info s[CNT] = {
		{"hello", "你好"},
		{"welcome", "欢迎"},
		{"china", "瓷器"},
		{"Chinese kongfu", "中国功夫"},
		{"UNIX", "操作系统"},
		{"beijing", "中国首都"},
		{"American", "美国"},
		{"Indian", "印度阿三"},
		{"china", "中国"},
		{"hello", "你好"},
	};

	int i = 0;
	for (; i < CNT; i++) {
		this->build(this, (s + i)->key, (s + i)->content);
	}
	this->for_each(this);

#if 1
	printf("删除：\n");
	this->del(this, "welcome");
	this->for_each(this);
#endif

#if 1
	printf("查找：\n");
	struct node *find = this->search(this, "beijing");
	if (find) {
		printf("找到了，content是：%s\n", find->content);
	} else {
		printf("没找到！\n");
	}
#endif

#if 0
	printf("插入一个新节点到某条链表指定节点之前：\n");
	struct node *new = (struct node *)malloc(sizeof(struct node));
	assert(new);
	//new->key = (char *)malloc(20);
	//assert(new->key);
	//new->content = (char *)malloc(100);
	//assert(new->content);
	new->key = "Japanese";
	new->content = "小日本";

	size_t hash = this->hlist_alg(new->key, this);
	printf("hash = %d\n", hash);

	list_add_before(&new->list, (this->parr + hash)->first);
	this->for_each(this);
#endif

	hlist_destroy(this);

	return 0;
}


