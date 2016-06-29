#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

void get_file_perm(const struct stat *pstat)
{
	char  perm[10] = {0};
	memset(perm, '-', 9);
	if (pstat->st_mode & S_IRUSR) {
		perm[0] = 'r';
	}
	if (pstat->st_mode & S_IWUSR) {
		perm[1] = 'w';
	}
	if (pstat->st_mode & S_IXUSR) {
		perm[2] = 'x';
	}
	/* .... */
	printf("%s", perm);
}


void get_file_type(const struct stat *pstat)
{
	switch (pstat->st_mode & S_IFMT) {
		case S_IFIFO:
				printf("p");
				break;
		case S_IFCHR:
				printf("c");
				break;
		case S_IFDIR:
				printf("d");
				break;
		case S_IFBLK:
				printf("b");
				break;
		case S_IFREG:
				printf("-");
				break;
		case S_IFLNK:
				printf("l");
				break;
		case S_IFSOCK:
				printf("s");
				break;
		default:
				fprintf(stderr, "unkown type\n");
				break;
	}
}

int ls_l_demo(const char *path)
{
	struct stat  f_stat;
	
	int iret;
	iret = stat(path, &f_stat);
	if (iret < 0) {
		perror("stat");
		return -1;
	}
	
	get_file_type(&f_stat);
	get_file_perm(&f_stat);
	printf("  ");
	printf("%d ", f_stat.st_nlink);
	printf("%d  %d  ", f_stat.st_uid, f_stat.st_gid);
	printf("%lld ", f_stat.st_size);
	printf("%s ", ctime(&f_stat.st_mtime));
	printf("%s\n", path);
	return 0;	
}

int main()
{
	ls_l_demo("1.txt");

	return 0;
}
