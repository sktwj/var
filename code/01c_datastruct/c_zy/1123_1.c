#include <stdio.h>

union u_e {
	int a;
	char s[4];
};

#define SWAP_ENDIAN(n) \
   ({char tmp = (n)->s[0]; \
	(n)->s[0] = (n)->s[3]; \
	(n)->s[3] = tmp; \
	\
	tmp = (n)->s[1]; \
	(n)->s[1] = (n)->s[2]; \
	(n)->s[2] = tmp; \
	})	

int main(void)
{
	union u_e a = {0x12345678}; //--> 0x78563412

	SWAP_ENDIAN(&a);

	printf("a = %#x\n", a);

	return 0;
}
