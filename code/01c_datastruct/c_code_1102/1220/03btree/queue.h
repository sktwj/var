#pragma once

#include "btree.h"
#include "list.h"

typedef struct bnode * qelem_t;
//typedef int qelem_t;
struct qnode {
	qelem_t data;
	struct list_head list;
};

//描述队列的类型
struct queue {
	struct list_head head; 

	void (*push)(struct queue *, qelem_t); //入队
	qelem_t (*pop)(struct queue *); //出队
	qelem_t (*top)(struct queue *); //访问队头元素
	int (*is_empty)(struct queue *); //判断队列是否为空
};

void queue_init(struct queue *);
void queue_destroy(struct queue *);
