

#include <stdio.h>
#include "list.h"


#define offsetof(type, member) \
        ((size_t) (&((type *)0)->member))

#define container_of(ptr, type, member) \
        (type *)(((size_t ) ptr ) - offsetof(type, member))
struct tt{
    int aa;
    float shaka;
    char bb;
};


int main(void)
{
    LIST_HEAD;
    printf("head %p %p %p\n", &head, head.prev, head.next);

    struct tt t;
    printf("addr t %p\n", &t);
    printf("bb %d\n", offsetof(struct tt, bb));
    printf("container of %p\n", container_of(&t.bb, struct tt, bb));

}
