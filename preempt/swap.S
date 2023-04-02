
#define oRBX		0x80
#define oRBP		0x78
#define oR12		0x48
#define oR13		0x50
#define oR14		0x58
#define oR15		0x60
#define oRDI		0x68
#define oRSI		0x70
#define oRDX		0x88
#define oRCX		0x98
#define oR8		0x28
#define oR9		0x30
#define oRIP		0xa8
#define oRSP		0xa0
#define oFPREGSMEM	0x1a8
#define oFPREGS		0xe0
#define oMXCSR		0x1c0


.section .note.GNU-stack
.text
.align 4
.globl m_setcontext
.type m_setcontext, @function

m_setcontext:
	movq	oRSP(%rdi), %rsp
	movq	oRBX(%rdi), %rbx
	movq	oRBP(%rdi), %rbp
	movq	oR12(%rdi), %r12
	movq	oR13(%rdi), %r13
	movq	oR14(%rdi), %r14
	movq	oR15(%rdi), %r15

	/* The following ret should return to the address set with
	getcontext.  Therefore push the address on the stack. */
	movq	oRIP(%rdi), %rcx
	pushq	%rcx

	/* Setup registers used for passing args.  */
	movq	oRDI(%rdi), %rdi
	movq	oRDX(%rdi), %rdx
	movq	oRCX(%rdi), %rcx
	movq	oR8(%rdi), %r8
	movq	oR9(%rdi), %r9

	movq	oRIP(%rdi), %r10

	/* Setup finally  %rsi.
	movq	oRSI(%rdi), %rsi */

	// jmpq %r10


	/* Clear rax to indicate success.  */
	xorl	%eax, %eax

	ret
