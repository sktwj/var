#include <stdio.h>
#include "queue.h"

int main(void)
{
	struct queue queue; //定义队列变量
	queue_init(&queue);
#define CNT 10
	struct qnode s[CNT];

	int i = 0;
	for (; i < CNT; i++) {
		(s + i)->data = i;
		LIST_INIT(&(s + i)->list);

		queue.push(&queue, (s + i)->data);
	}

	printf("队头元素是：%d\n", queue.top(&queue));

	printf("出队的顺序：");
	while (!queue.is_empty(&queue)) {
		printf("%d ", queue.pop(&queue));
	}
	printf("\n");

	queue_destroy(&queue);

	return 0;
}
