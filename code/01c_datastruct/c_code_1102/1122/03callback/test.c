#include <stdio.h>



#define swap_endian(n) \
    ((n & 0xff) << 24) | \
    (((n >> 8) & 0xff) << 16) | \
    (((n >> 16) & 0xff) << 8) | \
    (n >> 24)


static inline void callback(void *ss)
{
    printf("hello %s\n", ss);
}

static inline void do_it(void (*call)(void *), void *n)
{
   call(n);
}

int main(void)
{
    do_it(callback, "haha woshi aoteman!!");
    /*unsigned int a = 0x12345678;*/
    size_t a = 0x12345678;
    printf("%d %d\n", sizeof(size_t), sizeof(unsigned int));
    printf("%#x\n", ((a<< 24)|(a>>8<<24>>8)|(a<<8>>24<<8)|(a>>24)));
    /*a = swap_endian(a);*/
    printf("%#x\n", a);
}
