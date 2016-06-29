static int (*printf)(const char *, ...) = (int (*)(const char *, ...))0xc3e11bc0;

void main()
{
	static int data[20] = {0};

	printf("%s %s: data[0]=%d, data[19]=%d\n", __FILE__, __FUNCTION__, data[0], data[19]);
}
