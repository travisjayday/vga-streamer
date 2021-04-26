	.file	"bitperm.c"
	.text
	.p2align 4
	.globl	_print_bits
	.type	_print_bits, @function
_print_bits:
.LFB5619:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movl	%esi, %esi
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	testq	%rsi, %rsi
	je	.L2
	movq	%rdi, %rbp
	leaq	-1(%rsi), %rbx
	.p2align 4,,10
	.p2align 3
.L3:
	movl	%ebx, %ecx
	movq	%rbp, %rdi
	shrq	%cl, %rdi
	andl	$1, %edi
	addq	$48, %rdi
	call	putchar@PLT
	movq	%rbx, %rax
	subq	$1, %rbx
	testq	%rax, %rax
	jne	.L3
.L2:
	movq	stdout(%rip), %rsi
	addq	$8, %rsp
	.cfi_def_cfa_offset 24
	movl	$10, %edi
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	jmp	putc@PLT
	.cfi_endproc
.LFE5619:
	.size	_print_bits, .-_print_bits
	.p2align 4
	.globl	print_bits
	.type	print_bits, @function
print_bits:
.LFB5620:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rdi, %rbp
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	movl	$31, %ebx
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	.p2align 4,,10
	.p2align 3
.L11:
	movl	%ebx, %ecx
	movq	%rbp, %rdi
	shrq	%cl, %rdi
	andl	$1, %edi
	addq	$48, %rdi
	call	putchar@PLT
	subq	$1, %rbx
	jnb	.L11
	movq	stdout(%rip), %rsi
	addq	$8, %rsp
	.cfi_def_cfa_offset 24
	movl	$10, %edi
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	jmp	putc@PLT
	.cfi_endproc
.LFE5620:
	.size	print_bits, .-print_bits
	.p2align 4
	.globl	do_delta_swap
	.type	do_delta_swap, @function
do_delta_swap:
.LFB5621:
	.cfi_startproc
	endbr64
	movl	%esi, %ecx
	movq	%rdx, %r8
	movq	%rdi, %rdx
	movq	%rdi, %rax
	shrb	%cl
	shrq	%cl, %rdx
	xorq	%rdi, %rdx
	andq	%r8, %rdx
	xorq	%rdx, %rax
	salq	%cl, %rdx
	xorq	%rdx, %rax
	ret
	.cfi_endproc
.LFE5621:
	.size	do_delta_swap, .-do_delta_swap
	.p2align 4
	.globl	_perm_bitstring
	.type	_perm_bitstring, @function
_perm_bitstring:
.LFB5622:
	.cfi_startproc
	endbr64
	movl	%esi, %ecx
	movq	%rdi, %rax
	movq	%rdx, %rsi
	movl	%ecx, %r8d
	cmpb	$3, %cl
	jbe	.L16
	.p2align 4,,10
	.p2align 3
.L17:
	shrb	%cl
	movq	%rax, %rdx
	addq	$8, %rsi
	shrq	%cl, %rdx
	xorq	%rax, %rdx
	andq	-8(%rsi), %rdx
	movq	%rdx, %rdi
	salq	%cl, %rdi
	xorq	%rdi, %rdx
	xorq	%rdx, %rax
	cmpb	$3, %cl
	ja	.L17
	movl	$2, %edi
	.p2align 4,,10
	.p2align 3
.L18:
	movl	%edi, %ecx
	movq	%rax, %rdx
	addl	%edi, %edi
	addq	$8, %rsi
	shrb	%cl
	shrq	%cl, %rdx
	xorq	%rax, %rdx
	andq	-8(%rsi), %rdx
	movq	%rdx, %r9
	salq	%cl, %r9
	xorq	%r9, %rdx
	xorq	%rdx, %rax
	cmpb	%dil, %r8b
	jnb	.L18
	ret
	.p2align 4,,10
	.p2align 3
.L16:
	cmpb	$1, %cl
	ja	.L24
	ret
.L24:
	movq	%rdi, %rdx
	shrq	%rdx
	xorq	%rdi, %rdx
	andq	(%rsi), %rdx
	leaq	(%rdx,%rdx), %rcx
	xorq	%rcx, %rdx
	xorq	%rdx, %rax
	ret
	.cfi_endproc
.LFE5622:
	.size	_perm_bitstring, .-_perm_bitstring
	.p2align 4
	.globl	perm_bitstring
	.type	perm_bitstring, @function
perm_bitstring:
.LFB5623:
	.cfi_startproc
	endbr64
	movq	%rdi, %rax
	movq	%rdx, %rcx
	cmpb	$32, %sil
	je	.L29
	cmpb	$64, %sil
	je	.L30
	ret
	.p2align 4,,10
	.p2align 3
