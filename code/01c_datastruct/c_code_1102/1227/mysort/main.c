#include "sort.h"



int main(void)  
{
    struct sort s;
    size_t j;

    sort_init(&s);

    elem_t a[] = {11,15,4,6,2,3,-1};
    size_t n = sizeof(a)/sizeof(elem_t);

    printf("before sort: ");
    size_t i = 0;
    for (; i < n; i++)
    {
        printf("%d ", *(a+i)); 
    }
    printf("\n");

    /*s.bubble(a, n);*/
    /*s.select(a, n);*/
    s.insert(a, n);
    printf("after sort: ");

    i = 0;
    for (; i < n; i++)
    {
        printf("%d ", a[i]); 
    }
    printf("\n");

    for (i=0,j=0; i!=j;i++)
    {
        printf("fuck....%d \n", i); 
    
    }

    printf("here....%d %d\n", i, j); 
    

}
