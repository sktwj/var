// 字符串反转 中英文
#include <stdio.h>
#include <string.h>

void reverse(char *str)
{
    printf("here %s\n", str);
    char *p = str;
    int len = 0;
    while(*p!='\0')
    {
        p++;
        len++;
    }
    p--;
    while (len)
    {
        if (*p < 0)
        {
            printf("%c%c%c", *(p-2), *(p-1), *p);
            p-=3;
            len-=3;
        }
        else{
            printf("%c", *p);
            p--;
            len--;
            }
    }
    printf("\n");

}

#if 1
int main(int argc, char *argv[])

{
    char ss[] = "你好sb";
    int i=0;
    printf("%d\n", sizeof(ss));
    for (; i<strlen(ss); i++)
        printf("%c:%d\n", ss[i], ss[i]);
    if (argc > 1)
        reverse(argv[1]);   

}
#endif
#if 0 
char *str="汉字里有english也能正确显hehe";
 
int main() {
  int i = strlen(str) - 1;
  while (i>=0) {
   
        if(str[i] < 0)
        {
            printf("|%d", str[i]);
            printf("%c%c%c|", str[i-2], str[i-1],str[i]);
            i = i-2;
        }else{
            printf("<%d>", str[i]);
            printf("%c", str[i]);
        }
        i--;

  }
printf("\n");
  return 0;
}
#endif 
