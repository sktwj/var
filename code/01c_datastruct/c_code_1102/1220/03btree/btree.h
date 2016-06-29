#pragma once
#include "list.h"
#include "queue.h"
#include "stack.h"

//树型结构
//二叉树：每一个节点都严格区分其左右孩子
//排序二叉树：每一个节点的左孩子节点的值比其父亲节点的值小，右孩子节点的值比其父亲节点的值大

//树的深度：树中节点的最大层数
//平衡二叉树：左右子树的深度之差不大于1的二叉树
//完全二叉树：深度是k，节点个数是n的二叉树中，当且仅当树上的节点的编号，与深度为k的满二叉树的从1到n的节点的编号一一对应的二叉树被称为完全二叉树
//满二叉树：满足" 节点个数＝2^层数 - 1 " 性质的二叉树就被称为满二叉树
//根节点：唯一没有父亲节点的节点
//中间节点：有父亲节点，并且至少有一个孩子节点的节点
//叶子节点：有父亲节点，没有孩子节点的节点


//描述二叉树上的节点的类型
struct bnode {
	struct bnode *parent;
	struct bnode *lchild;
	struct bnode *rchild;
};

//描述二叉树的类型
struct btree {
	struct bnode *root; //指向根节点的指针

	void (*bnode_init)(struct bnode *);
	void (*build)(struct btree *, struct bnode *, 
		int (*)(struct bnode *, struct bnode *)); //建树（插入节点到树的过程）
	
	//递归遍历（前序、中序、后序）
	void (*in_order)(struct bnode *, void (*)(struct bnode *)); //中序
	void (*pre_order)(struct bnode *, void (*)(struct bnode *)); //前序
	void (*post_order)(struct bnode *, void (*)(struct bnode *)); //后序
	struct bnode *(*search)(struct btree *, struct bnode *, int (*)(struct bnode *, struct bnode *)); //查找节点
	struct bnode *(*search_min)(struct btree *); //查找值最小的节点
	struct bnode *(*search_max)(struct btree *); //查找值最大的节点
	
	void (*del)(struct btree *, struct bnode *); //删除节点

	//反转整个二叉树
	void (*reverse)(struct btree *);

	void (*level)(struct btree *, void (*)(struct bnode *)); //层序遍历

	//非递归遍历（前序、中序、后序）
	void (*in_order_norec)(struct btree *, void (*)(struct bnode *)); //中序
	void (*pre_order_norec)(struct btree *, void (*)(struct bnode *)); //前序
	void (*post_order_norec)(struct btree *, void (*)(struct bnode *)); //后序
	
	int (*depth)(struct btree *); //计算树的深度
	int (*is_balance)(struct btree *); //判断一棵二叉树是否平衡二叉树
};

//初始化二叉树
void btree_init(struct btree *);
//销毁二叉树
void btree_destroy(struct btree *);
