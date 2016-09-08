#pragma once 


struct list_head {
    struct list_head *prev;
    struct list_head *next;
}


#define LIST_HEAD_INIT \
    struct list_head head = {&head, &head};


static inline void LIST_INIT(struct list_head *list)
{
    list->prev = list;
    list->next = list;
}

static inline void add(struct list_head *list, struct list_head *prev, struct list_head *next)
{
    prev->next = list;
    next->prev = list;
    list->prev = prev;
    list->next = next;
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

static inline void list_is_empty(struct list_head *head)
{

    return head->next == head;
}

static inline void list_destory(struct list_head *head)
{
    while(!list_is_empty(head))
    {
        del(head->next); 
    }
}
#define offsetof(type, member) \
    ((size_t)(&((type *)0) -> member ) )

#define container_of(ptr, type, member) \
   ((type *) ((size_t)(ptr) - offsetof(type, member)))

#define list_entry(ptr, type, member) \
    container_of(ptr, type, member)

#define list_for_each(cur, head) \
    for (cur=(head)->next; \
         cur != (head); \
         cur = (cur) ->next)


//#define list_for_each_entry(ptr, cur, head, member) \
    for (cur = (head)->next, \
            ptr = list_entry(cur, typeof(*ptr), member); \
            cur != (head); \
            cur = (cur)->next; \
            ptr = list_entry(cur, typeof(*ptr), member) )

#define list_for_each_entry(ptr, head, member) \
    for ( ptr = list_entry((head)->next, typeof(*ptr), member), \
         &(ptr) -> member != head; \
        ptr = list_entry((ptr)->member.next, typeof(*ptr), member))

#define list_for_each_safe(cur, tmp, head) \
    for (cur = (head)->next, tmp=(cur)->next; \
        cur != head; \
        cur = tmp; tmp = (tmp) ->next)

#define list_for_each_entry_safe_2(ptr, tmp, head, member) \
    for (ptr= list_entry((head)->next, typeof(*ptr), member), \
         tmp = list_entry((ptr)->member.next, typeof(*ptr), member); \
         &(ptr)->member != (head); \
         ptr = tmp, \
         tmp = list_entry((tmp)->member.next, typeof(*tmp), member))



#define list_for_each_entry_safe(ptr, tmp, head, member) \
    for (ptr = list_entry((head)->next, typeof(*ptr),member), \
        tmp = (ptr) ->member.next; \
        &(ptr) ->member != head ; \
        ptr = list_entry(tmp, typeof(*ptr), member), \
        tmp = (tmp) ->next)
