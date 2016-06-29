#pragma once

//描述表头节点的类型
struct hlist_head {
	struct hlist_node *first;
};

//描述表头节点所在的链表上挂接的所有节点的类型
struct hlist_node {
	struct hlist_node *next;
	struct hlist_node **pprev; //指向前一个节点的first指针或者next指针的指针
};

//初始化表头节点
static inline void INIT_HLIST_HEAD(struct hlist_head *head)
{
	head->first = NULL;
}

//初始化有效节点
static inline void INIT_HLIST_NODE(struct hlist_node *node)
{
	node->next = NULL;
	node->pprev = NULL;
}

//头插：插入新节点到头节点和头节点的下一个节点之间
static inline void list_add(struct hlist_head *head, struct hlist_node *node)
{
	node->next = head->first;
	head->first = node;
	node->pprev = &head->first;
	if (node->next) { //如果head之后已经存在节点
		node->next->pprev = &node->next;
	}
}

//将一个节点插入到某个节点的前面
static inline void list_add_before(struct hlist_node *node, struct hlist_node *base)
{
	node->next = base;
	*(base->pprev) = node;
	node->pprev = base->pprev;
	base->pprev = &node->next;
}

//删除节点
static inline void list_del(struct hlist_node *node)
{
	*node->pprev = node->next;
	if (node->next) {
		node->next->pprev = node->pprev;
	}

	INIT_HLIST_NODE(node);
}

static inline int list_is_empty(struct hlist_head *head)
{
	return !head->first;
}

//macro
#define offsetof(type, member) \
	( (size_t)(&((type *)0)->member) )

#define hlist_entry(ptr, type, member) \
	( (type *)((size_t)(ptr) - offsetof(type, member)) )

#define hlist_for_each(cur, head) \
	for (cur = (head)->first; cur; cur = (cur)->next)

#define hlist_for_each_safe(cur, tmp, head) \
	for (cur = (head)->first; \
			cur && (tmp = (cur)->next, 1); \
			cur = tmp)

#define hlist_for_each_entry(ptr, cur, head, member) \
	for (cur = (head)->first; \
		cur && (ptr = hlist_entry(cur, typeof(*ptr), member)); \
		cur = (cur)->next)

#define hlist_for_each_entry_safe(ptr, cur, tmp, head, member) \
	for (cur = (head)->first; \
		cur && (tmp = (cur)->next, 1) \
			&& (ptr = hlist_entry(cur, typeof(*ptr), member)); \
		cur = tmp)


