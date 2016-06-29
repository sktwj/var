#include <stdio.h>
#include "btree.h"

static inline void bnode_init(struct bnode *bnode)
{
	bnode->parent = NULL;
	bnode->lchild = NULL;
	bnode->rchild = NULL;
}

static inline void build_tree(struct btree *btree, struct bnode *bnode, int (*cmp)(struct bnode *, struct bnode *))
{
	if (!btree->root) { //树为空，插入的第一个节点作为树根节点
		btree->root = bnode;
	}

	int ret;
	struct bnode *cur = btree->root; //用来遍历树上的节点
	struct bnode *parent = NULL;
	while (cur) { //寻找要插入的位置
		parent = cur;
		ret = cmp(bnode, cur);
		if (ret < 0) {
			cur = cur->lchild;
		} else if (ret > 0) {
			cur = cur->rchild;
		} else {
			return; //不允许插入值相同的节点
		}
	}

	//插入
	if (ret < 0) {
		parent->lchild = bnode;
	} else {
		parent->rchild = bnode;
	}	
	bnode->parent = parent;
}

static inline void in_order(struct bnode *bnode, 
		void (*todo)(struct bnode *))
{
	if (bnode) {
		in_order(bnode->lchild, todo);
		todo(bnode);
		in_order(bnode->rchild, todo);
	}	
}

static inline void pre_order(struct bnode *bnode, 
		void (*todo)(struct bnode *))
{
	if (bnode) {
		todo(bnode);
		pre_order(bnode->lchild, todo);
		pre_order(bnode->rchild, todo);
	}	
}

static inline void post_order(struct bnode *bnode, 
		void (*todo)(struct bnode *))
{
	if (bnode) {
		post_order(bnode->lchild, todo);
		post_order(bnode->rchild, todo);
		todo(bnode);
	}	
}

static inline struct bnode *search(struct btree *btree, struct bnode *bnode, int (*cmp)(struct bnode *, struct bnode *))
{
	if (!btree->root) {
		return NULL;
	}

	struct bnode *cur = btree->root;
	int cmp_value;
	while (cur) {
		cmp_value = cmp(bnode, cur);
		if (!cmp_value) {
			return cur;
		} else if (cmp_value < 0) {
			cur = cur->lchild;
		} else {
			cur = cur->rchild;
		}
	}

	return NULL;
}

static inline struct bnode *search_min(struct btree *btree)
{
	if (!btree->root) {
		return NULL;
	}

	struct bnode *cur = btree->root;
	while (cur->lchild) {
		cur = cur->lchild;
	}
	return cur;
}

static inline struct bnode *search_max(struct btree *btree)
{
	if (!btree->root) {
		return NULL;
	}

	struct bnode *cur = btree->root;
	while (cur->rchild) {
		cur = cur->rchild;
	}
	return cur;
}

