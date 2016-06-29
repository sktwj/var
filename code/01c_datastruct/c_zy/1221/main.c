#include <stdio.h>
#include "calc.h"

int main(int argc, char *argv[])
{
	struct calc *this = NULL;
	//const char *default_expr = \
		"(--1 ++2 + +-3 * (1 + 1)) * 2";
	const char *default_expr = "1 -+- 2";

	if (argc == 1) {
		this = calc_init(this, default_expr);	
		printf("%s = %d\n", default_expr, this->analyze(this, default_expr));	
	} else if (argc == 2) {
		this = calc_init(this, argv[1]);
		printf("%s = %d\n", argv[1], this->analyze(this, argv[1]));	
	} else {
		printf("sorry, 最多只能传递一个参数！\n使用方法：可执行文件 要进行运算的表达式\n");
		return 1;
	}

	calc_destroy(this);

	return 0;
}
