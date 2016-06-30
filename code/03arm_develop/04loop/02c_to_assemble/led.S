#define GPM4CON 0x110002e0
#define GPM4DAT 0x110002e4

.global led_init
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

.global led_on
led_on:
	mov r2, r0 //唯一的函数参数
	ldr r0, =GPM4DAT
	ldrb r1, [r0]
	bic r1, #0xf
	orr r1, r2
	strb r1, [r0]

	mov pc, lr