#include <stdio.h>

#include "list.h"


static inline void print_code(struct node * node)
{
    printf("%d ", node->data);

}

int main(void)
{
    struct list list;

    list_init(&list);

    int i = 0;

    for (; i< 10; i++)
    {
        list.add(&list, i);
    }

    list.list_for_each(&list, print_code);
    printf("search\n");

    if (list.search(&list, 9)){
        printf("find it !\n"); 
    }
    else{
        printf("can not find it !\n"); 
    }

    printf("del :\n");
    list.del(&list, 6);
    list.list_for_each(&list, print_code);
    list.destroy(&list);
    printf("------");
    list.list_for_each(&list, print_code);

}
