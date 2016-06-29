#include <stdio.h>

//函数的传参：传递是实际参数的值的副本。所以C中的参数传递只有值传递一种方式，并不存在所谓的地址传递或引用传递。形式参数的值的改变并不会影响实际参数的值。
void f1(int a) //int a = 1;
{
	a = 10;
}

void f2(int *a) //int *a = 主函数内a变量的地址值的副本;
{
	*a = 20;
}

//数组型参数在进行传递时，实际传递的是数组的首地址，因为相对于拷贝整个数组的开销来说，拷贝一个地址值的开销是比较小的，所以编译器对于数组型的实际参数，会自动将其转换为指向数组首元素的指针的类型
//如果在函数内需要知道数组的长度，需要将数组的元素个数作为函数的另一个参数传入
void f3(int arr[100]) //int *arr
{
	printf("f3, size arr = %d\n", sizeof(arr));
	*(arr + 1) = 20;
}

const int *f4(const int *arr) //const修饰函数参数，用来通知编译器，我们不会通过该函数去修改实际参数的值。
{
	//*arr = 5; //无法通过arr修改它指向的变量的值
	return arr;
}

int main(void)
{
	int a = 1;
	//f1(a);
	//f2(&a);
	//printf("main, a = %d\n", a);

	int arr[100] = {1,2,3};
#if 0
	printf("main, size arr = %d\n", sizeof(arr));
	f3(arr);
	printf("f3, arr[1] = %d\n", arr[1]);
#endif
		
	int *p = f4(arr + 1); //警告
	*p = 5; //如果忽视上述警告，编译器是不会干涉的
	printf("f4, arr[1] = %d\n", arr[1]);

	return 0;
}
