#define GPM4CON 0x110002e0
#define GPM4DAT 0x110002e4

.global _start
_start:
	//GPM4CON = (GPM4CON & ~0xffff) | 0x1111;
	ldr r0, =GPM4CON
	ldr r1, [r0]
	bic r1, #0xff
	bic r1, #0xff00
	ldr r2, =0x1111
	orr r1, r2
	str r1, [r0]

	ldr r1, [r0, #4] //GPM4DAT
	bic r1, #0xf
	orr r1, #0b1001
	str r1, [r0, #4]

loop:
	b loop


#if 0
#define GPM4CON (*(volatile u32*)0x110002e0)
#define GPM4DAT (*(volatile u8*)0x110002e4)

void _start(void)
{
	//将GPM4_0-3这4个管脚设为output
	GPM4CON = (GPM4CON & ~0xffff) | 0x1111;

	//GPM4_0-3清零，4灯全亮
	GPM4DAT &= ~0xf;
}
#endif