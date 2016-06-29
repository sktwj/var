#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

#define  BUF_SIZE	100

static char* page_template =
"<html>\n"
" <head>\n"
" <title> Music Player </title>"
" </head>\n"
" <body>\n"
" <h2>Now it Playing Music: %s.</h2>\n"
" </body>\n"
"</html>\n";

#define  MP3_FILE	"./1.mp3"

//int main()
void module_generate (int fd)
{

	char buf[BUF_SIZE] = {0};
	int n = snprintf(buf, BUF_SIZE, page_template, MP3_FILE);
	write(fd, buf, n);
	//fflush(fd);

	int pipe_fd[2];

	int iret = pipe(pipe_fd);

	pid_t p = fork();
	if (p == -1){
		perror("fork");
		return ;
	} else if (p == 0) {
		//子京城
		close(pipe_fd[1]);
		dup2(pipe_fd[0], STDIN_FILENO);
		execlp("mpg123", "mpg123", "-q", "-", NULL);
		fprintf(stderr, "execlp failed\n");
		exit(-1);
	} else {
		int fd = open("1.mp3", O_RDONLY);
		if (fd == -1) {
			perror("open failed.");
			return;
		}

		while ((iret = read(fd, buf, BUF_SIZE)) != 0) {
			write(pipe_fd[1], buf, iret);
		}
		printf("play done\n");
	}

	wait(NULL);
}
