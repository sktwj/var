#include "list.h"


struct queue{
    struct list_head head;

    void (*push)(struct list_head *, struct list_head *);

    struct list_head *(*pop)(struct queue *, void (*)(struct list_head *));

    struct list_head *(*top)(struct queue *);

    int (*is_empty)(struct queue *);

};


void queue_init(struct queue *);
void queue_destroy(struct queue *, void (*)(struct list_head *));
