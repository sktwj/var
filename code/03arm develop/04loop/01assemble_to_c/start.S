.global _start
_start:
	bl led_init
	bl key_init

loop:
	bl key_stat
	bl led_on
	b loop
