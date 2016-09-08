


#include <stdio.h>
#include <string.h>
#include <stdlib.h>



typedef int elem_t;

struct node {
    elem_t data;
    struct node *next;
};

static inline void add_tail(struct node *head, struct node *nd)
{
    /*printf("hehe\n");*/
    struct node *cur = head;
    for (; cur->next != NULL; cur = cur->next) {}
    cur->next=nd;
    /*node->next = NULL;*/
}

static void list_for_each(struct node *head){

    struct node *cur = head->next;
    for( ; cur!=NULL; cur = cur->next)
    {
        printf("list for each %d\n", cur->data);
    }
}

struct node * search(struct node *head, struct node *node){
    struct node *cur=head;
    for (; cur != NULL; cur = cur->next)
    {
        if(  cur == node ) { return cur;}
    
    }

    return NULL;
}


int main(int argc, char *argv[])
{

    /*char *p  = (char *)malloc(6);*/
    /*memset(p, 0, 6);*/

    /*printf("p %s sizeof %d\n", p, sizeof(p));*/

    /*free(p);*/
   struct node *head = (struct node *)malloc(sizeof (struct node));
   if (!head){
        printf("aquire head failed!!\n"); 
        return -1;
   }
   head->next = NULL; 

   struct node s[10];

   int i = 0;
   printf("%ld\n", sizeof(s)/sizeof(struct node));
   for ( ;i< (sizeof(s)/sizeof(struct node)); i++){
        (s + i)->data = i;
        (s + i)->next = NULL;
        printf("add tail %d &head %p, s+i %p\n", i, head, s+i);
        add_tail(head, s+i);
   }
#if 1
    list_for_each(head);
    printf("Search : \n");
    struct node *find = search(head, s+7);
    if (find){
        printf("找到了， 节点的值是: %d\n", find->data); 
    }
    else{
        printf("没找到"); 
    
    }

#endif

}
