#include <stdio.h>
#include <stdlib.h>

#include "list.h"


static inline struct node *search(struct list *list, elem_t data)
{
    printf("1\n");
    struct node * cur = list->head;
    printf("2\n");
    for (; cur; cur=cur->next)
    {
        if (cur->data == data) 
            return cur;
        printf("3\n");
    }

    return NULL;
}

static inline void del(struct list *list, elem_t data)
{

    if (!list->head)
        return;
#if 1
    struct node *cur = list->head, *tmp = NULL;
    /*for (; cur; cur=cur->next, tmp=cur)*/
    while(cur)
    {
        if (cur->data == data) 
        {
            printf("del %d\n", data);
            if(cur == list->head){
                list->head = cur->next; 
                cur->next = NULL;
                free(cur);
                cur = list->head;
            }

            else{
                tmp->next = cur->next;
                cur->next = NULL;
                free(cur);
                cur = tmp->next;
            }
         }else{
            tmp = cur;
            cur = cur->next;
         
         
         }
    }
#endif
    #if 0 
    struct node *tmp = NULL, *cur = list->head;
    while (cur) {
        if (cur->data == data) { //当前节点的值和要删除的值相同
            if (cur == list->head) {//要删除的节点是第一个节点
                list->head = cur->next;
                cur->next = NULL;
                free(cur);
                cur = list->head;
            } else {//要删除的是除第一个节点之外的其他节点
                tmp->next = cur->next;
                cur->next = NULL;
                free(cur);
                cur = tmp->next;
            }
        } else {
            tmp = cur;
            cur = cur->next;
        }
    }
    #endif

}

static inline void add(struct list *list, elem_t data)
{
    struct node * node = (struct node *)malloc(sizeof(struct node));
    if (!node)
    {
        printf("aquire node space failed!\n");
        exit(-1);
    }

    node->data = data;
    node->next = NULL;

    if (list->head == NULL)
    {
        list->head = node; 
    }

    else{
        node->next = list->head; 
        list->head = node;
    }

}


static inline void list_for_each(struct list *list, void (*todo)(struct node *))
{
    struct node *cur = list->head;
    printf("list for each\n");
    for (; cur; cur=cur->next)
    {
        (*todo)(cur);
    }
    printf("list for each done\n");

}

static inline void destroy(struct list *list)
{
    while (list->head)
    {
        del(list, list->head->data);
    }

}
void list_init(struct list *list)
{
    list->head = NULL;
    list->add = add;
    /*list->add_tail = add_tail;*/
    list->list_for_each = list_for_each;
    list->search  = search;
    list->del = del;
    list->destroy = destroy;

}
