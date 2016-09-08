#include "sort.h"


static inline void swap(elem_t *a, elem_t *b)
{
    elem_t tmp  = *a;
    *a = *b;
    *b = tmp;
}

static inline void bubble_sort(elem_t *a, size_t n)
{
    int i, j;
    int count=0;
    for (i = 0; i < n-1; i++){
        /*for (j = 0; j < n - 1 - i; j++)*/
        for (j = 0; j < n - 1-i; j++)
        {
            count ++;
            printf("count %d\n", count);
            //每次排序把最大的 放到后边。
            if (*(a+j) > *(a+j+1)) {
                swap(a+j, a+j+1);
            } 
        }
    }

}

static inline void select_sort(elem_t *a, size_t n)
{
    int i,j,min;

    for (i=0; i < n-1; i++)
    {
        for (min=i, j=i+1; j < n; j++) 
        {
            //每次排序把最小的 放到 最前边
            if (a[j] < a[min]) 
            {
                min = j; 
            }
        
        }
        if (min != i){
            swap(a+min, a+i); 
        }
    
    }

}

void pl(elem_t *, size_t);
static inline void insert_sort(elem_t *a, size_t n)
{
    // 12 1 4 3 11 6
    // save 1 j=0

    int i, j , save;
    for (i=1; i < n; i++)
    {
        /*pl(a, n);*/
        for (save = a[i], j=i-1; (j>=0) && (a[j] > save); j--) 
        {
            /*printf("before save %d a[j+1] %d a[j] %d \n", save, a[j+1], a[j]);*/
            a[j+1]  = a[j];
            printf("save %d a[j+1] %d a[j] %d \n", save, a[j+1], a[j]);
        
        }
        a[j+1] = save;
        printf("j = %d \n", j+1);
        pl(a, n);
    
    }


}


void sort_init(struct sort *this){
    this->bubble = bubble_sort;
    this->select = select_sort;
    this->insert = insert_sort;
}

void pl(elem_t *a, size_t n)
{
    int i=0;
    while (i < n)
    {
        printf("%d ", *(a+i));
        i++;
    }
    printf("\n");

}
