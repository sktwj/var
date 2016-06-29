#define GPM4CON 0x110002e0
#define GPM4DAT 0x110002e4

.global _start
_start:
	push {r4-r5, lr}

	bl led_init

	mov r4, #1 //led_status
	mov r5, #20 //count
1:
	mvn r0, r4
	bl led_on

	bl delay

	cmp r4, #0b1000
	moveq r4, #1
	movne r4, r4, lsl #1

	subs r5, #1
	bne 1b

	pop {r4-r5, pc}

led_init:
	push {lr}

	//GPM4CON = (GPM4CON & ~0xffff) | 0x1111;
	ldr r0, =GPM4CON
	ldr r1, [r0]
	bic r1, #0xff
	bic r1, #0xff00
	ldr r2, =0x1111
	orr r1, r2
	str r1, [r0]

	pop {pc}

led_on:
	push {lr}

	and r0, #0xf
	mov r2, r0 //唯一的函数参数
	ldr r0, =GPM4DAT
	ldrb r1, [r0]
	bic r1, #0xf
	orr r1, r2
	strb r1, [r0]

	pop {pc}

delay:
	push {lr}

	ldr r0, =0xfffffff
1:
	subs r0, #1
	bne 1b

	pop {pc}