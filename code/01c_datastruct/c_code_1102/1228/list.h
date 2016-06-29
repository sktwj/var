#pragma once

//内核链表的节点的结构
struct list_head {
	struct list_head *prev;
	struct list_head *next;
};

#define LIST_HEAD \
	struct list_head head = {&head, &head}

static inline void LIST_INIT(struct list_head *list)
{
	list->prev = list;
	list->next = list;
}

static inline void add(struct list_head *list, struct list_head *prev, struct list_head *next)
{
	prev->next = list;
	next->prev = list;
	list->next = next;
	list->prev = prev;
}

static inline void list_add(struct list_head *head, struct list_head *list)
{
	add(list, head, head->next);
}

static inline void list_add_tail(struct list_head *head, struct list_head *list)
{
	add(list, head->prev, head);
}

static inline void del(struct list_head *list)
{
	list->prev->next = list->next;
	list->next->prev = list->prev;

	LIST_INIT(list);
}

static inline int list_is_empty(struct list_head *head)
{
	return head->next == head;
}

static inline void list_destroy(struct list_head *head)
{
	while (!list_is_empty(head)) {
		del(head->next);
	}
}

#define offsetof(type, member) \
	( (size_t)(&((type *)0)->member) )

// 该宏用于得到ptr指向的member成员所在的结构体的首地址
#define container_of(ptr, type, member) \
	( (type *)((size_t)(ptr) - offsetof(type, member)) )

// list_entry 是 container_of 的别名，一般是用于链表中的，在哈希表中使用又是另一个别名 hlist_entry；container_of这个名字是比较通用的，可以用于任何能表示容器的结构中
#define list_entry(ptr, type, member) \
	container_of(ptr, type, member)

// 遍历内核链表结构struct list_head所在的小结构体
#define list_for_each(cur, head) \
	for (cur = (head)->next; \
			cur != (head); \
			cur = (cur)->next)

// 遍历内嵌了struct list_head类型的小结构体的那个大结构体
// @ptr: 表示cur指向的小结构体所在的大结构体的首地址

//#define list_for_each_entry(ptr, cur, head, member) \
	for (cur = (head)->next, \
			ptr = list_entry(cur, typeof(*ptr), member); \
			cur != (head); \
			cur = (cur)->next, \
			ptr = list_entry(cur, typeof(*ptr), member))

#define list_for_each_entry(ptr, head, member) \
	for (ptr = list_entry((head)->next, typeof(*ptr), member); \
		&(ptr)->member != (head); \
		ptr = list_entry((ptr)->member.next, typeof(*ptr), member))

#define list_for_each_safe(cur, tmp, head) \
	for (cur = (head)->next, tmp = (cur)->next; \
			cur != (head); \
			cur = tmp, tmp = (tmp)->next)

//tmp表示ptr指向的结构体中的小结构体的下一个小结构体的指针
#define list_for_each_entry_safe(ptr, tmp, head, member) \
	for (ptr = list_entry((head)->next, typeof(*ptr), member), tmp = (ptr)->member.next; \
	&(ptr)->member != (head); \
	ptr = list_entry(tmp, typeof(*ptr), member), \
	tmp = (tmp)->next)

//tmp参数表示ptr指向的大结构体的下一个同类型的大结构体的指针
#define list_for_each_entry_safe_2(ptr, tmp, head, member) \
	for (ptr = list_entry((head)->next, typeof(*ptr), member), \
	tmp = list_entry((ptr)->member.next, typeof(*tmp), member); \
	&(ptr)->member != (head); \
	ptr = tmp, \
	tmp = list_entry((tmp)->member.next, typeof(*tmp), member))
