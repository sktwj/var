#include <stdio.h>
#include <stdlib.h>

int main()
{
	#if 0
	printf("Content-type: text/html\r\n");
	printf("\r\n");
	#endif
	printf("<html>");
	printf("<head><title> %s </title></head>\r\n", "welcome to UPlooking");
	printf("<body>\n");
	printf("<h1> 这是标题栏 </h1>");
	printf("<pre> 内容,可以写下任何你想说的话.</pre>");
	printf("</body>");
	printf("</html>");

	return 0;
}
