#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//获得两次投掷的点数之和
int get_points(void)
{
	int point1, point2;
	time_t t = time(NULL); //是将返回的秒数作为rand函数的初值
	srand((size_t)t);
	point1 = rand()%6 + 1;
	point2 = rand()%6 + 1;

	return point1 + point2;	
}

enum status {
	WIN, LOSE, CONTINUE,
};

void play_game(void) //游戏过程描述
{
	int first_points; 
	first_points = get_points();
	printf("first_points = %d\n", first_points);

	enum status state;
	switch (first_points) {
		case 7:
		case 11:
			state = WIN;
			break;
		case 2:
		case 3:
		case 12:
			state = LOSE;
			break;
		default:
			state = CONTINUE;
	}
	
	int next_points;
	while (state == CONTINUE) {
		sleep(1);
		next_points = get_points();
		printf("next_points = %d\n", next_points);

		if (next_points == 7) {
			state = LOSE;
			break;
		} else if (next_points == first_points) {
			state = WIN;
			break;
		}
	}

	if (state == WIN) {
		printf("玩家赢了！\n");
	} else {
		printf("玩家输了！\n");
	}
}

int main(void)
{
	play_game();
	return 0;
}
