#pragma once

typedef int elem_t;



struct node {
    elem_t data;
    struct node *next;
};



struct list {
    
    struct node *head;

    void (*add)(struct list *, elem_t);
    void (*add_tail)(struct list *, elem_t);
    void (*list_for_each)(struct list *, void (*)(struct node *));

    struct node * (*search)(struct list *, elem_t);
    void (*del)(struct list *, elem_t);
    void (*destroy)(struct list *);
};

void list_init(struct list *);