//第2个参数是要删除的节点的小结构体的地址
static inline void del_node(struct btree *btree, struct bnode *bnode)
{
	//树上的节点的身份
	enum e_id {
		ROOT, LCHILD, RCHILD,
	} id;

	if (!bnode->parent) { //是否树根节点
		id = ROOT;
	} else if (bnode->parent->lchild == bnode) { //是否是其父亲节点的左孩子
		id = LCHILD;
	} else if (bnode->parent->rchild == bnode) { //是否是其父亲节点的右孩子
		id = RCHILD;
	}

	if (!bnode->lchild && !bnode->rchild) { //如果要删除的节点是叶子节点
		printf("ok\n");
		switch (id) {
		case ROOT:
			btree->root = NULL;
			break;
		case LCHILD:
			printf("ok1\n");
			bnode->parent->lchild = NULL;
			bnode->parent = NULL;
			break;
		case RCHILD:
			printf("ok2\n");
			bnode->parent->rchild = NULL;
			bnode->parent = NULL;
			break;
		}

	} else if (bnode->lchild && !bnode->rchild) { //如果要删除的节点有左孩子，没有右孩子
		switch (id) {
		case ROOT:
			bnode->lchild->parent = NULL;
			btree->root = bnode->lchild;
			//bnode->parent->lchild = bnode->lchild;
			break;
		case LCHILD:
			bnode->lchild->parent = bnode->parent;
			bnode->parent->lchild = bnode->lchild;
			bnode_init(bnode);
			break;
		case RCHILD:
			bnode->lchild->parent = bnode->parent;
			bnode->parent->rchild = bnode->lchild;
			bnode_init(bnode);
			break;
		}

	} else if (!bnode->lchild && bnode->rchild) { //如果要删除的节点有右孩子，没有左孩子
		switch (id) {
		case ROOT:
			bnode->rchild->parent = NULL;
			btree->root = bnode->rchild;
			break;
		case LCHILD:
			bnode->rchild->parent = bnode->parent;
			bnode->parent->lchild = bnode->rchild;
			bnode_init(bnode);
			break;
		case RCHILD:
			bnode->rchild->parent = bnode->parent;
			bnode->parent->rchild = bnode->rchild;
			bnode_init(bnode);
			break;
		}

	} else { //如果要删除的节点两个孩子都存在
	//找到一个能顶替删除节点的节点来取代要删除的节点，可以找这样的两个节点之一：
	//1、要删除节点的左孩子的最右边的孩子（其实就是找要删除节点的左子树中值最大的节点）
	//2、要删除节点的右孩子的最左边的孩子（其实就是找要删除节点的右子树中值最小的节点）
	
	//下面的代码以1来做：
		//定位到要删除节点的顶替节点
		struct bnode *cur = bnode->lchild;
		while (cur->rchild) {
			cur = cur->rchild;
		}

		//判断能顶替要删除的节点是其父亲节点的左孩子还是右孩子
		if (cur == cur->parent->lchild) { //是左孩子
			cur->parent->lchild = cur->lchild;
		} else { //是右孩子
			cur->parent->rchild = cur->lchild;
		}

		if (cur->lchild) { //cur的左孩子是否存在
			cur->lchild->parent = cur->parent;
		} 

		switch (id) {
		case ROOT:
			btree->root = cur;
			break;
		case LCHILD:
			bnode->parent->lchild = cur;
			break;
		case RCHILD:
			bnode->parent->rchild = cur;
			break;
		}

		//cur接收所有要删除节点bnode的所有关系
		cur->parent = bnode->parent;
		cur->lchild = bnode->lchild;
		cur->rchild = bnode->rchild;
	
		if (cur->lchild) {
			cur->lchild->parent = cur;
		}
		if (cur->rchild) { //其实这个判断条件可以不要，因为既然要删除的节点必定是有两个孩子的，我们选择的能顶替它（要删除的节点）的节点是它左（要删除节点）孩子的最右边的孩子，假如它的左孩子没有右孩子，那么它的左孩子就是能顶替它的节点，如果该节点没有左孩子，那么上面的第230行如果不判断，231行就会出现段错误。但右孩子之所以不用判断，是因为它现在接收的右孩子就是要删除节点的右孩子（该右孩子必定存在，否则我们要删除的节点就不是有两个孩子的节点）。
			cur->rchild->parent = cur;
		}
	}

	bnode_init(bnode); //初始化要删除的节点，使其不再指向之前所指向的节点
}

static inline void __reverse(struct bnode *bnode)
{
	//树为空，或者遍历到叶子节点为止
	if (!bnode || ((!bnode->lchild) && (!bnode->rchild))) {
		return;
	}

	struct bnode *tmp;

	tmp = bnode->lchild;
	bnode->lchild = bnode->rchild;
	bnode->rchild = tmp;

	__reverse(bnode->lchild);
	__reverse(bnode->rchild);
}

static inline void reverse(struct btree *btree)
{
	__reverse(btree->root);
}

static inline void level_order(struct btree *btree, 
		void (*todo)(struct bnode *))
{
	struct queue queue; //定义队列变量
	queue_init(&queue); //初始化队列

	struct bnode *cur = btree->root; //cur用来遍历树上节点的
	queue.push(&queue, cur); //树根节点入队
	
	while (!queue.is_empty(&queue)) {
		cur = queue.pop(&queue); //把队头元素出队列
		todo(cur);

		//判断队头元素（二叉树上的节点）的左右孩子是否存在
		
		if (cur->lchild) {
			queue.push(&queue, cur->lchild);
		}
		if (cur->rchild) { 
			queue.push(&queue, cur->rchild);
		}
	}

	queue_destroy(&queue);
}

