#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>


int main(void)
{
    int fd1 = open("./test.c", O_RDONLY);

    if (fd1 < 0){
        printf("open file failed\n"); 
        return -1;
    }

    char c;
    /*lseek(fd1, 100, SEEK_SET);*/
    while(read(fd1, &c, 1)){
        write(1, &c, 1); 
    }

    printf("file pos = %d\n", lseek(fd1, 0, SEEK_CUR));
    close(fd1);

}
