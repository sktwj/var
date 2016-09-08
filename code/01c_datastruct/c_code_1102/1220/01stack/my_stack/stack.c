#include <stdio.h>


#include "stack.h"



static inline void push(strct list_head *head, struct list_head *snode)
{
    list_add(head, snode);
}