//非递归
static inline void pre_order_norec(struct btree *btree, 
		void (*todo)(struct bnode *))
{
	struct stack stack; //定义栈变量
	stack_init(&stack);

	struct bnode *cur = btree->root;
	while (!stack.is_empty(&stack) || cur) { //栈不为空或者cur指向的节点还存在

		if (cur) {
			stack.push(&stack, cur);
			todo(cur);
			cur = cur->lchild;
		} else {
			cur = stack.pop(&stack);
			cur = cur->rchild;
		}
	}

	stack_destroy(&stack);
}

static inline void in_order_norec(struct btree *btree, 
		void (*todo)(struct bnode *))
{
	struct stack stack; //定义栈变量
	stack_init(&stack);

	struct bnode *cur = btree->root;
	while (!stack.is_empty(&stack) || cur) { //栈不为空或者cur指向的节点还存在

		if (cur) {
			stack.push(&stack, cur);
			cur = cur->lchild;
		} else {
			cur = stack.pop(&stack);
			todo(cur);
			cur = cur->rchild;
		}
	}

	stack_destroy(&stack);
}

static inline void post_order_norec(struct btree *btree, 
		void (*todo)(struct bnode *))
{
	struct stack stack; //定义栈变量
	stack_init(&stack);

	struct bnode *tmp = NULL; //用来记录cur指向的节点的右孩子是否之前已经被todo过
	struct bnode *cur = btree->root;
	while (!stack.is_empty(&stack) || cur) { //栈不为空或者cur指向的节点还存在

		if (cur) {
			stack.push(&stack, cur);
			cur = cur->lchild;
		} else {
			cur = stack.top(&stack);
			if ((cur->rchild) && (cur->rchild != tmp)) { //cur节点的右孩子存在，并且该节点之前没有被todo过
				cur = cur->rchild;
			} else {
				cur = stack.pop(&stack);
				todo(cur);
				tmp = cur;
				cur = NULL;
			}
		}
	}

	stack_destroy(&stack);
}

static inline int __depth(struct bnode *bnode)
{
	int l_depth = 0, r_depth = 0;
	if (bnode) {
		l_depth = __depth(bnode->lchild);
		r_depth = __depth(bnode->rchild);

		return l_depth >= r_depth ? l_depth + 1: r_depth + 1;
	}
}

static inline int depth(struct btree *btree)
{
	return __depth(btree->root);
}

static inline int __balance(struct bnode *bnode, int *depth)
{
	if (!bnode) { //如果树为空，视树是平衡树
		*depth = 0;
		return 1;
	}

	int ldepth = 0, rdepth = 0, tmp = 0;
	if (__balance(bnode->lchild, &ldepth) 
		&& __balance(bnode->rchild, &rdepth)) { //如果左子树是平衡的，右子树也是平衡的
		tmp = ldepth - rdepth;
		if ((tmp <= 1) && (tmp >= -1)) { //左右子树的深度之差是否小于等于1
			*depth = ldepth >= rdepth ? (ldepth + 1) : (rdepth + 1);
			return 1;
		}
	}
	return 0;
}

static inline int is_balance(struct btree *btree)
{
	int depth;
	return __balance(btree->root, &depth);
}

void btree_init(struct btree *btree)
{
	btree->root = NULL;
	btree->bnode_init = bnode_init;
	btree->build = build_tree;
	btree->in_order = in_order;
	btree->pre_order = pre_order;
	btree->post_order = post_order;
	btree->search = search;
	btree->search_min = search_min;
	btree->search_max = search_max;
	btree->del = del_node;
	btree->reverse = reverse;
	
	btree->level = level_order;
	btree->pre_order_norec = pre_order_norec;
	btree->in_order_norec = in_order_norec;
	btree->post_order_norec = post_order_norec;

	btree->depth = depth;
	btree->is_balance = is_balance;
}

void __btree_destroy(struct bnode *bnode)
{
	if (bnode) {
		__btree_destroy(bnode->lchild);
		__btree_destroy(bnode->rchild);
	}
}

void btree_destroy(struct btree *btree)
{
	__btree_destroy(btree->root);
}
