#include "config.h"

void main()
{
	static int data[20] = {0};

	printf("%s %s: data[0]=%d, data[19]=%d\n", __FILE__, __FUNCTION__, data[0], data[19]);
}
