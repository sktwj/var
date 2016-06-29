#define TTB 0x46000000 //页表首地址

.global _start
_start:
	//push {lr}

	//uboot把栈指针定位在虚拟地址，所以在mmu关闭前，需要重新定位到物理地址上
	ldr sp, =0x45000000

	ldr r0, =TTB
	bl create_translation_table

	//将domain全部设为manager，这样在页表项中就无需填充AP和XN权限管理位，而直接将权限全开
	bl set_domain_all_manager

	//填充页表首地址到TTBR0中，并由TTBCR决定mmu始终使用TTBR0的值作为页表首地址
	ldr r0, =TTB
	bl set_ttb

	bl mmu_on





	bl clear_bss
	bl do_initcall
	bl start_armboot

	//pop {pc}

clear_bss:
	push {lr}

	ldr r0, =__bss_start__
	ldr r1, =__bss_end__
	mov r2, #0

	//排除bss段为空的情况
	cmp r0, r1
	beq 2f

1:
	str r2, [r0]
	add r0, #4
	cmp r0, r1
	bne 1b

2:
	pop {pc}

do_initcall:
	push {r4, r5, lr}

	ldr r4, =_INITCALL_START
	ldr r5, =_INITCALL_END

	//排除initcall段为空的情况
	cmp r4, r5
	beq 2f

1:
	ldr r0, [r4]
	mov lr, pc
	mov pc, r0

	add r4, #4
	cmp r4, r5
	bne 1b

2:
	pop {r4, r5, pc}


//============mmu===============
//a8手册P122

.global mmu_off
mmu_off:
	push {lr}

	MRC p15, 0, r0, c1, c0, 0
	bic r0, #1
	MCR p15, 0, r0, c1, c0, 0

	pop {pc}

.global mmu_on
mmu_on:
	push {lr}

	MRC p15, 0, r0, c1, c0, 0
	orr r0, #1
	MCR p15, 0, r0, c1, c0, 0

	pop {pc}

set_domain_all_manager:
	push {lr}

	mvn r0, #0
	MCR p15, 0, r0, c3, c0, 0

	pop {pc}

set_ttb:
	push {lr}

	//开启页表缓冲
	orr r0, #(1 << 3 | 1 << 1 | 1 << 0)
	//将页表首地址写入TTBR0,因为TTBCR.N默认为0，所以mmu始终使用TTBR0作为页表首地址, a8手册P135
	MCR p15, 0, r0, c2, c0, 0

	pop {pc}

.global irq_off
irq_off:
	push {lr}

	mrs r0, cpsr
	orr r0, #(1 << 7)
	msr cpsr, r0

	pop {pc}