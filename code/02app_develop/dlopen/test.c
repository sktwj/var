#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

int main()
{
	void *handler = NULL;
	double (*cosine)(double) = NULL;
	void *error = NULL;

	handler = dlopen("libm.so", RTLD_LAZY);
	if (handler == NULL) {
		fprintf(stderr, "Error: dlopen failed.(%s)\n", dlerror());
		return -1;
	}

	//clear err output buffer
	dlerror();

	*(void **)(&cosine) = dlsym(handler, "cos");
	if (cosine == NULL) {
		fprintf(stderr, "Error: dlsy failed.(%s)\n", dlerror());
		dlclose(handler);
		return -1;
	}
	
	//function call
	printf("cosine(2.0) = %.2lf\n", cosine(2.0));

	dlclose(handler);

	return 0;
}
