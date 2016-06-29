#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	//根据程序执行的顺序，通常有以下3种结构：
	//1、顺序结构 
	//2、选择结构 
	//3、循环结构 

	//程序结构中常用的语句有以下几种：
	//1、选择语句 if... else if... else  switch...case
	//2、循环语句 while do...while for
	//3、跳转语句 break continue exit return goto
	//4、空语句

	int a = 0;
	if (0 == a) {
		a += 10;
	} else if (a < 0) {
		a = 1;
	} else {
		a -= 10;
	}
	printf("a = %d\n", a);

#if 1
	printf("请输入一个字符：\n");
	char c;
	scanf("%c", &c);
	//switch 后面的()中只能是整型或字符型的表达式，不能是浮点型和字符串类型
	switch (c) {
		//case 后面的表达式必须是确定的一个整常量，不能出现函数的调用或变量表达式
		case 'a' ... 'z':
		case 'A' ... 'Z':
			printf("alpha character !\n");
			break;
		case '0' ... '9':
			printf("digit num !\n");
			break;
		default: //作为最后一个分支，break可以省略
			;
			//printf("special character!\n");
	}
#endif

#if 0
	int a1 = 10, b1 = 12, c1 = 13;
	if (a1 < b1 < c1) { //(a1 < b1) < c1
		printf("ok\n");
	}
#endif
	
#if 0
	printf("请输入一个成绩：\n");
	int score;
	scanf("%d", &score);

	if (score > 100 || score < 0) {
		printf("成绩的范围是0~100，请确认！\n");
		exit (EXIT_FAILURE); //失败退出
	}

	int k = score/10;
	switch (k) {
		case 10:
		case 9:
			printf("level A!\n"); break;
		case 8:
			printf("level B!\n"); break;
		case 7:
			printf("level C!\n"); break;
		case 6:
			printf("level D!\n"); break;
		case 0 ... 5:
			printf("level E!\n"); break;
	}
#endif

	int sum = 0;
	int i = 0;
	while (i < 101) {
		sum += i;
		i++;
	}
	printf("while, sum = %d\n", sum);

	for (i = 0, sum = 0; i < 101; i++) {
		sum += i;
	}
	printf("for, sum = %d\n", sum);
	
	i = 0;
	sum = 0;
	do {
		sum += i;
		i++;
	} while (i < 101);
	printf("do...while, sum = %d\n", sum);

	if (1) {
		//break; //break只能用于循环和switch...case
	}

	i = 0;
	sum = 0;
	// goto 之所以不建议使用，是因为很容易写出面条式代码
	// 最常用之处是用于从深层循环内部直接跳到最外面
out:
	i++;
	if (i > 100) {
		goto out2;
	}
	sum += i;
	goto out;
out2:
	printf("goto, sum = %d\n", sum);
//out3:
//	; //空语句的用处之一

	int ss = 10;
	while (1) {
		while (1) {
			break; //break只能跳出它所在的那层循环
		}
		if (ss == 10) {
			break;
		} else {
			continue; //提前进入下一次循环的判断，如果有续后语句，不会执行
		}

		printf("outer loop!\n");
	}
	printf("ok\n");

	for (;0;) {}
	for (;0;) 
		;

	return 0;
}


