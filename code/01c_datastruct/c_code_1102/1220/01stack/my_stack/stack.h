#pragma once 

struct stack {
    struct list_head head;
    

    void (*push)(struct list_head *, struct list_head *);

    struct list_head *(*pop)(struct stack *);
    struct list_head *(*top)(struct stack *);

    int (*is_empty)(struct stack *);
};

void stack_init(struct stack *);
void stack_destroy(struct stack *);
