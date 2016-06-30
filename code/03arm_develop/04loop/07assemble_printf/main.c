static int (*printf)(const char *, ...) = (int (*)(const char *, ...))0xc3e11bc0;

void main()
{
	static int data[20];

	printf("%s %s: 0x%x %d %c\n", __FILE__, __FUNCTION__, 0x12345678, data[0], 'X');
}
