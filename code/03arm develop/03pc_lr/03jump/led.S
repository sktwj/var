#define GPM4CON 0x110002e0
#define GPM4DAT 0x110002e4

.global _start
_start:

	mov lr, pc
	b led_init

	//mov lr, pc
	//b led_on
	bl led_on

loop:
	b loop

led_init:
	//GPM4CON = (GPM4CON & ~0xffff) | 0x1111;
	ldr r0, =GPM4CON
	ldr r1, [r0]
	bic r1, #0xff
	bic r1, #0xff00
	ldr r2, =0x1111
	orr r1, r2
	str r1, [r0]

	mov pc, lr

led_on:
	ldr r0, =GPM4DAT
	ldr r1, [r0]
	bic r1, #0xf
	orr r1, #0b1001
	str r1, [r0]

	mov pc, lr