.L30:
	movq	%rdi, %rdx
	shrq	$32, %rdx
	xorq	%rdi, %rdx
	andq	(%rcx), %rdx
	xorq	%rdx, %rax
	salq	$32, %rdx
	xorq	%rdx, %rax
	movq	%rax, %rdx
	shrq	$16, %rdx
	xorq	%rax, %rdx
	andq	8(%rcx), %rdx
	xorq	%rdx, %rax
	salq	$16, %rdx
	xorq	%rdx, %rax
	movq	%rax, %rdx
	shrq	$8, %rdx
	xorq	%rax, %rdx
	andq	16(%rcx), %rdx
	xorq	%rdx, %rax
	movq	%rax, %rsi
	movq	%rdx, %rax
	salq	$8, %rax
	xorq	%rsi, %rax
	movq	%rax, %rdx
	shrq	$4, %rdx
	xorq	%rax, %rdx
	andq	24(%rcx), %rdx
	xorq	%rdx, %rax
	movq	%rax, %rsi
	movq	%rdx, %rax
	salq	$4, %rax
	xorq	%rsi, %rax
	movq	%rax, %rdx
	shrq	$2, %rdx
	xorq	%rax, %rdx
	andq	32(%rcx), %rdx
	xorq	%rdx, %rax
	movq	%rax, %rsi
	leaq	0(,%rdx,4), %rax
	xorq	%rsi, %rax
	movq	%rax, %rdx
	shrq	%rdx
	xorq	%rax, %rdx
	andq	40(%rcx), %rdx
	xorq	%rdx, %rax
	movq	%rax, %rsi
	leaq	(%rdx,%rdx), %rax
	xorq	%rsi, %rax
	movq	%rax, %rdx
	shrq	$2, %rdx
	xorq	%rax, %rdx
	andq	48(%rcx), %rdx
	xorq	%rdx, %rax
	movq	%rax, %rsi
	leaq	0(,%rdx,4), %rax
	xorq	%rsi, %rax
	movq	%rax, %rdx
	shrq	$4, %rdx
	xorq	%rax, %rdx
	andq	56(%rcx), %rdx
	xorq	%rdx, %rax
	movq	%rax, %rsi
	movq	%rdx, %rax
	salq	$4, %rax
	xorq	%rsi, %rax
	movq	%rax, %rdx
	shrq	$8, %rdx
	xorq	%rax, %rdx
	andq	64(%rcx), %rdx
	xorq	%rdx, %rax
	movq	%rax, %rsi
	movq	%rdx, %rax
	salq	$8, %rax
	xorq	%rsi, %rax
	movq	%rax, %rdx
	shrq	$16, %rdx
	xorq	%rax, %rdx
	andq	72(%rcx), %rdx
	xorq	%rdx, %rax
	movq	%rax, %rdi
	movq	%rdx, %rax
	salq	$16, %rax
	xorq	%rdi, %rax
	movq	%rax, %rdx
	shrq	$32, %rdx
	xorq	%rax, %rdx
	andq	80(%rcx), %rdx
	xorq	%rdx, %rax
	movq	%rax, %rdi
	movq	%rdx, %rax
	salq	$32, %rax
	xorq	%rdi, %rax
	ret
	.p2align 4,,10
	.p2align 3
.L29:
	shrq	$16, %rdi
	xorq	%rax, %rdi
	andq	(%rdx), %rdi
	xorq	%rdi, %rax
	salq	$16, %rdi
	xorq	%rdi, %rax
	movq	%rax, %rdx
	shrq	$8, %rdx
	xorq	%rax, %rdx
	andq	8(%rcx), %rdx
	xorq	%rdx, %rax
	salq	$8, %rdx
	xorq	%rdx, %rax
	movq	%rax, %rdx
	shrq	$4, %rdx
	xorq	%rax, %rdx
	andq	16(%rcx), %rdx
	xorq	%rdx, %rax
	movq	%rax, %rsi
	movq	%rdx, %rax
	salq	$4, %rax
	xorq	%rsi, %rax
	movq	%rax, %rdx
	shrq	$2, %rdx
	xorq	%rax, %rdx
	andq	24(%rcx), %rdx
	xorq	%rdx, %rax
	movq	%rax, %rsi
	leaq	0(,%rdx,4), %rax
	xorq	%rsi, %rax
	movq	%rax, %rdx
	shrq	%rdx
	xorq	%rax, %rdx
	andq	32(%rcx), %rdx
	xorq	%rdx, %rax
	movq	%rax, %rsi
	leaq	(%rdx,%rdx), %rax
	xorq	%rsi, %rax
	movq	%rax, %rdx
	shrq	$2, %rdx
	xorq	%rax, %rdx
	andq	40(%rcx), %rdx
	xorq	%rdx, %rax
	movq	%rax, %rsi
	leaq	0(,%rdx,4), %rax
	xorq	%rsi, %rax
	movq	%rax, %rdx
	shrq	$4, %rdx
	xorq	%rax, %rdx
	andq	48(%rcx), %rdx
	xorq	%rdx, %rax
	movq	%rax, %rsi
	movq	%rdx, %rax
	salq	$4, %rax
	xorq	%rsi, %rax
	movq	%rax, %rdx
	shrq	$8, %rdx
	xorq	%rax, %rdx
	andq	56(%rcx), %rdx
	xorq	%rdx, %rax
	movq	%rax, %rdi
	movq	%rdx, %rax
	salq	$8, %rax
	xorq	%rdi, %rax
	movq	%rax, %rdx
	shrq	$16, %rdx
	xorq	%rax, %rdx
	andq	64(%rcx), %rdx
	xorq	%rdx, %rax
	movq	%rax, %rdi
	movq	%rdx, %rax
	salq	$16, %rax
	xorq	%rdi, %rax
	ret
	.cfi_endproc
.LFE5623:
	.size	perm_bitstring, .-perm_bitstring
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
