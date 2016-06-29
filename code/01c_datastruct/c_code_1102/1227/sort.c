#include "sort.h"

static inline void swap(elem_t *a, elem_t *b)
{
	elem_t tmp = *a;
	*a = *b;
	*b = tmp;
}

static inline void bubble_sort(elem_t *a, size_t n)
{
	int i, j;
	for (i = 0; i < n - 1; i++) {
		for (j = 0; j <= n - 1 - i; j++) {
			if (*(a + j) > *(a + j + 1)) {
				swap(a + j, a + j + 1);
			}
		}
	}
}

static inline void select_sort(elem_t *a, size_t n)
{
	int i, j, min;
	for (i = 0; i < n - 1; i++) {
		for (min = i, j = i + 1; j < n; j++) {
			if (a[j] < a[min]) {
				min = j;
			}
		}
		if (min != i) {
			swap(a + min, a + i);
		}
	}	
}

static inline void insert_sort(elem_t *a, size_t n)
{
	int i, j, save; //save用于保存即将被擦写掉的元素的值
	for (i = 1; i < n; i++) {
		for (save = a[i], j = i-1; 
				(j>=0) && (a[j] > save);
				j--) {
			a[j+1] = a[j];
		}
		a[j+1] = save;
	}
}

static inline void quick_sort(elem_t *a, size_t n)
{
	if (n <= 1) { //递归的结束条件
		return;
	}

	elem_t *p = a + n/2; //中值
	printf("1, *p = %d\n", *p);
	elem_t *pl = a;
	elem_t *pr = a + n-1;
	elem_t save = *p; //用来保存中值的值

	//从中值左边找第一个比中值大的元素（如果存在的话）
	for (; (pl < p) && (*pl < *p); pl++) {}
	if (pl < p) { //找到了比中值大的元素
		*p = *pl;
	}

	while (1) {
		for (; (pl < pr) && (*pr > save); pr--) {} //从中值右边找比中值小的元素
		if (pl < pr) {
			*pl = *pr;
			pl++;
		} else {
			break;
		}

		for (; (pl < pr) && (*pl < save); pl++) {} //从中值左边找比中值大的元素
		if (pl < pr) {
			*pr = *pl;
			pr--;
		} else {
			break;
		}
	}
	printf("2, *p = %d\n", *p);
	*pl = save; //找到了中值元素应该插入的位置

	quick_sort(a, pl - a); //中值左边元素递归
	quick_sort(pl + 1, n - 1 - (pl - a)); //中值右边元素递归
}

static inline void adjust_tree(elem_t *a, size_t i, size_t n)
{
	size_t j = 2 * i + 1; //i节点的孩子中，值比较大的节点的下标

	while (j < n) {	
		if ((j+1 < n) && (a[j+1]>a[j])) { //i节点的右孩子存在
			j = j+1; //j重新记录两个孩子中，值更大的节点下标
		}
		if (a[i] < a[j]) { //比较i节点和i节点的左孩子的值
			swap(a + i, a + j);
			i = j;
			j = j * 2 + 1;
		} else {
			break;
		}
	}
}

static inline void heap_sort(elem_t *a, size_t n)
{
	//大根堆（大顶堆）：每一个子树的根节点的值大于它的孩子节点的值
	//小根堆（小顶堆）：每一个子树的根节点的值小于它的孩子节点的值
	int i = n/2 - 1; //最后一个非叶子节点的下标
	for (; i >= 0; i--) { //从最后一个非叶子节点开始，初始调整完全二叉树为大根堆
		adjust_tree(a, i, n); //调整完全二叉树为大根堆
	}
	int k = 0;
	for (i = n - 1; i > 0; i--) {
		swap(a + 0, a + i); //交换堆顶节点和最后一个叶子节点的值
		for (k = 0; k < n; k++) {
			printf("%d ", a[k]);
		}
		printf("\n");
		adjust_tree(a, 0, i); //继续调整完全二叉树为大根堆。中间的参数表示当次从哪个节点开始调整完全二叉树为大根堆，因为在上一次交换过最后一个叶子节点和堆顶节点的值之后，只有堆顶节点可能破坏了大根堆，所以从堆顶节点开始继续调整。第三个参数表示还有多少个节点待排序。
	}
}

//希尔排序也称为缩小增量排序
static inline void shell_sort(elem_t *a, size_t n)
{
	int k = n/2; //增量的值
	int i, j, save; //save用于保存即将被擦写掉的元素的值
	for (; k >= 1; k /= 2) {
		for (i = k; i < n; i++) {
			for (save = a[i], j = i-k; 
					(j>=0) && (a[j] > save);
					j-=k) {
				a[j+k] = a[j];
			}
			a[j+k] = save;
		}
	}
}

static inline void merge_sort(elem_t *a, size_t n)
{
	int m = n/2; //用来拆分数组为两半的中值
	int l = 0; //中值左边的第一个元素的下标
	int r = m; //中值右边的第一个元素的下标

	if (m > 1) {
		merge_sort(a, m); //中值左边待排序元素递归
		merge_sort(a + m, n - m); //中值右边待排序元素递归
	}

	elem_t b[n];
	int i = 0;
	while ((l < m) && (r < n)) {
		if (a[l] <= a[r]) {
			b[i++] = a[l++];
		} else {
			b[i++] = a[r++];
		}
	}
	while (l < m) { //右半部分先拷贝完毕
		b[i++] = a[l++];
	}
	while (r < n) { //左半部分先拷贝完毕
		b[i++] = a[r++];
	}

	for (i = 0; i < n; i++) {
		a[i] = b[i];
	}
}

void sort_init(struct sort *this)
{
	this->bubble = bubble_sort;
	this->select = select_sort;
	this->insert = insert_sort;
	this->quick = quick_sort;
	this->heap = heap_sort;
	this->shell = shell_sort;
	this->merge = merge_sort;
}
