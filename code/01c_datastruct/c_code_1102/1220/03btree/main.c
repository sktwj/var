#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include "btree.h"

typedef int elem_t;
struct node {
	elem_t data;
	struct bnode bnode;
};

static inline int cmp_data(struct bnode *a, struct bnode *b)
{
	struct node *pa = container_of(a, struct node, bnode);
	struct node *pb = container_of(b, struct node, bnode);
	return pa->data - pb->data;
}

static inline void print_node(struct bnode *btree_node)
{
	printf("%d ", container_of(btree_node, struct node, bnode)->data);
}

int main(void)
{
	struct btree btree;
	btree_init(&btree);

	elem_t arr[] = {29, 15, 71, 6, 26, 17, 43, 78, 90, 44, 65};

#define CNT sizeof(arr)/sizeof(elem_t)
	struct node *ps = (struct node *)malloc(sizeof(struct node) * CNT);
	assert(ps);
	int i = 0;
	for (; i < CNT; i++) {
		(ps + i)->data = *(arr + i);
		btree.bnode_init(&(ps + i)->bnode);

		btree.build(&btree, &(ps + i)->bnode, cmp_data);
	}

	printf("=========递归========\n");
	printf("中序遍历：\n");
	btree.in_order(btree.root, print_node);
	printf("\n前序遍历：\n");
	btree.pre_order(btree.root, print_node);
	printf("\n后序遍历：\n");
	btree.post_order(btree.root, print_node);
	
	printf("\n=========非递归========\n");
	printf("中序遍历：\n");
	btree.in_order_norec(&btree, print_node);
	printf("\n前序遍历：\n");
	btree.pre_order_norec(&btree, print_node);
	printf("\n后序遍历：\n");
	btree.post_order_norec(&btree, print_node);
	printf("\n");
	
	printf("\n层序遍历：\n");
	btree.level(&btree, print_node);
	printf("\n");

	printf("树的深度是：%d\n", btree.depth(&btree));
	printf("该二叉树%s是平衡二叉树！\n", 
			btree.is_balance(&btree) ? "" : "不");

	printf("查找：\n");
	struct bnode *find = btree.search(&btree, &(ps + 10)->bnode, cmp_data);
	if (find) {
		printf("找到了，该节点的值＝%d\n", 
			container_of(find, struct node, bnode)->data);
	} else {
		printf("没找到!\n");
	}
	
	printf("查找值最小的节点：\n");
	find = btree.search_min(&btree);
	if (find) {
		printf("值最小的节点的值＝%d\n", 
			container_of(find, struct node, bnode)->data);
	} else {
		printf("树为空!\n");
	}
	printf("查找值最大的节点：\n");
	find = btree.search_max(&btree);
	if (find) {
		printf("值最大的节点的值＝%d\n", 
			container_of(find, struct node, bnode)->data);
	} else {
		printf("树为空!\n");
	}

#if 0
	printf("删除节点：\n");
	printf("del: %d\n", container_of(&(ps + 6)->bnode,struct node, bnode)->data);
	btree.del(&btree, &(ps + 6)->bnode);
	printf("中序遍历：\n");
	btree.in_order(btree.root, print_node);
	printf("\n");
#endif

	printf("反转二叉树：\n");
	btree.reverse(&btree);
	printf("中序遍历：\n");
	btree.in_order(btree.root, print_node);
	printf("\n");

	btree_destroy(&btree);	
	free(ps);

	return 0;
}
