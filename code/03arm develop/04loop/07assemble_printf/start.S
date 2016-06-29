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

	bl main

	pop {pc}

.section .rodata
fmt:
	.asciz "bss start = 0x%x, bss end = 0x%x\n"