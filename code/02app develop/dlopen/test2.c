#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

#define  CALC_SO	"./calc.so"

int main()
{
	void *handler = NULL;
	int (*myadd)(int, int) = NULL;
	int (*mysub)(int, int) = NULL;
	void *error = NULL;

	handler = dlopen(CALC_SO, RTLD_LAZY);
	if (handler == NULL) {
		fprintf(stderr, "Error: dlopen failed.(%s)\n", dlerror());
		return -1;
	}

	//clear err output buffer
	dlerror();

	*(void **)(&myadd) = dlsym(handler, "add");
	if (myadd == NULL) {
		fprintf(stderr, "Error: dlsy failed.(%s)\n", dlerror());
		dlclose(handler);
		return -1;
	}
	*(void **)(&mysub) = dlsym(handler, "sub");
	if (mysub == NULL) {
		fprintf(stderr, "Error: dlsy failed.(%s)\n", dlerror());
		dlclose(handler);
		return -1;
	}

	
	//function call
	printf("myadd(7, 4) = %d\n", myadd(7, 4));
	printf("mysub(7, 4) = %d\n", mysub(7, 4));

	dlclose(handler);

	return 0;
}
