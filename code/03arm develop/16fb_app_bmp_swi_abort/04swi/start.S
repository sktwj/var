#define printf 0xc3e11bcc

.section .text

.global _start
_start:
	//异常向量表（Exception vectors），cpu发生异常时会自动跳转到异常向量表中对应的地址上执行该地址上的指令
	//异常向量表写在程序头部只是出于习惯
	//异常向量表的格式见arm v7手册P1149

	b reset
	nop
	ldr pc, =swi_handler
	nop
	nop
	nop
	nop
	nop

reset:
	push {lr}

	//指定异常向量表基地址为本程序的首地址, a8手册P195
	//MRC p15, 0, r0, c12, c0, 0
	ldr r0, =0x40000000
	MCR p15, 0, r0, c12, c0, 0

	//操作数为24位数字，保存在指令本身的低24位上
	swi #123

	bl clear_bss
	bl do_initcall
	bl main

	pop {pc}

clear_bss:
	push {lr}

	//从链接脚本中提取bss段的首末地址
	ldr r0, =_bss_start
	ldr r1, =_bss_end
	mov r2, #0
	//如果bss为空，不进入循环
	cmp r0, r1
	beq 2f
	//循环清空整个bss段，r0将遍历整个bss段
1:
	str r2, [r0]
	add r0, #4
	cmp r0, r1
	bne 1b
2:

	pop {pc}

do_initcall:
	push {r4-r5, lr}

	//不能使用r0-r3来做循环，因为每经过一个函数调用，r0-r3的值都可能会被函数修改

	//从链接脚本中提取initcall段的首末地址
	ldr r4, =_initcall_start
	ldr r5, =_initcall_end
	//如果bss为空，不进入循环
	cmp r4, r5
	beq 2f

	//循环遍历整个initcall段
1:
	mov lr, pc
	ldr pc, [r4]

	add r4, #4
	cmp r4, r5
	bne 1b
2:

	pop {r4-r5, pc}

swi_handler:
	//lr由cpu自行赋值，保存触发本次异常的swi指令的下一条指令地址
	push {r0-r3, lr}

	ldr r0, =fmt

	//取出swi指令的内容, 其中低24位就是swi指令的参数
	ldr r1, [lr, #-4]
	bic r1, #(0xff << 24)

	mov lr, pc
	ldr pc, =printf

	pop {r0-r3, pc}

.section .rodata
fmt:
	.asciz "swi number = %d\n"