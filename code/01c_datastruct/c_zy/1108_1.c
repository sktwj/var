#include <stdio.h>

void find_murder(void)
{
	int flag = 0;
	char murder = 'A'; //假定凶手是A
	for (; murder < 'A' + 4; murder++) { //穷举所有的情况，找出符合条件的
		if (3 == (murder != 'A') + (murder == 'C') + (murder == 'D') + (murder != 'D')) { //判断条件是三个人说了真话，一个人说了假话，也是解题的关键
			flag = 1;
			printf("找到了，凶手是：%c\n", murder);
		}
	}

	if (flag == 0) {
		printf("题目有误，无法找到凶手！\n");
	}
}

int main(void)
{
	find_murder();
	return 0;
}
