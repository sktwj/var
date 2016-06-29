#include <stdio.h>

void guess_age(void)
{
	int flag = 0;
	int age; //记录年龄的
	int min, max; //记录年龄范围的最小和最大值的
	for (age = 1; age*age <= 999; age++) {
		if (age*age > 99) {
			if (flag == 0) {
				min = age;
			}
			flag = 1;
		}
	}
	max = age - 1;
	printf("1, min = %d, max = %d\n", min, max);

	flag = 0;
	for (age = min; age <= max; age++) {
		if (age*age*age > 999 && age*age*age <= 9999) {
			if (flag == 0) {
				min = age;
			}
			flag = 1;
		} else {
			break;
		}
	}
	max = age - 1;
	printf("2, min = %d, max = %d\n", min, max);

	flag = 0;
	for (age = min; age <= max; age++) {
		if (age*age*age*age > 99999 && 
				age*age*age*age <= 999999) {
			if (flag == 0) {
				min = age;
			}
			flag = 1;
		} 
	}
	max = age - 1;
	printf("3, min = %d, max = %d\n", min, max);

	int num3, num4; //三次方和四次方的数
	int i, arr[10] = {0}, brr[10] = {0};
	for (age = min; age <= max; age++) {
		num3 = age*age*age;
		num4 = age*age*age*age;
		
		for (i = 0; i < 4; i++) {
			arr[i] = num3 % 10;
			num3 /= 10;
		}

		for (; i < 10; i++) {
			arr[i] = num4 % 10;
			num4 /= 10;
		}

		printf("数组arr的元素为：");
		for (i = 0; i < 10; i++) {
			printf("%d ", arr[i]);
		}
		printf("\n");

		for (i = 0; i < 10; i++) {
			brr[arr[i]]++;
		}
		
		int ret = 1;
		printf("数组brr的元素为：");
		for (i = 0; i < 10; i++) {
			printf("%d ", brr[i]);
			ret *= brr[i];
			brr[i] = 0;
		}
		printf("\n");
		if (ret == 1) {
			printf("该同学的年龄是：%d\n", age);
		}
		ret = 1;
	}
}

int main(void)
{
	guess_age();
	return 0;
}
