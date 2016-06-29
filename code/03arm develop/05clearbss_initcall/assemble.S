.global _start
_start:

//1 cpu内部通用寄存器的赋值操作
mov r0, #100 //r0 = 100
mov r0, r1 //r0 = r1
mvn r0, #0xff //r0 = ~0xff

//arm cpu是RISC精简指令集，指令总数少，并且每条指令都是和cpu字长相等，32位cpu每条指令都是32位，其中一些指令需要传递常量数字，指令中只有12位来表示数字
//12位数字表示法采用8和4的组合,由8位有效位和4位移位数来表示一个数字

//2 非法立即数的赋值，伪指令ldr，cpu本身无法识别，该指令在编译时将由编译器转换成cpu可以识别的指令
ldr r0, =0x123456 //r0 = 0x123456

//3 访存操作，通过地址访问cpu外部资源
ldr r0, [r1] //r0 = *r1
str r0, [r1] //*r1 = r0

//原指令访问地址内容时采用是cpu字长的长度，我们也可以使用更精确的位长来访问，比如ldrb, strb, 采用8位方式访问地址内容

//以下扩展格式是ldr和str都可以使用的
ldr r0, [r1], #4 //r0 = *r1; r1 += 4
ldr r0, [r1, #4] //r0 = *(r1 + 4)
ldr r0, [r1, #-4]! //r0 = *(r1 - 4); r1 -= 4

//4 运算操作，以下指令的语法格式都是通用的
and r0, r1 //r0 &= r1
orr r0, #0x100 //r0 |= 0x100
add r0, r1, #0x200 //r0 = r1 + 0x200
sub r0, r1, r2 //r0 = r1 - r2
bic r0, #0xff //r0 &= ~0xff

//5 无限循环
loop:
	b loop

//6 PC寄存器（R15） process counter
//读取时得到当前正在执行的指令的内存地址+8
//对pc寄存器赋值，将造成指令跳转，写入时不需要加偏移量

//7 LR寄存器(R14): linker register
//专门用于保存函数的返回地址的, bl指令会自动对lr赋值

//8 压栈出栈的简单操作
push {r4-r6, lr}
//...
pop {r4-r6, pc}
//大括号内的寄存器列表必须按照寄存器号的顺序来书写

//9 条件判断
//9.1 条件判断的依据是cpsr[30]，ZERO位
//9.2 能够改变zero位的指令
cmp r0, r1 //(!(r0-r1)) ? cpsr[30] = 1: cpsr[30] = 0;
cmp r0, #0x100
tst r0, #0b11 //(!(r0 & 0b11)) ? cpsr[30] = 1: cpsr[30] = 0;
//普通指令加上后缀s时也能将运算结果更新到zero
subs r0, r1
adds r0, r1, #100
movs r0, r1
//9.3 如何判断使用zero位的结果
只需要在普通指令后加上eq或ne的后缀即可
eq: equal, 当zero为1时，执行该指令，否则不执行
ne: not equal, zero为0是执行指令，否则不执行
//9.4 loop example
	mov r0, #10
loop:
	subs r0, #1
	//loop content
	bne loop

//10 用于跳转的数字标签
1:
	//...
	b 1b //向上跳转
	//...
	b 2f
	b 1f //向下跳转
1:
2:

//11 ATPCS规范，描述arm编译器使用cpu资源的规范
//在函数中，只有r0-r3可以随便使用，必须保证r4-r12的值在退出函数时复原成进入函数时的样子
//函数的参数传递规则：前4个参数依次由r0-r3来传递，更多参数通过栈来传递。返回值由r0来传递
//只允许使用满降栈

//12 SP寄存器（R13）：stack pointer
//栈操作有两套命名方式，可以互相替代，归纳见《常用ARM指令集及汇编》P25
//以栈类型命名
//满降栈fd：full descend
//空升栈ea：empty ascend
//以及满升栈，空降栈
//以栈操作命名
//db(decrease before), da(decrease after), ib(increase before), ia(increase after)
//ATCPS规范规定的栈是满降栈
//入栈
push {r0-r3, lr}
stmfd sp!, {r0-r3, lr}
stmdb sp!, {r0-r3, lr}
//出栈
pop {r0-r3, pc}
ldmfd sp!, {r0-r3, pc}
ldmia sp!, {r0-r3, pc}