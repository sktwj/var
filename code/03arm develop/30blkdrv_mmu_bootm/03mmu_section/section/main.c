#include "config.h"

void start_armboot()
{
	printk("%s, %d, %s\n", __FUNCTION__, __LINE__, __FILE__);

	led_test();

	while (1) {}
}