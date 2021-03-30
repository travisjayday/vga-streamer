	.file	"permgen.c"
	.text
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC1:
	.string	"Forcing %d-bit masks"
	.text
	.p2align 4
	.globl	force
	.type	force, @function
force:
.LFB51:
	.cfi_startproc
	endbr64
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	movl	%edi, %edx
	leaq	.LC1(%rip), %rsi
	xorl	%eax, %eax
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	movl	$1, %r12d
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	movl	%edi, %ebx
	movl	$1, %edi
	movl	%ebx, %r13d
	leal	1(%rbx), %ebp
	shrl	$31, %r13d
	movslq	%ebp, %rbp
	addl	%ebx, %r13d
	subq	$24, %rsp
	.cfi_def_cfa_offset 80
	call	__printf_chk@PLT
	sarl	%r13d
	movq	%rbp, %rdi
	leal	-1(%r13), %r15d
	movl	%r15d, %ecx
	sall	%cl, %r12d
	movl	%r12d, 12(%rsp)
	call	malloc@PLT
	movq	%rax, %r12
	testl	%ebx, %ebx
	jle	.L2
	leal	-1(%rbx), %eax
	cmpl	$14, %eax
	jbe	.L12
	movl	%ebx, %edx
	movq	%r12, %rax
	movdqa	.LC0(%rip), %xmm3
	movdqa	.LC2(%rip), %xmm9
	shrl	$4, %edx
	movdqa	.LC4(%rip), %xmm7
	movdqa	.LC3(%rip), %xmm8
	salq	$4, %rdx
	movdqa	.LC5(%rip), %xmm6
	movdqa	.LC6(%rip), %xmm4
	movdqa	.LC7(%rip), %xmm5
	addq	%r12, %rdx
	.p2align 4,,10
	.p2align 3
.L4:
	movdqa	%xmm3, %xmm2
	addq	$16, %rax
	paddd	%xmm9, %xmm3
	movdqa	%xmm2, %xmm10
	movdqa	%xmm2, %xmm0
	movdqa	%xmm2, %xmm1
	paddd	%xmm8, %xmm10
	punpcklwd	%xmm10, %xmm0
	punpckhwd	%xmm10, %xmm1
	movdqa	%xmm0, %xmm10
	punpcklwd	%xmm1, %xmm0
	punpckhwd	%xmm1, %xmm10
	movdqa	%xmm2, %xmm1
	paddd	%xmm6, %xmm2
	paddd	%xmm7, %xmm1
	punpcklwd	%xmm10, %xmm0
	movdqa	%xmm1, %xmm10
	punpcklwd	%xmm2, %xmm1
	pand	%xmm4, %xmm0
	punpckhwd	%xmm2, %xmm10
	movdqa	%xmm1, %xmm2
	punpckhwd	%xmm10, %xmm2
	punpcklwd	%xmm10, %xmm1
	punpcklwd	%xmm2, %xmm1
	pand	%xmm4, %xmm1
	packuswb	%xmm1, %xmm0
	paddb	%xmm5, %xmm0
	movups	%xmm0, -16(%rax)
	cmpq	%rdx, %rax
	jne	.L4
	movl	%ebx, %eax
	andl	$-16, %eax
	testb	$15, %bl
	je	.L5
