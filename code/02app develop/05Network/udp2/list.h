#pragma once

#include <assert.h>

struct list_head {
	struct list_head *next;
	struct list_head *prev;
};

/*内联函数:
 1. 指使用inline关键词的函数
 2. 特点：
 	在函数的调用，会把函数展开，替换成对应的函数体
	好处：较少了函数的寻址、传参、返回等过程，提高了执行效率 3. 函数使用注意事项:
	a. 函数体比较简单、简洁,不超过10行代码
	b. 不允许出现循环、递归、多分支等控制语句
	c. inline仅仅是一个建议，不是强制的，所以还要看是否符合规则
	d. 不允许使用函数指针，指向该类型的函数
	e. 宏替换是由预处理器完成、内联展开是由编译器完成
	f. 内联展开后势必增大程序的体积，但是空间换时间还是值得的
	g. 内联只能在1个源文件中有效，如果希望可以被多个源文件访问，所以把内联函数的定义放到.h文件中，将来需要用到时，只需要include就可以，为了方式函数名的冲突，一般配合关键词static使用
 */

static inline void list_head_init(struct list_head *phead)
{
	assert(phead != NULL);
	phead->next = phead;
	phead->prev = phead;
}

static inline void __list_add(struct list_head *pnode, 
		struct list_head *Prev, struct list_head *Next)
{
	pnode->next = Next;
	pnode->prev = Prev;

	Prev->next = pnode;
	Next->prev = pnode;
}


static inline void list_add(struct list_head *phead, 
							struct list_head *pnode)
{
	__list_add(pnode, phead, phead->next);
}

static inline void list_add_tail(struct list_head *phead, 
							struct list_head *pnode)
{
	__list_add(pnode, phead->prev, phead);
}

static inline void list_del(struct list_head *pnode)
{
	pnode->prev->next = pnode->next;
	pnode->next->prev = pnode->prev;
}

static inline void list_del_init(struct list_head *pnode)
{
	pnode->prev->next = pnode->next;
	pnode->next->prev = pnode->prev;

	list_head_init(pnode);
}

#define   list_for_each(cur, phead) \
		for (cur = (phead)->next; cur != phead; cur = (cur)->next)


#define   list_for_each_safe(cur, Next, phead) \
	for (cur = (phead)->next; (cur != phead) && (Next = (cur)->next); cur = Next)

/*offset_of: 取出小结构体与大结构之间的偏移量
 @type: 大结构(宿主结构)的类型
 @member: 小结构(指针域)在大结构体中的成员名
 */
#define  offset_of(type, member)	\
		((size_t)&(((type *)0)->member))

/*container_of: 小结构体的地址转大结构体的地址*/
#define  container_of(ptr, type, member)	\
		(type *)((char *)ptr - offset_of(type, member))

#define  list_entry(ptr, type, member)	\
		container_of(ptr, type, member)

/*从下一个节点开始*/
#define   list_for_each_continue(cur, phead) \
		for (cur = (cur)->next; cur != phead; cur = (cur)->next)

/*从当前的节点开始*/
#define   list_for_each_from(cur, phead) \
		for (; cur != phead; cur = (cur)->next)
