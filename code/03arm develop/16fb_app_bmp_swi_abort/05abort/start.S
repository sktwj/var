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
	ldr pc, =data_abort_handler
	nop
	nop
	nop

reset:
	push {lr}

	//指定异常向量表基地址为本程序的首地址, a8手册P195
	//MRC p15, 0, r0, c12, c0, 0
	ldr r0, =0x40000000
	MCR p15, 0, r0, c12, c0, 0

	swi #123

	//故意访问0地址产生一次abort异常
	mov r0, #0
	str r0, [r0]

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
	push {r0-r3, lr}

	ldr r0, =fmt

	//取出swi指令的内容, 其中低24位就是swi指令的参数
	ldr r1, [lr, #-4]
	bic r1, #(0xff << 24)

	mov lr, pc
	ldr pc, =printf

	pop {r0-r3, pc}

data_abort_handler:
	//初始化abort模式下的栈，不同模式，sp寄存器是独立的，所以需要分别初始化才能使用
	ldr sp, =0x50000000

	//cpu提供的返回地址有偏差，偏差见a8手册P67，不过data abort模式的切换造成的偏差经测与手册不吻合
	sub lr, #4

	/* 压r0-r3是因为
	 * 在data_abort_handler中的指令使用和覆盖了部分寄存器的值，这些值保存着原SVC模式的运算过程，覆盖将导致回到SVC就无法继续运行
	 * 由于在data_abort_handler中调用了printf，按照ATPCS规范，printf有权利任意使用r0-r3，但对r4-r12需保证调用函数前后保持不变，而我们本身也没有动用r4-r12，所以只需对r0-r3压栈
	 */
	push {r0-r3, lr}

	ldr r0, =fmt_abort
	mov lr, pc
	ldr pc, =printf

	//尖角号表示将当前abort模式下的spsr自动覆盖到cpsr中，除了恢复原来模式下的运算状态外，同时也恢复了cpsr[4:0]模式位，所以模式将自动切回原模式
	//pop伪指令的尖角号不起作用，可能是bug，所以需要翻译成真实cpu汇编指令
	//pop {r0-r3, pc}^
	ldmfd sp!, {r0-r3, pc}^

.section .rodata
fmt:
	.asciz "swi number = %d\n"

fmt_abort:
	.asciz "data abort\n"