.L3:
	movslq	%eax, %rdx
	leal	65(%rax), %esi
	movb	%sil, (%r12,%rdx)
	leal	1(%rax), %edx
	cmpl	%edx, %ebx
	jle	.L5
	movslq	%edx, %rsi
	addl	$65, %edx
	movb	%dl, (%r12,%rsi)
	leal	2(%rax), %edx
	cmpl	%edx, %ebx
	jle	.L5
	movslq	%edx, %rsi
	addl	$65, %edx
	movb	%dl, (%r12,%rsi)
	leal	3(%rax), %edx
	cmpl	%edx, %ebx
	jle	.L5
	movslq	%edx, %rsi
	addl	$65, %edx
	movb	%dl, (%r12,%rsi)
	leal	4(%rax), %edx
	cmpl	%edx, %ebx
	jle	.L5
	movslq	%edx, %rsi
	addl	$65, %edx
	movb	%dl, (%r12,%rsi)
	leal	5(%rax), %edx
	cmpl	%edx, %ebx
	jle	.L5
	movslq	%edx, %rsi
	addl	$65, %edx
	movb	%dl, (%r12,%rsi)
	leal	6(%rax), %edx
	cmpl	%edx, %ebx
	jle	.L5
	movslq	%edx, %rsi
	addl	$65, %edx
	movb	%dl, (%r12,%rsi)
	leal	7(%rax), %edx
	cmpl	%edx, %ebx
	jle	.L5
	movslq	%edx, %rsi
	addl	$65, %edx
	movb	%dl, (%r12,%rsi)
	leal	8(%rax), %edx
	cmpl	%edx, %ebx
	jle	.L5
	movslq	%edx, %rsi
	addl	$65, %edx
	movb	%dl, (%r12,%rsi)
	leal	9(%rax), %edx
	cmpl	%edx, %ebx
	jle	.L5
	movslq	%edx, %rsi
	addl	$65, %edx
	movb	%dl, (%r12,%rsi)
	leal	10(%rax), %edx
	cmpl	%edx, %ebx
	jle	.L5
	movslq	%edx, %rsi
	addl	$65, %edx
	movb	%dl, (%r12,%rsi)
	leal	11(%rax), %edx
	cmpl	%edx, %ebx
	jle	.L5
	movslq	%edx, %rsi
	addl	$65, %edx
	movb	%dl, (%r12,%rsi)
	leal	12(%rax), %edx
	cmpl	%edx, %ebx
	jle	.L5
	movslq	%edx, %rsi
	addl	$65, %edx
	movb	%dl, (%r12,%rsi)
	leal	13(%rax), %edx
	cmpl	%edx, %ebx
	jle	.L5
	movslq	%edx, %rsi
	addl	$14, %eax
	addl	$65, %edx
	movb	%dl, (%r12,%rsi)
	cmpl	%eax, %ebx
	jle	.L5
	movslq	%eax, %rdx
	addl	$65, %eax
	movb	%al, (%r12,%rdx)
	.p2align 4,,10
	.p2align 3
.L5:
	movslq	%ebx, %rax
	addq	%r12, %rax
.L2:
	movb	$0, (%rax)
	movq	%rbp, %rdi
	xorl	%r14d, %r14d
	call	malloc@PLT
	movq	%rax, %rdi
	.p2align 4,,10
	.p2align 3
.L7:
	movq	%rbp, %rdx
	movq	%r12, %rsi
	call	memcpy@PLT
	movq	%rax, %rdi
	xorl	%eax, %eax
	cmpl	$1, %ebx
	jle	.L10
	.p2align 4,,10
	.p2align 3
.L6:
	movl	%r15d, %edx
	subl	%eax, %edx
	btl	%edx, %r14d
	jnc	.L9
	leal	0(%r13,%rax), %edx
	movzbl	(%rdi,%rax), %esi
	movslq	%edx, %rdx
	addq	%rdi, %rdx
	movzbl	(%rdx), %r8d
	movb	%r8b, (%rdi,%rax)
	movb	%sil, (%rdx)
.L9:
	addq	$1, %rax
	cmpl	%eax, %r13d
	jg	.L6
.L10:
	addl	$1, %r14d
	testl	%r14d, 12(%rsp)
	je	.L7
	addq	$24, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 56
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	ret
.L12:
	.cfi_restore_state
	xorl	%eax, %eax
	jmp	.L3
	.cfi_endproc
.LFE51:
	.size	force, .-force
	.section	.text.startup,"ax",@progbits
	.p2align 4
	.globl	main
	.type	main, @function
main:
.LFB52:
	.cfi_startproc
	endbr64
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	movl	$64, %edi
	call	force
	xorl	%eax, %eax
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE52:
	.size	main, .-main
	.section	.rodata.cst16,"aM",@progbits,16
	.align 16
.LC0:
	.long	0
	.long	1
	.long	2
	.long	3
	.align 16
.LC2:
	.long	16
	.long	16
	.long	16
	.long	16
	.align 16
.LC3:
	.long	4
	.long	4
	.long	4
	.long	4
	.align 16
.LC4:
	.long	8
	.long	8
	.long	8
	.long	8
	.align 16
.LC5:
	.long	12
	.long	12
	.long	12
	.long	12
	.align 16
.LC6:
	.value	255
	.value	255
	.value	255
	.value	255
	.value	255
	.value	255
	.value	255
	.value	255
	.align 16
.LC7:
	.byte	65
	.byte	65
	.byte	65
	.byte	65
	.byte	65
	.byte	65
	.byte	65
	.byte	65
	.byte	65
	.byte	65
	.byte	65
	.byte	65
	.byte	65
	.byte	65
	.byte	65
	.byte	65
	.ident	"GCC: (Ubuntu 10.2.0-13ubuntu1) 10.2.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	 1f - 0f
	.long	 4f - 1f
	.long	 5
0:
	.string	 "GNU"
1:
	.align 8
	.long	 0xc0000002
	.long	 3f - 2f
2:
	.long	 0x3
3:
	.align 8
4:
