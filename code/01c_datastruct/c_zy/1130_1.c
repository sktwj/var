#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct info {
	char *name;
	int score;
};

#define LEN 10
void assign_value(struct info *ps, int n)
{
	int i = 0;
	for (; i < n; i++) {
		printf("请为数组的第%d个元素赋值：\n", i);
		(ps + i)->name = (char *)malloc(sizeof(char) * LEN);
		assert((ps + i)->name);
		printf("请输入姓名 和 成绩：\n");
		scanf("%s", (ps + i)->name);
		//fgets((ps + i)->name, LEN, stdin);
		//printf("请输入成绩：\n");
		scanf("%d", &(ps + i)->score);
	}
}

void save_to_file(struct info *ps, int n, char ** argv)
{
	FILE *fp = NULL;
	if (!(fp = fopen(argv[1], "a+"))) {
		fprintf(stderr, "打开要写入的文件失败！\n");
		exit (1);
	}	

	//方案1：先读取文件，如果能读到内容，判断每一个信息中的成绩，小于60分，删除，然后继续添加内容到文件； 如果文件内容为空，直接添加内容到文件

	//方案2：先继续添加内容到文件，然后将文件指针置回到文件开头，再次读取文件内容，将其中不及格的信息删除
	
	//方案3：在向文件添加内容时，如果即将添加的内容中的成绩不及格，不将此信息添加进文件，否则添加到文件

	//下面的程序是按照方案3来实现：
	int i = 0;
	while (i < n) {
		if ((ps + i)->score >= 60) {
			fputs((ps + i)->name, fp);
			putc('\n', fp);
			fprintf(fp, "%d", (ps + i)->score);
			putc('\n', fp);
		}
		i++;
	}

	fclose(fp);
}

int file_lines(FILE *fp)
{ 
	//判断文件有多少个换行符号		
	int count = 0, c;
	rewind(fp);
	while ((c = getc(fp)) != EOF) {
		if (c == '\n') {
			count++;
		}
	}
	return count;
}

int main(int argc, char **argv)
{
	int n;
	printf("请输入您想建立的数组的元素个数：");
	scanf("%d", &n);
	struct info s[n]; 
	assign_value(s, n); //为数组元素赋值

	int i = 0;
	printf("s数组元素是：\n");
	for (; i < n ; i++) {
		printf("%s: %d\n", (s + i)->name, (s + i)->score);
	}

	printf("---- next: \n");
	if (argc != 2) {
		printf("使用方法：可执行文件 保存到的文件\n");
		return 1;
	} else {
		save_to_file(s, n, argv); //将为结构体数组所赋值写入到文件中保存起来
	}

#define CNT 10
	struct info t[CNT] = {0};
	FILE *fp = NULL;
	fp = fopen(argv[1], "r");
	assert(fp);
	//FILE *fp1 = fp;
	if (file_lines(fp) >= CNT * 2) { //如果文件行数大于等于20行，给另一个结构体数组赋值
		
		rewind(fp);
		for (i = 0; i < CNT; i++) {
			(t + i)->name = (char *)malloc(CNT);
			assert((t + i)->name);
			fgets((t + i)->name, LEN, fp);
			char s[LEN];
			fgets(s, LEN, fp);
			(t + i)->score = atoi(s);
		}

		printf("t数组的元素为：\n");
		for (i = 0; i < CNT; i++) {
			printf("%s: \t%d\n", 
				(t + i)->name, (t + i)->score);
		}

		fclose(fp);
	} else { 
		printf("========= no =====\n");
	}
	return 0;
}
