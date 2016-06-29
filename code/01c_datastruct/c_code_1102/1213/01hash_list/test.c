#include "hash.h"

int main(void)
{
	struct hash hash;
	hash_init(&hash);
		
	hash.build(&hash, "hello", "你好");
	hash.build(&hash, "welcome", "欢迎");
	hash.build(&hash, "china", "瓷器");
	hash.build(&hash, "Chinese kongfu", "中国功夫");
	hash.build(&hash, "UNIX", "操作系统");
	hash.build(&hash, "beijing", "中国首都");
	hash.build(&hash, "American", "美国");
	hash.build(&hash, "Indian", "印度阿三");
	hash.build(&hash, "china", "中国");
	hash.build(&hash, "hello", "你好");

	hash.for_each(&hash);

	printf("删除：\n");
	hash.del(&hash, "welcome");
	hash.for_each(&hash);

	printf("查找：\n");
	struct node *find = hash.search(&hash, "beijing");
	if (find) {
		printf("找到了，content是：%s\n", find->content);
	} else {
		printf("没找到！\n");
	}

	hash_destroy(&hash);

	return 0;
}
