#include "list.h"
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>

#define LEN 10

struct node {
    char name[LEN];
    int score;
    struct list_head list;
};

int main(void)
{
    LIST_HEAD;

    struct node *ps[LEN];
    int i = 0;
    for (; i < LEN; i++){
         ps[i] = malloc(sizeof(struct node));
         assert(ps[i]);
         sprintf(ps[i]->name, "%s%d", "lucy", i);
         ps[i]->score = rand() % 100;
         LIST_INIT(&(ps[i]->list));
         list_add_tail(&head, &(ps[i]->list));
         printf("add %p\n", &(ps[i]->list));
    }

    struct node *ptr = NULL;
    struct list_head * cur = NULL;
    i = 0;

    list_for_each(cur, &head) {
        ptr = container_of(cur, struct node, list); 
        printf("name: %s, \t score:%d \n", ptr->name, ptr->score);
    }
    printf("after del score < 60: \n");
    struct list_head *tmp = NULL;
    
    list_for_each_safe(cur, tmp, &head){
        ptr = list_entry(cur, struct node, list); 
        if (ptr->score < 60) 
        {
            printf("<60 %s\n", ptr->name);
            printf("cur %p ptrlist %p\n", cur, &ptr->list);
            del(cur);
            free(ptr);
        }
    }

    list_for_each_entry(ptr, &head, list) {
        printf("name=%s score %d \n", ptr->name, ptr->score); 
    
    }

    printf("del all node\n");

    struct node *ptmp = NULL;

    list_for_each_entry_safe_2(ptr, ptmp, &head, list){
        del( &(ptr->list)); 
        free(ptr); 
    }
    
    list_for_each_entry(ptr, &head, list){
        printf("name %s, score %d \n", ptr->name, ptr->score);
    }

    srand((unsigned int)(time(NULL)));
    for (; i<10; i++)
    {
        printf("random  %d %d\n", i,rand()%100);
    }

}
