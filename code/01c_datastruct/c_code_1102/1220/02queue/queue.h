#include "list.h"

//描述队列的类型
struct queue {
	struct list_head head; 

	void (*push)(struct list_head *, struct list_head *); //入队
	struct list_head *(*pop)(struct queue *, 
			void (*)(struct list_head *)); //出队
	struct list_head *(*top)(struct queue *); //访问队头元素
	int (*is_empty)(struct queue *); //判断队列是否为空
};

void queue_init(struct queue *);
void queue_destroy(struct queue *, void (*)(struct list_head *));
