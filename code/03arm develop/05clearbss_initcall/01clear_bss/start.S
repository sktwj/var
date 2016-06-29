#define printf 0xc3e11bc0

.section .text

.global _start
_start:
	push {lr}

	ldr r0, =fmt
	ldr r1, =__bss_start__
	ldr r2, =__bss_end__

	mov lr, pc
	ldr pc, =printf

	bl clear_bss

	bl main

	pop {pc}

clear_bss:
	push {lr}

	ldr r0, =__bss_start__
	ldr r1, =__bss_end__
	mov r2, #0

	//排除bss段为空的特殊情况
	cmp r0, r1
	beq 2f
1:
	str r2, [r0]

	add r0, #4
	cmp r0, r1
	bne 1b
2:
	pop {pc}

.section .rodata
fmt:
	.asciz "bss start = 0x%x, bss end = 0x%x\n"