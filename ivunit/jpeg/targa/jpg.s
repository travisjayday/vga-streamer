	.text
	.file	"jpg.c"
	.globl	load_tga                        # -- Begin function load_tga
	.p2align	4, 0x90
	.type	load_tga,@function
load_tga:                               # @load_tga
	.cfi_startproc
# %bb.0:
	pushq	%r14
	.cfi_def_cfa_offset 16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	pushq	%rax
	.cfi_def_cfa_offset 32
	.cfi_offset %rbx, -24
	.cfi_offset %r14, -16
	movq	%rsi, %r14
	movq	%rdi, %rbx
	callq	tga_read
	testl	%eax, %eax
	jne	.LBB0_10
# %bb.1:
	movzwl	14(%rbx), %esi
	movzwl	16(%rbx), %edx
	movzbl	18(%rbx), %ecx
	movl	$.L.str.1, %edi
	movq	%r14, %r8
	xorl	%eax, %eax
	callq	printf
	movq	%rbx, %rdi
	callq	tga_is_mono
	testl	%eax, %eax
	jne	.LBB0_3
# %bb.2:
	movq	%rbx, %rdi
	callq	tga_desaturate_rec_601_1
	testl	%eax, %eax
	jne	.LBB0_10
.LBB0_3:
	movq	%rbx, %rdi
	callq	tga_is_top_to_bottom
	testl	%eax, %eax
	jne	.LBB0_5
# %bb.4:
	movq	%rbx, %rdi
	callq	tga_flip_vert
	testl	%eax, %eax
	jne	.LBB0_10
.LBB0_5:
	movq	%rbx, %rdi
	callq	tga_is_right_to_left
	testl	%eax, %eax
	je	.LBB0_7
# %bb.6:
	movq	%rbx, %rdi
	callq	tga_flip_horiz
	testl	%eax, %eax
	jne	.LBB0_10
.LBB0_7:
	testb	$7, 14(%rbx)
	jne	.LBB0_11
# %bb.8:
	testb	$7, 16(%rbx)
	jne	.LBB0_11
# %bb.9:
	addq	$8, %rsp
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%r14
	.cfi_def_cfa_offset 8
	retq
.LBB0_10:
	.cfi_def_cfa_offset 32
	movl	%eax, %edi
	callq	tga_error
	movl	$.L.str, %edi
	movq	%rax, %rsi
	xorl	%eax, %eax
	callq	printf
	movl	$1, %edi
	callq	exit
.LBB0_11:
	movl	$.Lstr, %edi
	callq	puts
	movl	$1, %edi
	callq	exit
.Lfunc_end0:
	.size	load_tga, .Lfunc_end0-load_tga
	.cfi_endproc
                                        # -- End function
	.section	.rodata.cst8,"aM",@progbits,8
	.p2align	3                               # -- Begin function dct8
.LCPI1_0:
	.quad	0x3fd872b020c49ba6              # double 0.38200000000000001
.LCPI1_1:
	.quad	0x3fe69fbe76c8b439              # double 0.70699999999999996
.LCPI1_2:
	.quad	0x3fe14fdf3b645a1d              # double 0.54100000000000004
.LCPI1_3:
	.quad	0x3ff4e5604189374c              # double 1.306
.LCPI1_4:
	.quad	0x3fd0000000000000              # double 0.25
.LCPI1_5:
	.quad	0x3fc0000000000000              # double 0.125
	.text
	.globl	dct8
	.p2align	4, 0x90
	.type	dct8,@function
dct8:                                   # @dct8
	.cfi_startproc
# %bb.0:
                                        # kill: def $ecx killed $ecx def $rcx
                                        # kill: def $edx killed $edx def $rdx
	movl	%ecx, %eax
	movsd	(%rdi,%rax,8), %xmm12           # xmm12 = mem[0],zero
	leal	(,%rdx,8), %eax
	subl	%edx, %eax
	addl	%ecx, %eax
	movsd	(%rdi,%rax,8), %xmm8            # xmm8 = mem[0],zero
	movapd	%xmm12, %xmm11
	addsd	%xmm8, %xmm11
	leal	(%rcx,%rdx), %eax
	movsd	(%rdi,%rax,8), %xmm2            # xmm2 = mem[0],zero
	leal	(%rdx,%rdx,2), %r8d
	leal	(%rcx,%r8,2), %eax
	movsd	(%rdi,%rax,8), %xmm9            # xmm9 = mem[0],zero
	movapd	%xmm2, %xmm3
	addsd	%xmm9, %xmm3
	leal	(%rcx,%rdx,2), %eax
	movsd	(%rdi,%rax,8), %xmm4            # xmm4 = mem[0],zero
	leal	(%rdx,%rdx,4), %eax
	addl	%ecx, %eax
	movsd	(%rdi,%rax,8), %xmm10           # xmm10 = mem[0],zero
	movapd	%xmm4, %xmm1
	addsd	%xmm10, %xmm1
	addl	%ecx, %r8d
	movsd	(%rdi,%r8,8), %xmm6             # xmm6 = mem[0],zero
	leal	(%rcx,%rdx,4), %eax
	movsd	(%rdi,%rax,8), %xmm5            # xmm5 = mem[0],zero
	movapd	%xmm6, %xmm0
	addsd	%xmm5, %xmm0
	subsd	%xmm5, %xmm6
	subsd	%xmm10, %xmm4
	subsd	%xmm9, %xmm2
	subsd	%xmm8, %xmm12
	movapd	%xmm11, %xmm5
	addsd	%xmm0, %xmm5
	movapd	%xmm3, %xmm7
	addsd	%xmm1, %xmm7
	subsd	%xmm1, %xmm3
	subsd	%xmm0, %xmm11
	addsd	%xmm4, %xmm6
	addsd	%xmm2, %xmm4
	addsd	%xmm12, %xmm2
	movapd	%xmm7, %xmm0
	addsd	%xmm5, %xmm0
	subsd	%xmm7, %xmm5
	movapd	%xmm6, %xmm1
	subsd	%xmm2, %xmm1
	mulsd	.LCPI1_0(%rip), %xmm1
	addsd	%xmm11, %xmm3
	movsd	.LCPI1_1(%rip), %xmm7           # xmm7 = mem[0],zero
	mulsd	.LCPI1_2(%rip), %xmm6
	mulsd	%xmm7, %xmm3
	addsd	%xmm1, %xmm6
	mulsd	.LCPI1_3(%rip), %xmm2
	mulsd	%xmm7, %xmm4
	addsd	%xmm1, %xmm2
	movapd	%xmm11, %xmm1
	addsd	%xmm3, %xmm1
	subsd	%xmm3, %xmm11
	movapd	%xmm12, %xmm3
	addsd	%xmm4, %xmm3
	subsd	%xmm4, %xmm12
	movapd	%xmm12, %xmm4
	addsd	%xmm6, %xmm4
	movapd	%xmm3, %xmm7
	addsd	%xmm2, %xmm7
	subsd	%xmm2, %xmm3
	subsd	%xmm6, %xmm12
	mulsd	.LCPI1_4(%rip), %xmm0
	movsd	%xmm0, -72(%rsp)
	movsd	.LCPI1_5(%rip), %xmm0           # xmm0 = mem[0],zero
	mulsd	%xmm0, %xmm7
	movsd	%xmm7, -64(%rsp)
	mulsd	%xmm0, %xmm1
	movsd	%xmm1, -56(%rsp)
	mulsd	%xmm0, %xmm12
	movsd	%xmm12, -48(%rsp)
	mulsd	%xmm0, %xmm5
	movsd	%xmm5, -40(%rsp)
	mulsd	%xmm0, %xmm4
	movsd	%xmm4, -32(%rsp)
	mulsd	%xmm0, %xmm11
	movsd	%xmm11, -24(%rsp)
	mulsd	%xmm0, %xmm3
	movsd	%xmm3, -16(%rsp)
	movaps	-24(%rsp), %xmm0
	movups	%xmm0, 48(%rsi)
	movaps	-72(%rsp), %xmm0
	movups	%xmm0, (%rsi)
	movaps	-56(%rsp), %xmm0
	movups	%xmm0, 16(%rsi)
	movaps	-40(%rsp), %xmm0
	movups	%xmm0, 32(%rsi)
	retq
.Lfunc_end1:
	.size	dct8, .Lfunc_end1-dct8
	.cfi_endproc
                                        # -- End function
	.section	.rodata.cst8,"aM",@progbits,8
	.p2align	3                               # -- Begin function dct_ii
.LCPI2_0:
	.quad	0x3fd872b020c49ba6              # double 0.38200000000000001
.LCPI2_1:
	.quad	0x3fe69fbe76c8b439              # double 0.70699999999999996
.LCPI2_2:
	.quad	0x3fe14fdf3b645a1d              # double 0.54100000000000004
.LCPI2_3:
	.quad	0x3ff4e5604189374c              # double 1.306
.LCPI2_4:
	.quad	0x3fd0000000000000              # double 0.25
.LCPI2_5:
	.quad	0x3fc0000000000000              # double 0.125
	.section	.rodata.cst16,"aM",@progbits,16
	.p2align	4
.LCPI2_6:
	.quad	0x3fd872b020c49ba6              # double 0.38200000000000001
	.quad	0x3fd872b020c49ba6              # double 0.38200000000000001
.LCPI2_7:
	.quad	0x3fe69fbe76c8b439              # double 0.70699999999999996
	.quad	0x3fe69fbe76c8b439              # double 0.70699999999999996
.LCPI2_8:
	.quad	0x3fe14fdf3b645a1d              # double 0.54100000000000004
	.quad	0x3fe14fdf3b645a1d              # double 0.54100000000000004
.LCPI2_9:
	.quad	0x3ff4e5604189374c              # double 1.306
	.quad	0x3ff4e5604189374c              # double 1.306
.LCPI2_10:
	.quad	0x3fd0000000000000              # double 0.25
	.quad	0x3fd0000000000000              # double 0.25
.LCPI2_11:
	.quad	0x3fc0000000000000              # double 0.125
	.quad	0x3fc0000000000000              # double 0.125
	.text
	.globl	dct_ii
	.p2align	4, 0x90
	.type	dct_ii,@function
dct_ii:                                 # @dct_ii
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	pushq	%r15
	.cfi_def_cfa_offset 24
	pushq	%r14
	.cfi_def_cfa_offset 32
	pushq	%r12
	.cfi_def_cfa_offset 40
	pushq	%rbx
	.cfi_def_cfa_offset 48
	subq	$576, %rsp                      # imm = 0x240
	.cfi_def_cfa_offset 624
	.cfi_offset %rbx, -48
	.cfi_offset %r12, -40
	.cfi_offset %r14, -32
	.cfi_offset %r15, -24
	.cfi_offset %rbp, -16
	movl	%ecx, %ebx
	movl	%edx, %r15d
	movq	%rsi, %r14
	movq	%rdi, %r12
	leaq	64(%rsp), %rdi
	xorl	%ebp, %ebp
	movl	$512, %edx                      # imm = 0x200
	xorl	%esi, %esi
	callq	memset
	movq	40(%r12), %rax
	movzwl	14(%r12), %ecx
	imull	%ecx, %ebx
	addl	%r15d, %ebx
	.p2align	4, 0x90
.LBB2_1:                                # =>This Inner Loop Header: Depth=1
	movslq	%ebx, %rbx
	movzbl	(%rax,%rbx), %edx
	xorps	%xmm0, %xmm0
	cvtsi2sd	%edx, %xmm0
	movsd	%xmm0, 64(%rsp,%rbp)
	leal	1(%rbx), %edx
	movslq	%edx, %rdx
	movzbl	(%rax,%rdx), %edx
	xorps	%xmm0, %xmm0
	cvtsi2sd	%edx, %xmm0
	movsd	%xmm0, 72(%rsp,%rbp)
	leal	2(%rbx), %edx
	movslq	%edx, %rdx
	movzbl	(%rax,%rdx), %edx
	xorps	%xmm0, %xmm0
	cvtsi2sd	%edx, %xmm0
	movsd	%xmm0, 80(%rsp,%rbp)
	leal	3(%rbx), %edx
	movslq	%edx, %rdx
	movzbl	(%rax,%rdx), %edx
	xorps	%xmm0, %xmm0
	cvtsi2sd	%edx, %xmm0
	movsd	%xmm0, 88(%rsp,%rbp)
	leal	4(%rbx), %edx
	movslq	%edx, %rdx
	movzbl	(%rax,%rdx), %edx
	xorps	%xmm0, %xmm0
	cvtsi2sd	%edx, %xmm0
	movsd	%xmm0, 96(%rsp,%rbp)
	leal	5(%rbx), %edx
	movslq	%edx, %rdx
	movzbl	(%rax,%rdx), %edx
	xorps	%xmm0, %xmm0
	cvtsi2sd	%edx, %xmm0
	movsd	%xmm0, 104(%rsp,%rbp)
	leal	6(%rbx), %edx
	movslq	%edx, %rdx
	movzbl	(%rax,%rdx), %edx
	xorps	%xmm0, %xmm0
	cvtsi2sd	%edx, %xmm0
	movsd	%xmm0, 112(%rsp,%rbp)
	leal	7(%rbx), %edx
	movslq	%edx, %rdx
	movzbl	(%rax,%rdx), %edx
	xorps	%xmm0, %xmm0
	cvtsi2sd	%edx, %xmm0
	movsd	%xmm0, 120(%rsp,%rbp)
	addq	$64, %rbp
	addl	%ecx, %ebx
	cmpq	$512, %rbp                      # imm = 0x200
	jne	.LBB2_1
# %bb.2:
	xorpd	%xmm0, %xmm0
	movapd	%xmm0, 48(%rsp)
	movapd	%xmm0, 32(%rsp)
	movapd	%xmm0, 16(%rsp)
	movapd	%xmm0, (%rsp)
	xorl	%eax, %eax
	movsd	.LCPI2_0(%rip), %xmm8           # xmm8 = mem[0],zero
	movsd	.LCPI2_1(%rip), %xmm12          # xmm12 = mem[0],zero
	movsd	.LCPI2_2(%rip), %xmm9           # xmm9 = mem[0],zero
	movsd	.LCPI2_3(%rip), %xmm10          # xmm10 = mem[0],zero
	movsd	.LCPI2_4(%rip), %xmm11          # xmm11 = mem[0],zero
	movsd	.LCPI2_5(%rip), %xmm13          # xmm13 = mem[0],zero
	.p2align	4, 0x90
.LBB2_3:                                # =>This Inner Loop Header: Depth=1
	movsd	120(%rsp,%rax), %xmm14          # xmm14 = mem[0],zero
	movsd	64(%rsp,%rax), %xmm6            # xmm6 = mem[0],zero
	movsd	72(%rsp,%rax), %xmm5            # xmm5 = mem[0],zero
	movsd	112(%rsp,%rax), %xmm15          # xmm15 = mem[0],zero
	movsd	80(%rsp,%rax), %xmm2            # xmm2 = mem[0],zero
	movsd	104(%rsp,%rax), %xmm0           # xmm0 = mem[0],zero
	movsd	88(%rsp,%rax), %xmm3            # xmm3 = mem[0],zero
	movsd	96(%rsp,%rax), %xmm7            # xmm7 = mem[0],zero
	movapd	%xmm3, %xmm1
	addsd	%xmm7, %xmm1
	subsd	%xmm7, %xmm3
	movapd	%xmm2, %xmm4
	addsd	%xmm0, %xmm4
	subsd	%xmm0, %xmm2
	movapd	%xmm5, %xmm0
	addsd	%xmm15, %xmm0
	subsd	%xmm15, %xmm5
	movapd	%xmm6, %xmm7
	addsd	%xmm14, %xmm7
	subsd	%xmm14, %xmm6
	movapd	%xmm0, %xmm15
	addsd	%xmm4, %xmm15
	subsd	%xmm4, %xmm0
	movapd	%xmm7, %xmm4
	addsd	%xmm1, %xmm4
	subsd	%xmm1, %xmm7
	addsd	%xmm2, %xmm3
	addsd	%xmm5, %xmm2
	addsd	%xmm6, %xmm5
	movapd	%xmm15, %xmm14
	addsd	%xmm4, %xmm14
	subsd	%xmm15, %xmm4
	addsd	%xmm7, %xmm0
	movapd	%xmm3, %xmm1
	subsd	%xmm5, %xmm1
	mulsd	%xmm8, %xmm1
	mulsd	%xmm12, %xmm0
	mulsd	%xmm9, %xmm3
	addsd	%xmm1, %xmm3
	mulsd	%xmm12, %xmm2
	mulsd	%xmm10, %xmm5
	addsd	%xmm1, %xmm5
	movapd	%xmm7, %xmm1
	addsd	%xmm0, %xmm1
	subsd	%xmm0, %xmm7
	movapd	%xmm6, %xmm0
	addsd	%xmm2, %xmm0
	subsd	%xmm2, %xmm6
	movapd	%xmm0, %xmm2
	addsd	%xmm5, %xmm2
	subsd	%xmm5, %xmm0
	movapd	%xmm6, %xmm5
	addsd	%xmm3, %xmm5
	subsd	%xmm3, %xmm6
	mulsd	%xmm11, %xmm14
	movsd	%xmm14, (%rsp)
	mulsd	%xmm13, %xmm2
	movsd	%xmm2, 8(%rsp)
	mulsd	%xmm13, %xmm1
	movsd	%xmm1, 16(%rsp)
	mulsd	%xmm13, %xmm6
	movsd	%xmm6, 24(%rsp)
	mulsd	%xmm13, %xmm4
	movsd	%xmm4, 32(%rsp)
	mulsd	%xmm13, %xmm5
	movsd	%xmm5, 40(%rsp)
	mulsd	%xmm13, %xmm7
	movsd	%xmm7, 48(%rsp)
	mulsd	%xmm13, %xmm0
	movsd	%xmm0, 56(%rsp)
	movaps	48(%rsp), %xmm0
	movups	%xmm0, 48(%r14,%rax)
	movaps	32(%rsp), %xmm0
	movups	%xmm0, 32(%r14,%rax)
	movaps	16(%rsp), %xmm0
	movups	%xmm0, 16(%r14,%rax)
	movapd	(%rsp), %xmm0
	movupd	%xmm0, (%r14,%rax)
	addq	$64, %rax
	cmpq	$512, %rax                      # imm = 0x200
	jne	.LBB2_3
# %bb.4:
	movupd	448(%r14), %xmm8
	movupd	(%r14), %xmm1
	movupd	16(%r14), %xmm13
	movupd	64(%r14), %xmm6
	movupd	80(%r14), %xmm14
	movapd	%xmm1, %xmm2
	addpd	%xmm8, %xmm2
	movupd	384(%r14), %xmm9
	movapd	%xmm6, %xmm4
	addpd	%xmm9, %xmm4
	movupd	128(%r14), %xmm0
	movupd	320(%r14), %xmm10
	movapd	%xmm0, %xmm11
	addpd	%xmm10, %xmm11
	movupd	192(%r14), %xmm5
	movupd	256(%r14), %xmm3
	movapd	%xmm5, %xmm12
	addpd	%xmm3, %xmm12
	subpd	%xmm3, %xmm5
	subpd	%xmm10, %xmm0
	subpd	%xmm9, %xmm6
	subpd	%xmm8, %xmm1
	movapd	%xmm2, %xmm3
	addpd	%xmm12, %xmm3
	movapd	%xmm4, %xmm7
	addpd	%xmm11, %xmm7
	subpd	%xmm11, %xmm4
	subpd	%xmm12, %xmm2
	addpd	%xmm0, %xmm5
	addpd	%xmm6, %xmm0
	addpd	%xmm1, %xmm6
	movapd	%xmm7, %xmm15
	addpd	%xmm3, %xmm15
	subpd	%xmm7, %xmm3
	addpd	%xmm2, %xmm4
	movapd	%xmm5, %xmm7
	subpd	%xmm6, %xmm7
	movapd	.LCPI2_6(%rip), %xmm8           # xmm8 = [3.8200000000000001E-1,3.8200000000000001E-1]
	mulpd	%xmm8, %xmm7
	movapd	.LCPI2_7(%rip), %xmm12          # xmm12 = [7.0699999999999996E-1,7.0699999999999996E-1]
	mulpd	%xmm12, %xmm4
	movapd	.LCPI2_8(%rip), %xmm9           # xmm9 = [5.4100000000000004E-1,5.4100000000000004E-1]
	mulpd	%xmm9, %xmm5
	addpd	%xmm7, %xmm5
	mulpd	%xmm12, %xmm0
	movapd	.LCPI2_9(%rip), %xmm10          # xmm10 = [1.306E+0,1.306E+0]
	mulpd	%xmm10, %xmm6
	addpd	%xmm7, %xmm6
	movapd	%xmm2, %xmm7
	addpd	%xmm4, %xmm7
	subpd	%xmm4, %xmm2
	movapd	%xmm1, %xmm4
	addpd	%xmm0, %xmm4
	subpd	%xmm0, %xmm1
	movapd	%xmm4, %xmm0
	addpd	%xmm6, %xmm0
	subpd	%xmm6, %xmm4
	movapd	%xmm1, %xmm6
	addpd	%xmm5, %xmm6
	subpd	%xmm5, %xmm1
	movapd	.LCPI2_10(%rip), %xmm11         # xmm11 = [2.5E-1,2.5E-1]
	mulpd	%xmm11, %xmm15
	movupd	%xmm15, (%r14)
	movapd	.LCPI2_11(%rip), %xmm15         # xmm15 = [1.25E-1,1.25E-1]
	mulpd	%xmm15, %xmm0
	mulpd	%xmm15, %xmm7
	mulpd	%xmm15, %xmm1
	mulpd	%xmm15, %xmm3
	mulpd	%xmm15, %xmm6
	mulpd	%xmm15, %xmm2
	mulpd	%xmm15, %xmm4
	movupd	%xmm0, 64(%r14)
	movupd	%xmm7, 128(%r14)
	movupd	%xmm1, 192(%r14)
	movupd	%xmm3, 256(%r14)
	movupd	%xmm6, 320(%r14)
	movupd	%xmm2, 384(%r14)
	movupd	%xmm4, 448(%r14)
	movupd	464(%r14), %xmm2
	movupd	400(%r14), %xmm1
	movupd	144(%r14), %xmm3
	movupd	336(%r14), %xmm4
	movupd	208(%r14), %xmm0
	movupd	272(%r14), %xmm5
	movapd	%xmm0, %xmm6
	addpd	%xmm5, %xmm6
	subpd	%xmm5, %xmm0
	movapd	%xmm3, %xmm7
	addpd	%xmm4, %xmm7
	subpd	%xmm4, %xmm3
	movapd	%xmm14, %xmm5
	addpd	%xmm1, %xmm5
	subpd	%xmm1, %xmm14
	movapd	%xmm13, %xmm1
	addpd	%xmm2, %xmm1
	subpd	%xmm2, %xmm13
	movapd	%xmm5, %xmm4
	addpd	%xmm7, %xmm4
	subpd	%xmm7, %xmm5
	movapd	%xmm1, %xmm2
	addpd	%xmm6, %xmm2
	subpd	%xmm6, %xmm1
	addpd	%xmm3, %xmm0
	addpd	%xmm14, %xmm3
	addpd	%xmm13, %xmm14
	movapd	%xmm4, %xmm7
	addpd	%xmm2, %xmm7
	subpd	%xmm4, %xmm2
	addpd	%xmm1, %xmm5
	movapd	%xmm0, %xmm4
	subpd	%xmm14, %xmm4
	mulpd	%xmm8, %xmm4
	mulpd	%xmm12, %xmm5
	mulpd	%xmm9, %xmm0
	addpd	%xmm4, %xmm0
	mulpd	%xmm12, %xmm3
	mulpd	%xmm10, %xmm14
	addpd	%xmm4, %xmm14
	movapd	%xmm1, %xmm6
	addpd	%xmm5, %xmm6
	subpd	%xmm5, %xmm1
	movapd	%xmm13, %xmm5
	addpd	%xmm3, %xmm5
	subpd	%xmm3, %xmm13
	movapd	%xmm5, %xmm3
	addpd	%xmm14, %xmm3
	subpd	%xmm14, %xmm5
	movapd	%xmm13, %xmm4
	addpd	%xmm0, %xmm4
	subpd	%xmm0, %xmm13
	mulpd	%xmm11, %xmm7
	mulpd	%xmm15, %xmm3
	mulpd	%xmm15, %xmm6
	mulpd	%xmm15, %xmm13
	mulpd	%xmm15, %xmm2
	mulpd	%xmm15, %xmm4
	mulpd	%xmm15, %xmm1
	mulpd	%xmm15, %xmm5
	movupd	%xmm7, 16(%r14)
	movupd	%xmm3, 80(%r14)
	movupd	%xmm6, 144(%r14)
	movupd	%xmm13, 208(%r14)
	movupd	%xmm2, 272(%r14)
	movupd	%xmm4, 336(%r14)
	movupd	%xmm1, 400(%r14)
	movupd	%xmm5, 464(%r14)
	movupd	480(%r14), %xmm13
	movupd	32(%r14), %xmm14
	movupd	96(%r14), %xmm6
	movupd	160(%r14), %xmm4
	movupd	224(%r14), %xmm3
	movupd	416(%r14), %xmm2
	movupd	352(%r14), %xmm5
	movupd	288(%r14), %xmm7
	movapd	%xmm3, %xmm0
	addpd	%xmm7, %xmm0
	subpd	%xmm7, %xmm3
	movapd	%xmm4, %xmm7
	addpd	%xmm5, %xmm7
	subpd	%xmm5, %xmm4
	movapd	%xmm6, %xmm5
	addpd	%xmm2, %xmm5
	subpd	%xmm2, %xmm6
	movapd	%xmm14, %xmm2
	addpd	%xmm13, %xmm2
	subpd	%xmm13, %xmm14
	movapd	%xmm5, %xmm1
	addpd	%xmm7, %xmm1
	subpd	%xmm7, %xmm5
	movapd	%xmm2, %xmm7
	addpd	%xmm0, %xmm7
	subpd	%xmm0, %xmm2
	addpd	%xmm4, %xmm3
	addpd	%xmm6, %xmm4
	addpd	%xmm14, %xmm6
	movapd	%xmm1, %xmm13
	addpd	%xmm7, %xmm13
	subpd	%xmm1, %xmm7
	addpd	%xmm2, %xmm5
	movapd	%xmm3, %xmm0
	subpd	%xmm6, %xmm0
	mulpd	%xmm8, %xmm0
	mulpd	%xmm12, %xmm5
	mulpd	%xmm9, %xmm3
	addpd	%xmm0, %xmm3
	mulpd	%xmm12, %xmm4
	mulpd	%xmm10, %xmm6
	addpd	%xmm0, %xmm6
	movapd	%xmm2, %xmm0
	addpd	%xmm5, %xmm0
	subpd	%xmm5, %xmm2
	movapd	%xmm14, %xmm5
	addpd	%xmm4, %xmm5
	subpd	%xmm4, %xmm14
	movapd	%xmm5, %xmm1
	addpd	%xmm6, %xmm1
	subpd	%xmm6, %xmm5
	movapd	%xmm14, %xmm4
	addpd	%xmm3, %xmm4
	subpd	%xmm3, %xmm14
	mulpd	%xmm11, %xmm13
	mulpd	%xmm15, %xmm1
	mulpd	%xmm15, %xmm0
	mulpd	%xmm15, %xmm14
	mulpd	%xmm15, %xmm7
	mulpd	%xmm15, %xmm4
	mulpd	%xmm15, %xmm2
	mulpd	%xmm15, %xmm5
	movupd	%xmm13, 32(%r14)
	movupd	%xmm1, 96(%r14)
	movupd	%xmm0, 160(%r14)
	movupd	%xmm14, 224(%r14)
	movupd	%xmm7, 288(%r14)
	movupd	%xmm4, 352(%r14)
	movupd	%xmm2, 416(%r14)
	movupd	%xmm5, 480(%r14)
	movupd	496(%r14), %xmm13
	movupd	48(%r14), %xmm1
	movupd	112(%r14), %xmm7
	movupd	176(%r14), %xmm4
	movupd	240(%r14), %xmm3
	movupd	432(%r14), %xmm14
	movupd	368(%r14), %xmm2
	movupd	304(%r14), %xmm6
	movapd	%xmm3, %xmm0
	addpd	%xmm6, %xmm0
	subpd	%xmm6, %xmm3
	movapd	%xmm4, %xmm5
	addpd	%xmm2, %xmm5
	subpd	%xmm2, %xmm4
	movapd	%xmm7, %xmm6
	addpd	%xmm14, %xmm6
	subpd	%xmm14, %xmm7
	movapd	%xmm1, %xmm2
	addpd	%xmm13, %xmm2
	subpd	%xmm13, %xmm1
	movapd	%xmm6, %xmm13
	addpd	%xmm5, %xmm13
	subpd	%xmm5, %xmm6
	movapd	%xmm2, %xmm14
	addpd	%xmm0, %xmm14
	subpd	%xmm0, %xmm2
	addpd	%xmm4, %xmm3
	addpd	%xmm7, %xmm4
	addpd	%xmm1, %xmm7
	movapd	%xmm13, %xmm8
	addpd	%xmm14, %xmm8
	subpd	%xmm13, %xmm14
	movapd	%xmm3, %xmm0
	subpd	%xmm7, %xmm0
	mulpd	.LCPI2_6(%rip), %xmm0
	mulpd	%xmm9, %xmm3
	addpd	%xmm2, %xmm6
	mulpd	%xmm12, %xmm6
	mulpd	%xmm12, %xmm4
	mulpd	%xmm10, %xmm7
	addpd	%xmm0, %xmm3
	addpd	%xmm0, %xmm7
	movapd	%xmm2, %xmm0
	addpd	%xmm6, %xmm0
	subpd	%xmm6, %xmm2
	movapd	%xmm1, %xmm6
	addpd	%xmm4, %xmm6
	subpd	%xmm4, %xmm1
	movapd	%xmm6, %xmm5
	addpd	%xmm7, %xmm5
	subpd	%xmm7, %xmm6
	movapd	%xmm1, %xmm4
	addpd	%xmm3, %xmm4
	subpd	%xmm3, %xmm1
	mulpd	%xmm11, %xmm8
	mulpd	%xmm15, %xmm5
	mulpd	%xmm15, %xmm0
	mulpd	%xmm15, %xmm1
	mulpd	%xmm15, %xmm14
	mulpd	%xmm15, %xmm4
	mulpd	%xmm15, %xmm2
	mulpd	%xmm15, %xmm6
	movupd	%xmm8, 48(%r14)
	movupd	%xmm5, 112(%r14)
	movhlps	%xmm8, %xmm5                    # xmm5 = xmm8[1],xmm5[1]
	movupd	%xmm0, 176(%r14)
	movupd	%xmm1, 240(%r14)
	movupd	%xmm14, 304(%r14)
	movupd	%xmm4, 368(%r14)
	movupd	%xmm2, 432(%r14)
	movupd	%xmm6, 496(%r14)
	movaps	%xmm5, (%rsp)
	movhlps	%xmm0, %xmm1                    # xmm1 = xmm0[1],xmm1[1]
	movaps	%xmm1, 16(%rsp)
	movhlps	%xmm14, %xmm4                   # xmm4 = xmm14[1],xmm4[1]
	movaps	%xmm4, 32(%rsp)
	movhlps	%xmm2, %xmm6                    # xmm6 = xmm2[1],xmm6[1]
	movaps	%xmm6, 48(%rsp)
	movq	stdout(%rip), %rsi
	movl	$10, %edi
	addq	$576, %rsp                      # imm = 0x240
	.cfi_def_cfa_offset 48
	popq	%rbx
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r14
	.cfi_def_cfa_offset 24
	popq	%r15
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	jmp	putc                            # TAILCALL
.Lfunc_end2:
	.size	dct_ii, .Lfunc_end2-dct_ii
	.cfi_endproc
                                        # -- End function
	.globl	idct8                           # -- Begin function idct8
	.p2align	4, 0x90
	.type	idct8,@function
idct8:                                  # @idct8
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	pushq	%r15
	.cfi_def_cfa_offset 24
	pushq	%r14
	.cfi_def_cfa_offset 32
	pushq	%r13
	.cfi_def_cfa_offset 40
	pushq	%r12
	.cfi_def_cfa_offset 48
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset %rbx, -56
	.cfi_offset %r12, -48
	.cfi_offset %r13, -40
	.cfi_offset %r14, -32
	.cfi_offset %r15, -24
	.cfi_offset %rbp, -16
                                        # kill: def $ecx killed $ecx def $rcx
	movq	%rcx, -8(%rsp)                  # 8-byte Spill
                                        # kill: def $edx killed $edx def $rdx
	movl	%edx, %r11d
	movl	%ecx, %r9d
	leaq	(%r11,%r9), %rax
	movl	(%rdi,%rax,4), %r12d
	leaq	(%r9,%r11,2), %rax
	movl	(%rdi,%rax,4), %r8d
	leaq	(%r11,%r11,2), %rbx
	leaq	(%rbx,%r9), %rax
	movl	(%rdi,%rax,4), %r14d
	leaq	(%r11,%r11,4), %rax
	addq	%r9, %rax
	movl	(%rdi,%rax,4), %eax
	leaq	(%r9,%rbx,2), %rbx
	movl	(%rdi,%rbx,4), %ebx
	leaq	(,%r11,8), %rbp
	subq	%r11, %rbp
	addq	%r9, %rbp
	movl	(%rdi,%rbp,4), %ebp
	shll	$5, %r12d
	shll	$6, %r8d
	shll	$5, %r14d
	shll	$5, %eax
	shll	$6, %ebx
	shll	$5, %ebp
	movl	%eax, %r10d
	subl	%r14d, %r10d
	leal	(%r12,%rbp), %r15d
	subl	%ebp, %r12d
	addl	%r14d, %eax
	leal	(%rbx,%r8), %ebp
	leal	(%rbx,%r8), %r14d
	addl	$15, %r14d
	movl	%r8d, %r13d
	subl	%ebx, %r13d
	leal	(%r15,%rax), %ecx
	leal	(%r15,%rax), %r8d
	addl	$15, %r8d
                                        # kill: def $r15d killed $r15d killed $r15
	subl	%eax, %r15d
	leaq	(%r9,%r11,4), %rax
	movl	(%rdi,%rax,4), %eax
	movl	(%rdi,%r9,4), %ebx
	shll	$2, %ebx
	testl	%ebp, %ebp
	cmovnsl	%ebp, %r14d
	sarl	$4, %r14d
	testl	%ecx, %ecx
	cmovnsl	%ecx, %r8d
	sarl	$4, %r8d
	movslq	%r13d, %rcx
	imulq	$715827883, %rcx, %rbp          # imm = 0x2AAAAAAB
	movq	%rbp, %rcx
	shrq	$63, %rcx
	sarq	$33, %rbp
	addl	%ecx, %ebp
	movslq	%r15d, %rcx
	imulq	$715827883, %rcx, %r15          # imm = 0x2AAAAAAB
	movq	%r15, %rcx
	shrq	$63, %rcx
	sarq	$33, %r15
	addl	%ecx, %r15d
	leal	(,%r12,8), %ecx
	subl	%r12d, %ecx
	leal	(%r10,%r10,2), %edi
	subl	%edi, %ecx
	leal	63(%rcx), %r11d
	testl	%ecx, %ecx
	cmovnsl	%ecx, %r11d
	leal	(,%r10,8), %ecx
	subl	%r10d, %ecx
	leal	(,%rax,8), %r10d
	sarl	$6, %r11d
	leal	(%r12,%r12,2), %edi
	addl	%edi, %ecx
	movslq	%ecx, %rcx
	imulq	$156180629, %rcx, %r12          # imm = 0x94F2095
	movq	%r12, %rcx
	shrq	$63, %rcx
	sarq	$33, %r12
	addl	%ecx, %r12d
	leal	(%rbx,%rax,8), %edi
	sarl	%edi
	subl	%r10d, %ebx
	sarl	%ebx
	subl	%r14d, %ebp
	leal	(%rbx,%rbp), %eax
	movl	%eax, %r13d
	shrl	$31, %r13d
	addl	%eax, %r13d
	leal	(%r14,%rdi), %r10d
	sarl	%r10d
	sarl	%r13d
	subl	%ebp, %ebx
	movl	%ebx, %eax
	shrl	$31, %eax
	addl	%ebx, %eax
	sarl	%eax
	subl	%r14d, %edi
	sarl	%edi
	subl	%r8d, %r11d
	subl	%r11d, %r15d
	subl	%r15d, %r12d
	leal	(%r10,%r8), %ecx
	movl	%ecx, %ebx
	shrl	$31, %ebx
	addl	%ecx, %ebx
	sarl	%ebx
	movl	%ebx, (%rsi,%r9,4)
	leal	(%r11,%r13), %ebp
	movl	%ebp, %ecx
	shrl	$31, %ecx
	addl	%ebp, %ecx
	sarl	%ecx
	movq	-8(%rsp), %r9                   # 8-byte Reload
	leal	(%r9,%rdx), %ebp
	movl	%ecx, (%rsi,%rbp,4)
	leal	(%r15,%rax), %ebp
	movl	%ebp, %ecx
	shrl	$31, %ecx
	addl	%ebp, %ecx
	sarl	%ecx
	leal	(%r9,%rdx,2), %ebp
	movl	%ecx, (%rsi,%rbp,4)
	leal	(%r12,%rdi), %ebp
	movl	%ebp, %ecx
	shrl	$31, %ecx
	addl	%ebp, %ecx
	sarl	%ecx
	leal	(%rdx,%rdx,2), %ebp
	leal	(%r9,%rbp), %ebx
	movl	%ecx, (%rsi,%rbx,4)
	subl	%r12d, %edi
	movl	%edi, %ecx
	shrl	$31, %ecx
	addl	%edi, %ecx
	sarl	%ecx
	leal	(%r9,%rdx,4), %edi
	movl	%ecx, (%rsi,%rdi,4)
	subl	%r15d, %eax
	movl	%eax, %ecx
	shrl	$31, %ecx
	addl	%eax, %ecx
	sarl	%ecx
	leal	(%rdx,%rdx,4), %eax
	movq	%r9, %rdi
	addl	%edi, %eax
	movl	%ecx, (%rsi,%rax,4)
	subl	%r11d, %r13d
	movl	%r13d, %eax
	shrl	$31, %eax
	addl	%r13d, %eax
	sarl	%eax
	leal	(%r9,%rbp,2), %ecx
	movl	%eax, (%rsi,%rcx,4)
	subl	%r8d, %r10d
	movl	%r10d, %eax
	shrl	$31, %eax
	addl	%r10d, %eax
	leal	(,%rdx,8), %ecx
	subl	%edx, %ecx
	addl	%edi, %ecx
	sarl	%eax
	movl	%eax, (%rsi,%rcx,4)
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%r12
	.cfi_def_cfa_offset 40
	popq	%r13
	.cfi_def_cfa_offset 32
	popq	%r14
	.cfi_def_cfa_offset 24
	popq	%r15
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end3:
	.size	idct8, .Lfunc_end3-idct8
	.cfi_endproc
                                        # -- End function
	.section	.rodata.cst16,"aM",@progbits,16
	.p2align	4                               # -- Begin function idct_ii
.LCPI4_0:
	.long	715827883                       # 0x2aaaaaab
	.long	715827883                       # 0x2aaaaaab
	.long	715827883                       # 0x2aaaaaab
	.long	715827883                       # 0x2aaaaaab
.LCPI4_1:
	.long	156180629                       # 0x94f2095
	.long	156180629                       # 0x94f2095
	.long	156180629                       # 0x94f2095
	.long	156180629                       # 0x94f2095
	.text
	.globl	idct_ii
	.p2align	4, 0x90
	.type	idct_ii,@function
idct_ii:                                # @idct_ii
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	pushq	%r15
	.cfi_def_cfa_offset 24
	pushq	%r14
	.cfi_def_cfa_offset 32
	pushq	%r13
	.cfi_def_cfa_offset 40
	pushq	%r12
	.cfi_def_cfa_offset 48
	pushq	%rbx
	.cfi_def_cfa_offset 56
	pushq	%rax
	.cfi_def_cfa_offset 64
	.cfi_offset %rbx, -56
	.cfi_offset %r12, -48
	.cfi_offset %r13, -40
	.cfi_offset %r14, -32
	.cfi_offset %r15, -24
	.cfi_offset %rbp, -16
	movl	%r8d, %r13d
	movl	%ecx, %r14d
	movq	%rdx, %rbx
	movl	%esi, %r12d
	movq	%rdi, %r15
	xorl	%ebp, %ebp
	movq	%rdx, %rdi
	movq	%rdx, %rsi
	movl	$8, %edx
	xorl	%ecx, %ecx
	callq	idct8
	movq	%rbx, %rdi
	movq	%rbx, %rsi
	movl	$8, %edx
	movl	$1, %ecx
	callq	idct8
	movq	%rbx, %rdi
	movq	%rbx, %rsi
	movl	$8, %edx
	movl	$2, %ecx
	callq	idct8
	movq	%rbx, %rdi
	movq	%rbx, %rsi
	movl	$8, %edx
	movl	$3, %ecx
	callq	idct8
	movq	%rbx, %rdi
	movq	%rbx, %rsi
	movl	$8, %edx
	movl	$4, %ecx
	callq	idct8
	movq	%rbx, %rdi
	movq	%rbx, %rsi
	movl	$8, %edx
	movl	$5, %ecx
	callq	idct8
	movq	%rbx, %rdi
	movq	%rbx, %rsi
	movl	$8, %edx
	movl	$6, %ecx
	callq	idct8
	movq	%rbx, %rdi
	movq	%rbx, %rsi
	movl	$8, %edx
	movl	$7, %ecx
	callq	idct8
	movdqa	.LCPI4_0(%rip), %xmm9           # xmm9 = [715827883,715827883,715827883,715827883]
	.p2align	4, 0x90
.LBB4_1:                                # =>This Inner Loop Header: Depth=1
	movdqu	112(%rbx,%rbp), %xmm10
	movdqu	80(%rbx,%rbp), %xmm2
	movdqu	(%rbx,%rbp), %xmm4
	movdqu	16(%rbx,%rbp), %xmm5
	movdqu	32(%rbx,%rbp), %xmm0
	movdqu	48(%rbx,%rbp), %xmm11
	movdqu	96(%rbx,%rbp), %xmm6
	movdqu	64(%rbx,%rbp), %xmm3
	movdqa	%xmm3, %xmm7
	punpckldq	%xmm6, %xmm7            # xmm7 = xmm7[0],xmm6[0],xmm7[1],xmm6[1]
	movdqa	%xmm4, %xmm1
	punpckldq	%xmm0, %xmm1            # xmm1 = xmm1[0],xmm0[0],xmm1[1],xmm0[1]
	movdqa	%xmm1, %xmm14
	punpcklqdq	%xmm7, %xmm14           # xmm14 = xmm14[0],xmm7[0]
	punpckhqdq	%xmm7, %xmm1            # xmm1 = xmm1[1],xmm7[1]
	punpckhdq	%xmm6, %xmm3            # xmm3 = xmm3[2],xmm6[2],xmm3[3],xmm6[3]
	punpckhdq	%xmm0, %xmm4            # xmm4 = xmm4[2],xmm0[2],xmm4[3],xmm0[3]
	movdqa	%xmm4, %xmm6
	punpcklqdq	%xmm3, %xmm6            # xmm6 = xmm6[0],xmm3[0]
	punpckhqdq	%xmm3, %xmm4            # xmm4 = xmm4[1],xmm3[1]
	movdqa	%xmm2, %xmm3
	punpckldq	%xmm10, %xmm3           # xmm3 = xmm3[0],xmm10[0],xmm3[1],xmm10[1]
	movdqa	%xmm5, %xmm12
	punpckldq	%xmm11, %xmm12          # xmm12 = xmm12[0],xmm11[0],xmm12[1],xmm11[1]
	movdqa	%xmm12, %xmm0
	punpcklqdq	%xmm3, %xmm0            # xmm0 = xmm0[0],xmm3[0]
	punpckhqdq	%xmm3, %xmm12           # xmm12 = xmm12[1],xmm3[1]
	punpckhdq	%xmm10, %xmm2           # xmm2 = xmm2[2],xmm10[2],xmm2[3],xmm10[3]
	punpckhdq	%xmm11, %xmm5           # xmm5 = xmm5[2],xmm11[2],xmm5[3],xmm11[3]
	movdqa	%xmm5, %xmm7
	punpcklqdq	%xmm2, %xmm7            # xmm7 = xmm7[0],xmm2[0]
	punpckhqdq	%xmm2, %xmm5            # xmm5 = xmm5[1],xmm2[1]
	pslld	$5, %xmm1
	pslld	$6, %xmm6
	pslld	$5, %xmm4
	pslld	$5, %xmm12
	pslld	$6, %xmm7
	pslld	$5, %xmm5
	movdqa	%xmm12, %xmm2
	psubd	%xmm4, %xmm2
	movdqa	%xmm5, %xmm3
	paddd	%xmm1, %xmm3
	psubd	%xmm5, %xmm1
	paddd	%xmm4, %xmm12
	movdqa	%xmm6, %xmm5
	psubd	%xmm7, %xmm5
	paddd	%xmm6, %xmm7
	movdqa	%xmm3, %xmm4
	psubd	%xmm12, %xmm4
	paddd	%xmm3, %xmm12
	pslld	$2, %xmm14
	pslld	$3, %xmm0
	psrad	$4, %xmm7
	psrad	$4, %xmm12
	pshufd	$245, %xmm5, %xmm3              # xmm3 = xmm5[1,1,3,3]
	pxor	%xmm6, %xmm6
	pcmpgtd	%xmm5, %xmm6
	pmuludq	%xmm9, %xmm5
	pshufd	$237, %xmm5, %xmm5              # xmm5 = xmm5[1,3,2,3]
	pmuludq	%xmm9, %xmm3
	pshufd	$237, %xmm3, %xmm3              # xmm3 = xmm3[1,3,2,3]
	punpckldq	%xmm3, %xmm5            # xmm5 = xmm5[0],xmm3[0],xmm5[1],xmm3[1]
	pand	%xmm9, %xmm6
	psubd	%xmm6, %xmm5
	movdqa	%xmm5, %xmm3
	psrld	$31, %xmm3
	psrad	$1, %xmm5
	paddd	%xmm3, %xmm5
	pshufd	$245, %xmm4, %xmm3              # xmm3 = xmm4[1,1,3,3]
	pxor	%xmm6, %xmm6
	pcmpgtd	%xmm4, %xmm6
	pmuludq	%xmm9, %xmm4
	pshufd	$237, %xmm4, %xmm10             # xmm10 = xmm4[1,3,2,3]
	pmuludq	%xmm9, %xmm3
	pshufd	$237, %xmm3, %xmm3              # xmm3 = xmm3[1,3,2,3]
	punpckldq	%xmm3, %xmm10           # xmm10 = xmm10[0],xmm3[0],xmm10[1],xmm3[1]
	pand	%xmm9, %xmm6
	psubd	%xmm6, %xmm10
	movdqa	%xmm10, %xmm3
	psrld	$31, %xmm3
	psrad	$1, %xmm10
	paddd	%xmm3, %xmm10
	movdqa	%xmm1, %xmm3
	pslld	$3, %xmm3
	psubd	%xmm1, %xmm3
	movdqa	%xmm2, %xmm4
	paddd	%xmm2, %xmm4
	paddd	%xmm2, %xmm4
	psubd	%xmm4, %xmm3
	movdqa	%xmm3, %xmm11
	psrad	$31, %xmm11
	psrld	$26, %xmm11
	paddd	%xmm3, %xmm11
	psrad	$6, %xmm11
	movdqa	%xmm1, %xmm3
	paddd	%xmm1, %xmm3
	paddd	%xmm1, %xmm3
	movdqa	%xmm2, %xmm1
	pslld	$3, %xmm1
	psubd	%xmm2, %xmm1
	paddd	%xmm3, %xmm1
	pshufd	$245, %xmm1, %xmm2              # xmm2 = xmm1[1,1,3,3]
	pxor	%xmm3, %xmm3
	pcmpgtd	%xmm1, %xmm3
	movdqa	.LCPI4_1(%rip), %xmm4           # xmm4 = [156180629,156180629,156180629,156180629]
	pmuludq	%xmm4, %xmm1
	pshufd	$237, %xmm1, %xmm6              # xmm6 = xmm1[1,3,2,3]
	pmuludq	%xmm4, %xmm2
	pshufd	$237, %xmm2, %xmm1              # xmm1 = xmm2[1,3,2,3]
	punpckldq	%xmm1, %xmm6            # xmm6 = xmm6[0],xmm1[0],xmm6[1],xmm1[1]
	pand	%xmm4, %xmm3
	psubd	%xmm3, %xmm6
	movdqa	%xmm6, %xmm1
	psrld	$31, %xmm1
	psrad	$1, %xmm6
	paddd	%xmm1, %xmm6
	movdqa	%xmm0, %xmm1
	paddd	%xmm14, %xmm1
	psrad	$1, %xmm1
	psubd	%xmm0, %xmm14
	psrad	$1, %xmm14
	psubd	%xmm7, %xmm5
	movdqa	%xmm7, %xmm15
	paddd	%xmm1, %xmm15
	psrad	$1, %xmm15
	movdqa	%xmm5, %xmm0
	paddd	%xmm14, %xmm0
	movdqa	%xmm0, %xmm13
	psrld	$31, %xmm13
	paddd	%xmm0, %xmm13
	psrad	$1, %xmm13
	psubd	%xmm5, %xmm14
	movdqa	%xmm14, %xmm5
	psrld	$31, %xmm5
	paddd	%xmm14, %xmm5
	psrad	$1, %xmm5
	psubd	%xmm7, %xmm1
	psrad	$1, %xmm1
	psubd	%xmm12, %xmm11
	psubd	%xmm11, %xmm10
	psubd	%xmm10, %xmm6
	movdqa	%xmm15, %xmm0
	paddd	%xmm12, %xmm0
	movdqa	%xmm0, %xmm3
	psrld	$31, %xmm3
	paddd	%xmm0, %xmm3
	psrad	$1, %xmm3
	movdqa	%xmm10, %xmm0
	paddd	%xmm5, %xmm0
	movdqa	%xmm0, %xmm2
	psrld	$31, %xmm2
	paddd	%xmm0, %xmm2
	psrad	$1, %xmm2
	movdqa	%xmm6, %xmm0
	paddd	%xmm1, %xmm0
	movdqa	%xmm0, %xmm7
	psrld	$31, %xmm7
	paddd	%xmm0, %xmm7
	psrad	$1, %xmm7
	movdqa	%xmm11, %xmm4
	paddd	%xmm13, %xmm4
	movdqa	%xmm4, %xmm8
	psrld	$31, %xmm8
	paddd	%xmm4, %xmm8
	psrad	$1, %xmm8
	pshufd	$229, %xmm3, %xmm14             # xmm14 = xmm3[1,1,2,3]
	psubd	%xmm12, %xmm15
	movdqa	%xmm15, %xmm4
	psrld	$31, %xmm4
	paddd	%xmm15, %xmm4
	psrad	$1, %xmm4
	pshufd	$250, %xmm3, %xmm12             # xmm12 = xmm3[2,2,3,3]
	pshufd	$164, %xmm4, %xmm15             # xmm15 = xmm4[0,1,2,2]
	punpckhdq	%xmm3, %xmm15           # xmm15 = xmm15[2],xmm3[2],xmm15[3],xmm3[3]
	punpckldq	%xmm8, %xmm3            # xmm3 = xmm3[0],xmm8[0],xmm3[1],xmm8[1]
	movdqa	%xmm8, %xmm0
	punpckldq	%xmm2, %xmm0            # xmm0 = xmm0[0],xmm2[0],xmm0[1],xmm2[1]
	punpckhdq	%xmm2, %xmm8            # xmm8 = xmm8[2],xmm2[2],xmm8[3],xmm2[3]
	punpckldq	%xmm7, %xmm2            # xmm2 = xmm2[0],xmm7[0],xmm2[1],xmm7[1]
	punpcklqdq	%xmm2, %xmm3            # xmm3 = xmm3[0],xmm2[0]
	movdqu	%xmm3, (%rbx,%rbp)
	movd	%xmm14, 32(%rbx,%rbp)
	psubd	%xmm6, %xmm1
	movdqa	%xmm1, %xmm2
	psrld	$31, %xmm2
	paddd	%xmm1, %xmm2
	psrad	$1, %xmm2
	movdqa	%xmm7, %xmm1
	punpckldq	%xmm2, %xmm1            # xmm1 = xmm1[0],xmm2[0],xmm1[1],xmm2[1]
	punpckhqdq	%xmm1, %xmm0            # xmm0 = xmm0[1],xmm1[1]
	movdqu	%xmm0, 36(%rbx,%rbp)
	punpckhdq	%xmm2, %xmm7            # xmm7 = xmm7[2],xmm2[2],xmm7[3],xmm2[3]
	movdqa	%xmm8, %xmm0
	punpcklqdq	%xmm7, %xmm0            # xmm0 = xmm0[0],xmm7[0]
	movdqu	%xmm0, 68(%rbx,%rbp)
	punpckhqdq	%xmm7, %xmm8            # xmm8 = xmm8[1],xmm7[1]
	movdqu	%xmm8, 100(%rbx,%rbp)
	psubd	%xmm10, %xmm5
	movdqa	%xmm5, %xmm0
	psrld	$31, %xmm0
	paddd	%xmm5, %xmm0
	psrad	$1, %xmm0
	punpckldq	%xmm0, %xmm2            # xmm2 = xmm2[0],xmm0[0],xmm2[1],xmm0[1]
	psubd	%xmm11, %xmm13
	movdqa	%xmm13, %xmm1
	psrld	$31, %xmm1
	paddd	%xmm13, %xmm1
	psrad	$1, %xmm1
	movdqa	%xmm0, %xmm3
	punpckldq	%xmm1, %xmm3            # xmm3 = xmm3[0],xmm1[0],xmm3[1],xmm1[1]
	pshufd	$231, %xmm0, %xmm5              # xmm5 = xmm0[3,1,2,3]
	punpckhdq	%xmm1, %xmm0            # xmm0 = xmm0[2],xmm1[2],xmm0[3],xmm1[3]
	pshufd	$231, %xmm1, %xmm6              # xmm6 = xmm1[3,1,2,3]
	punpckldq	%xmm4, %xmm1            # xmm1 = xmm1[0],xmm4[0],xmm1[1],xmm4[1]
	punpcklqdq	%xmm1, %xmm2            # xmm2 = xmm2[0],xmm1[0]
	movdqu	%xmm2, 16(%rbx,%rbp)
	pshufd	$231, %xmm4, %xmm1              # xmm1 = xmm4[3,1,2,3]
	punpckldq	%xmm12, %xmm4           # xmm4 = xmm4[0],xmm12[0],xmm4[1],xmm12[1]
	movhlps	%xmm3, %xmm4                    # xmm4 = xmm3[1],xmm4[1]
	movups	%xmm4, 52(%rbx,%rbp)
	movsd	%xmm0, %xmm15                   # xmm15 = xmm0[0],xmm15[1]
	movupd	%xmm15, 84(%rbx,%rbp)
	movd	%xmm5, 116(%rbx,%rbp)
	movd	%xmm6, 120(%rbx,%rbp)
	movd	%xmm1, 124(%rbx,%rbp)
	subq	$-128, %rbp
	cmpq	$256, %rbp                      # imm = 0x100
	jne	.LBB4_1
# %bb.2:
	movzwl	%r12w, %eax
	imull	%eax, %r13d
	addl	%r13d, %r14d
	xorl	%ecx, %ecx
	.p2align	4, 0x90
.LBB4_3:                                # =>This Inner Loop Header: Depth=1
	movzbl	(%rbx,%rcx), %edx
	movslq	%r14d, %r14
	movb	%dl, (%r15,%r14)
	movzbl	4(%rbx,%rcx), %edx
	leal	1(%r14), %esi
	movslq	%esi, %rsi
	movb	%dl, (%r15,%rsi)
	movzbl	8(%rbx,%rcx), %edx
	leal	2(%r14), %esi
	movslq	%esi, %rsi
	movb	%dl, (%r15,%rsi)
	movzbl	12(%rbx,%rcx), %edx
	leal	3(%r14), %esi
	movslq	%esi, %rsi
	movb	%dl, (%r15,%rsi)
	movzbl	16(%rbx,%rcx), %edx
	leal	4(%r14), %esi
	movslq	%esi, %rsi
	movb	%dl, (%r15,%rsi)
	movzbl	20(%rbx,%rcx), %edx
	leal	5(%r14), %esi
	movslq	%esi, %rsi
	movb	%dl, (%r15,%rsi)
	movzbl	24(%rbx,%rcx), %edx
	leal	6(%r14), %esi
	movslq	%esi, %rsi
	movb	%dl, (%r15,%rsi)
	movzbl	28(%rbx,%rcx), %edx
	leal	7(%r14), %esi
	movslq	%esi, %rsi
	movb	%dl, (%r15,%rsi)
	addq	$32, %rcx
	addl	%eax, %r14d
	cmpq	$256, %rcx                      # imm = 0x100
	jne	.LBB4_3
# %bb.4:
	addq	$8, %rsp
	.cfi_def_cfa_offset 56
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%r12
	.cfi_def_cfa_offset 40
	popq	%r13
	.cfi_def_cfa_offset 32
	popq	%r14
	.cfi_def_cfa_offset 24
	popq	%r15
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end4:
	.size	idct_ii, .Lfunc_end4-idct_ii
	.cfi_endproc
                                        # -- End function
	.globl	quantize                        # -- Begin function quantize
	.p2align	4, 0x90
	.type	quantize,@function
quantize:                               # @quantize
	.cfi_startproc
# %bb.0:
	retq
.Lfunc_end5:
	.size	quantize, .Lfunc_end5-quantize
	.cfi_endproc
                                        # -- End function
	.globl	main                            # -- Begin function main
	.p2align	4, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	pushq	%r15
	.cfi_def_cfa_offset 24
	pushq	%r14
	.cfi_def_cfa_offset 32
	pushq	%r13
	.cfi_def_cfa_offset 40
	pushq	%r12
	.cfi_def_cfa_offset 48
	pushq	%rbx
	.cfi_def_cfa_offset 56
	subq	$856, %rsp                      # imm = 0x358
	.cfi_def_cfa_offset 912
	.cfi_offset %rbx, -56
	.cfi_offset %r12, -48
	.cfi_offset %r13, -40
	.cfi_offset %r14, -32
	.cfi_offset %r15, -24
	.cfi_offset %rbp, -16
	leaq	24(%rsp), %rdi
	movl	$.L.str.3, %esi
	callq	load_tga
	movzwl	38(%rsp), %ebp
	movzwl	40(%rsp), %r14d
	movq	%r14, %rdi
	imulq	%rbp, %rdi
	callq	malloc
	movq	%rax, 16(%rsp)                  # 8-byte Spill
	testq	%r14, %r14
	je	.LBB6_1
# %bb.2:
	xorl	%ebx, %ebx
	jmp	.LBB6_3
	.p2align	4, 0x90
.LBB6_7:                                #   in Loop: Header=BB6_3 Depth=1
	movl	$.Lstr.11, %edi
	callq	puts
	addl	$1, %ebx
	movzwl	38(%rsp), %ebp
	movzwl	40(%rsp), %r14d
	movzwl	%r14w, %eax
	cmpl	%eax, %ebx
	jae	.LBB6_8
.LBB6_3:                                # =>This Loop Header: Depth=1
                                        #     Child Loop BB6_6 Depth 2
	testw	%bp, %bp
	je	.LBB6_7
# %bb.4:                                #   in Loop: Header=BB6_3 Depth=1
	movq	64(%rsp), %rax
	movzwl	%r14w, %ecx
	imull	%ebx, %ecx
	movzbl	(%rax,%rcx), %esi
	movl	$.L.str.4, %edi
	xorl	%eax, %eax
	callq	printf
	cmpw	$2, 38(%rsp)
	jb	.LBB6_7
# %bb.5:                                #   in Loop: Header=BB6_3 Depth=1
	movl	$1, %ebp
	.p2align	4, 0x90
.LBB6_6:                                #   Parent Loop BB6_3 Depth=1
                                        # =>  This Inner Loop Header: Depth=2
	movzwl	40(%rsp), %eax
	movq	64(%rsp), %rcx
	imull	%ebx, %eax
	addl	%ebp, %eax
	movzbl	(%rcx,%rax), %esi
	movl	$.L.str.4, %edi
	xorl	%eax, %eax
	callq	printf
	addl	$1, %ebp
	movzwl	38(%rsp), %eax
	cmpl	%eax, %ebp
	jb	.LBB6_6
	jmp	.LBB6_7
.LBB6_8:
	xorl	%r13d, %r13d
	cmpw	$8, %r14w
	jb	.LBB6_17
# %bb.9:
	movzwl	%bp, %ecx
	shrl	$3, %eax
	shrl	$3, %ecx
	imull	%eax, %ecx
	movl	%ecx, 8(%rsp)                   # 4-byte Spill
	xorl	%r13d, %r13d
	xorl	%ecx, %ecx
	xorl	%r15d, %r15d
	jmp	.LBB6_10
	.p2align	4, 0x90
.LBB6_16:                               #   in Loop: Header=BB6_10 Depth=1
	addl	$1, %ecx
	movzwl	%r14w, %eax
	shrl	$3, %eax
	cmpl	%eax, %ecx
	jae	.LBB6_17
.LBB6_10:                               # =>This Loop Header: Depth=1
                                        #     Child Loop BB6_12 Depth 2
                                        #       Child Loop BB6_13 Depth 3
	cmpw	$8, %bp
	jb	.LBB6_16
# %bb.11:                               #   in Loop: Header=BB6_10 Depth=1
	movq	%rcx, 72(%rsp)                  # 8-byte Spill
	leal	(,%rcx,8), %ebx
	xorl	%r12d, %r12d
	movl	%ebx, 12(%rsp)                  # 4-byte Spill
	.p2align	4, 0x90
.LBB6_12:                               #   Parent Loop BB6_10 Depth=1
                                        # =>  This Loop Header: Depth=2
                                        #       Child Loop BB6_13 Depth 3
	leal	(,%r12,8), %r14d
	leaq	24(%rsp), %rdi
	leaq	336(%rsp), %rsi
	movl	%r14d, %edx
	movl	%ebx, %ecx
	callq	dct_ii
	cvttpd2dq	352(%rsp), %xmm0
	cvttpd2dq	336(%rsp), %xmm1
	cvttpd2dq	384(%rsp), %xmm2
	unpcklpd	%xmm0, %xmm1                    # xmm1 = xmm1[0],xmm0[0]
	cvttpd2dq	368(%rsp), %xmm0
	movapd	%xmm1, 80(%rsp)
	cvttpd2dq	416(%rsp), %xmm1
	unpcklpd	%xmm2, %xmm0                    # xmm0 = xmm0[0],xmm2[0]
	cvttpd2dq	400(%rsp), %xmm2
	movapd	%xmm0, 96(%rsp)
	cvttpd2dq	448(%rsp), %xmm0
	unpcklpd	%xmm1, %xmm2                    # xmm2 = xmm2[0],xmm1[0]
	cvttpd2dq	432(%rsp), %xmm1
	movapd	%xmm2, 112(%rsp)
	cvttpd2dq	480(%rsp), %xmm2
	unpcklpd	%xmm0, %xmm1                    # xmm1 = xmm1[0],xmm0[0]
	cvttpd2dq	464(%rsp), %xmm0
	movapd	%xmm1, 128(%rsp)
	cvttpd2dq	512(%rsp), %xmm1
	unpcklpd	%xmm2, %xmm0                    # xmm0 = xmm0[0],xmm2[0]
	cvttpd2dq	496(%rsp), %xmm2
	movapd	%xmm0, 144(%rsp)
	cvttpd2dq	544(%rsp), %xmm0
	unpcklpd	%xmm1, %xmm2                    # xmm2 = xmm2[0],xmm1[0]
	cvttpd2dq	528(%rsp), %xmm1
	movapd	%xmm2, 160(%rsp)
	cvttpd2dq	576(%rsp), %xmm2
	unpcklpd	%xmm0, %xmm1                    # xmm1 = xmm1[0],xmm0[0]
	cvttpd2dq	560(%rsp), %xmm0
	movapd	%xmm1, 176(%rsp)
	cvttpd2dq	608(%rsp), %xmm1
	unpcklpd	%xmm2, %xmm0                    # xmm0 = xmm0[0],xmm2[0]
	cvttpd2dq	592(%rsp), %xmm2
	movapd	%xmm0, 192(%rsp)
	cvttpd2dq	640(%rsp), %xmm0
	unpcklpd	%xmm1, %xmm2                    # xmm2 = xmm2[0],xmm1[0]
	cvttpd2dq	624(%rsp), %xmm1
	movapd	%xmm2, 208(%rsp)
	cvttpd2dq	672(%rsp), %xmm2
	unpcklpd	%xmm0, %xmm1                    # xmm1 = xmm1[0],xmm0[0]
	cvttpd2dq	656(%rsp), %xmm0
	movapd	%xmm1, 224(%rsp)
	cvttpd2dq	704(%rsp), %xmm1
	unpcklpd	%xmm2, %xmm0                    # xmm0 = xmm0[0],xmm2[0]
	cvttpd2dq	688(%rsp), %xmm2
	movapd	%xmm0, 240(%rsp)
	cvttpd2dq	736(%rsp), %xmm0
	unpcklpd	%xmm1, %xmm2                    # xmm2 = xmm2[0],xmm1[0]
	cvttpd2dq	720(%rsp), %xmm1
	movapd	%xmm2, 256(%rsp)
	cvttpd2dq	768(%rsp), %xmm2
	unpcklpd	%xmm0, %xmm1                    # xmm1 = xmm1[0],xmm0[0]
	cvttpd2dq	752(%rsp), %xmm0
	movapd	%xmm1, 272(%rsp)
	cvttpd2dq	800(%rsp), %xmm1
	unpcklpd	%xmm2, %xmm0                    # xmm0 = xmm0[0],xmm2[0]
	cvttpd2dq	784(%rsp), %xmm2
	movapd	%xmm0, 288(%rsp)
	cvttpd2dq	832(%rsp), %xmm0
	unpcklpd	%xmm1, %xmm2                    # xmm2 = xmm2[0],xmm1[0]
	cvttpd2dq	816(%rsp), %xmm1
	movapd	%xmm2, 304(%rsp)
	unpcklpd	%xmm0, %xmm1                    # xmm1 = xmm1[0],xmm0[0]
	movapd	%xmm1, 320(%rsp)
	xorl	%eax, %eax
	.p2align	4, 0x90
.LBB6_13:                               #   Parent Loop BB6_10 Depth=1
                                        #     Parent Loop BB6_12 Depth=2
                                        # =>    This Inner Loop Header: Depth=3
	movl	80(%rsp,%rax), %edx
	movl	%edx, %ecx
	negl	%ecx
	cmovll	%edx, %ecx
	xorl	%edx, %edx
	cmpl	$1, %ecx
	sete	%dl
	leal	(%rdx,%r13), %esi
	movl	%ecx, %edi
	andl	$2147483646, %edi               # imm = 0x7FFFFFFE
	cmpl	$2, %edi
	leal	2(%r13,%rdx), %edx
	cmovnel	%esi, %edx
	movl	%ecx, %esi
	andl	$2147483644, %esi               # imm = 0x7FFFFFFC
	leal	3(%rdx), %edi
	cmpl	$4, %esi
	cmovnel	%edx, %edi
	movl	%ecx, %edx
	andl	$2147483640, %edx               # imm = 0x7FFFFFF8
	leal	4(%rdi), %esi
	cmpl	$8, %edx
	cmovnel	%edi, %esi
	movl	%ecx, %edx
	andl	$2147483632, %edx               # imm = 0x7FFFFFF0
	leal	5(%rsi), %edi
	cmpl	$16, %edx
	cmovnel	%esi, %edi
	movl	84(%rsp,%rax), %ebx
	movl	%ecx, %esi
	andl	$2147483616, %esi               # imm = 0x7FFFFFE0
	leal	6(%rdi), %ebp
	cmpl	$32, %esi
	cmovnel	%edi, %ebp
	andl	$2147483584, %ecx               # imm = 0x7FFFFFC0
	leal	7(%rbp), %esi
	cmpl	$64, %ecx
	cmovnel	%ebp, %esi
	movl	%ebx, %edx
	negl	%edx
	cmovll	%ebx, %edx
	xorl	%ebp, %ebp
	cmpl	$1, %edx
	sete	%bpl
	leal	(%rsi,%rbp), %edi
	movl	%edx, %ecx
	andl	$2147483646, %ecx               # imm = 0x7FFFFFFE
	cmpl	$2, %ecx
	leal	2(%rsi,%rbp), %ecx
	cmovnel	%edi, %ecx
	movl	%edx, %esi
	andl	$2147483644, %esi               # imm = 0x7FFFFFFC
	leal	3(%rcx), %edi
	cmpl	$4, %esi
	cmovnel	%ecx, %edi
	movl	%edx, %ecx
	andl	$2147483640, %ecx               # imm = 0x7FFFFFF8
	leal	4(%rdi), %esi
	cmpl	$8, %ecx
	cmovnel	%edi, %esi
	movl	%edx, %ecx
	andl	$2147483632, %ecx               # imm = 0x7FFFFFF0
	leal	5(%rsi), %edi
	cmpl	$16, %ecx
	cmovnel	%esi, %edi
	movl	%edx, %ecx
	andl	$2147483616, %ecx               # imm = 0x7FFFFFE0
	leal	6(%rdi), %esi
	cmpl	$32, %ecx
	cmovnel	%edi, %esi
	andl	$2147483584, %edx               # imm = 0x7FFFFFC0
	leal	7(%rsi), %edi
	cmpl	$64, %edx
	cmovnel	%esi, %edi
	movl	88(%rsp,%rax), %ecx
	movl	%ecx, %edx
	negl	%edx
	cmovll	%ecx, %edx
	xorl	%ebp, %ebp
	cmpl	$1, %edx
	sete	%bpl
	leal	(%rdi,%rbp), %esi
	movl	%edx, %ecx
	andl	$2147483646, %ecx               # imm = 0x7FFFFFFE
	cmpl	$2, %ecx
	leal	2(%rdi,%rbp), %ecx
	cmovnel	%esi, %ecx
	movl	%edx, %esi
	andl	$2147483644, %esi               # imm = 0x7FFFFFFC
	leal	3(%rcx), %edi
	cmpl	$4, %esi
	cmovnel	%ecx, %edi
	movl	%edx, %ecx
	andl	$2147483640, %ecx               # imm = 0x7FFFFFF8
	leal	4(%rdi), %esi
	cmpl	$8, %ecx
	cmovnel	%edi, %esi
	movl	%edx, %ecx
	andl	$2147483632, %ecx               # imm = 0x7FFFFFF0
	leal	5(%rsi), %edi
	cmpl	$16, %ecx
	cmovnel	%esi, %edi
	movl	%edx, %ecx
	andl	$2147483616, %ecx               # imm = 0x7FFFFFE0
	leal	6(%rdi), %esi
	cmpl	$32, %ecx
	cmovnel	%edi, %esi
	andl	$2147483584, %edx               # imm = 0x7FFFFFC0
	leal	7(%rsi), %edi
	cmpl	$64, %edx
	cmovnel	%esi, %edi
	movl	92(%rsp,%rax), %ecx
	movl	%ecx, %edx
	negl	%edx
	cmovll	%ecx, %edx
	xorl	%ebp, %ebp
	cmpl	$1, %edx
	sete	%bpl
	leal	(%rdi,%rbp), %esi
	movl	%edx, %ecx
	andl	$2147483646, %ecx               # imm = 0x7FFFFFFE
	cmpl	$2, %ecx
	leal	2(%rdi,%rbp), %ecx
	cmovnel	%esi, %ecx
	movl	%edx, %esi
	andl	$2147483644, %esi               # imm = 0x7FFFFFFC
	leal	3(%rcx), %edi
	cmpl	$4, %esi
	cmovnel	%ecx, %edi
	movl	%edx, %ecx
	andl	$2147483640, %ecx               # imm = 0x7FFFFFF8
	leal	4(%rdi), %esi
	cmpl	$8, %ecx
	cmovnel	%edi, %esi
	movl	%edx, %ecx
	andl	$2147483632, %ecx               # imm = 0x7FFFFFF0
	leal	5(%rsi), %edi
	cmpl	$16, %ecx
	cmovnel	%esi, %edi
	movl	%edx, %ecx
	andl	$2147483616, %ecx               # imm = 0x7FFFFFE0
	leal	6(%rdi), %esi
	cmpl	$32, %ecx
	cmovnel	%edi, %esi
	andl	$2147483584, %edx               # imm = 0x7FFFFFC0
	leal	7(%rsi), %edi
	cmpl	$64, %edx
	cmovnel	%esi, %edi
	movl	96(%rsp,%rax), %ecx
	movl	%ecx, %edx
	negl	%edx
	cmovll	%ecx, %edx
	xorl	%ebp, %ebp
	cmpl	$1, %edx
	sete	%bpl
	leal	(%rdi,%rbp), %esi
	movl	%edx, %ecx
	andl	$2147483646, %ecx               # imm = 0x7FFFFFFE
	cmpl	$2, %ecx
	leal	2(%rdi,%rbp), %ecx
	cmovnel	%esi, %ecx
	movl	%edx, %esi
	andl	$2147483644, %esi               # imm = 0x7FFFFFFC
	leal	3(%rcx), %edi
	cmpl	$4, %esi
	cmovnel	%ecx, %edi
	movl	%edx, %ecx
	andl	$2147483640, %ecx               # imm = 0x7FFFFFF8
	leal	4(%rdi), %esi
	cmpl	$8, %ecx
	cmovnel	%edi, %esi
	movl	%edx, %ecx
	andl	$2147483632, %ecx               # imm = 0x7FFFFFF0
	leal	5(%rsi), %edi
	cmpl	$16, %ecx
	cmovnel	%esi, %edi
	movl	%edx, %ecx
	andl	$2147483616, %ecx               # imm = 0x7FFFFFE0
	leal	6(%rdi), %esi
	cmpl	$32, %ecx
	cmovnel	%edi, %esi
	andl	$2147483584, %edx               # imm = 0x7FFFFFC0
	leal	7(%rsi), %edi
	cmpl	$64, %edx
	cmovnel	%esi, %edi
	movl	100(%rsp,%rax), %ecx
	movl	%ecx, %edx
	negl	%edx
	cmovll	%ecx, %edx
	xorl	%ebp, %ebp
	cmpl	$1, %edx
	sete	%bpl
	leal	(%rdi,%rbp), %esi
	movl	%edx, %ecx
	andl	$2147483646, %ecx               # imm = 0x7FFFFFFE
	cmpl	$2, %ecx
	leal	2(%rdi,%rbp), %ecx
	cmovnel	%esi, %ecx
	movl	%edx, %esi
	andl	$2147483644, %esi               # imm = 0x7FFFFFFC
	leal	3(%rcx), %edi
	cmpl	$4, %esi
	cmovnel	%ecx, %edi
	movl	%edx, %ecx
	andl	$2147483640, %ecx               # imm = 0x7FFFFFF8
	leal	4(%rdi), %esi
	cmpl	$8, %ecx
	cmovnel	%edi, %esi
	movl	%edx, %ecx
	andl	$2147483632, %ecx               # imm = 0x7FFFFFF0
	leal	5(%rsi), %edi
	cmpl	$16, %ecx
	cmovnel	%esi, %edi
	movl	%edx, %ecx
	andl	$2147483616, %ecx               # imm = 0x7FFFFFE0
	leal	6(%rdi), %esi
	cmpl	$32, %ecx
	cmovnel	%edi, %esi
	andl	$2147483584, %edx               # imm = 0x7FFFFFC0
	leal	7(%rsi), %edi
	cmpl	$64, %edx
	cmovnel	%esi, %edi
	movl	104(%rsp,%rax), %ecx
	movl	%ecx, %edx
	negl	%edx
	cmovll	%ecx, %edx
	xorl	%ebp, %ebp
	cmpl	$1, %edx
	sete	%bpl
	leal	(%rdi,%rbp), %esi
	movl	%edx, %ecx
	andl	$2147483646, %ecx               # imm = 0x7FFFFFFE
	cmpl	$2, %ecx
	leal	2(%rdi,%rbp), %ecx
	cmovnel	%esi, %ecx
	movl	%edx, %esi
	andl	$2147483644, %esi               # imm = 0x7FFFFFFC
	leal	3(%rcx), %edi
	cmpl	$4, %esi
	cmovnel	%ecx, %edi
	movl	%edx, %ecx
	andl	$2147483640, %ecx               # imm = 0x7FFFFFF8
	leal	4(%rdi), %esi
	cmpl	$8, %ecx
	cmovnel	%edi, %esi
	movl	%edx, %ecx
	andl	$2147483632, %ecx               # imm = 0x7FFFFFF0
	leal	5(%rsi), %edi
	cmpl	$16, %ecx
	cmovnel	%esi, %edi
	movl	%edx, %ecx
	andl	$2147483616, %ecx               # imm = 0x7FFFFFE0
	leal	6(%rdi), %esi
	cmpl	$32, %ecx
	cmovnel	%edi, %esi
	andl	$2147483584, %edx               # imm = 0x7FFFFFC0
	leal	7(%rsi), %edi
	cmpl	$64, %edx
	cmovnel	%esi, %edi
	movl	108(%rsp,%rax), %ecx
	movl	%ecx, %edx
	negl	%edx
	cmovll	%ecx, %edx
	xorl	%ebp, %ebp
	cmpl	$1, %edx
	sete	%bpl
	leal	(%rdi,%rbp), %esi
	movl	%edx, %ecx
	andl	$2147483646, %ecx               # imm = 0x7FFFFFFE
	cmpl	$2, %ecx
	leal	2(%rdi,%rbp), %ecx
	cmovnel	%esi, %ecx
	movl	%edx, %esi
	andl	$2147483644, %esi               # imm = 0x7FFFFFFC
	leal	3(%rcx), %edi
	cmpl	$4, %esi
	cmovnel	%ecx, %edi
	movl	%edx, %ecx
	andl	$2147483640, %ecx               # imm = 0x7FFFFFF8
	leal	4(%rdi), %esi
	cmpl	$8, %ecx
	cmovnel	%edi, %esi
	movl	%edx, %ecx
	andl	$2147483632, %ecx               # imm = 0x7FFFFFF0
	leal	5(%rsi), %edi
	cmpl	$16, %ecx
	cmovnel	%esi, %edi
	movl	%edx, %ecx
	andl	$2147483616, %ecx               # imm = 0x7FFFFFE0
	leal	6(%rdi), %esi
	cmpl	$32, %ecx
	cmovnel	%edi, %esi
	andl	$2147483584, %edx               # imm = 0x7FFFFFC0
	leal	7(%rsi), %r13d
	cmpl	$64, %edx
	cmovnel	%esi, %r13d
	addq	$32, %rax
	cmpq	$256, %rax                      # imm = 0x100
	jne	.LBB6_13
# %bb.14:                               #   in Loop: Header=BB6_12 Depth=2
	movzwl	38(%rsp), %esi
	movq	16(%rsp), %rdi                  # 8-byte Reload
	leaq	80(%rsp), %rdx
	movl	%r14d, %ecx
	movl	12(%rsp), %ebx                  # 4-byte Reload
	movl	%ebx, %r8d
	callq	idct_ii
	addl	$1, %r15d
	movl	$.L.str.6, %edi
	movl	%r15d, %esi
	movl	8(%rsp), %edx                   # 4-byte Reload
	xorl	%eax, %eax
	callq	printf
	movq	stdout(%rip), %rdi
	callq	fflush
	addl	$1, %r12d
	movzwl	38(%rsp), %ebp
	movl	%ebp, %eax
	shrl	$3, %eax
	cmpl	%eax, %r12d
	jb	.LBB6_12
# %bb.15:                               #   in Loop: Header=BB6_10 Depth=1
	movzwl	40(%rsp), %r14d
	movq	72(%rsp), %rcx                  # 8-byte Reload
	jmp	.LBB6_16
.LBB6_1:
	xorl	%r13d, %r13d
.LBB6_17:
	movl	$.L.str.7, %edi
	xorl	%esi, %esi
	xorl	%eax, %eax
	callq	printf
	leal	7(%r13), %esi
	testl	%r13d, %r13d
	cmovnsl	%r13d, %esi
	sarl	$3, %esi
	movl	$.L.str.8, %edi
	xorl	%eax, %eax
	callq	printf
	movq	stdout(%rip), %rsi
	movl	$10, %edi
	callq	putc
	movzwl	38(%rsp), %edx
	movzwl	40(%rsp), %ecx
	movl	$.L.str.10, %edi
	movq	16(%rsp), %rsi                  # 8-byte Reload
	callq	tga_write_mono
	testl	%eax, %eax
	jne	.LBB6_19
# %bb.18:
	leaq	24(%rsp), %rdi
	callq	tga_free_buffers
	xorl	%eax, %eax
	addq	$856, %rsp                      # imm = 0x358
	.cfi_def_cfa_offset 56
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%r12
	.cfi_def_cfa_offset 40
	popq	%r13
	.cfi_def_cfa_offset 32
	popq	%r14
	.cfi_def_cfa_offset 24
	popq	%r15
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	retq
.LBB6_19:
	.cfi_def_cfa_offset 912
	movl	%eax, %edi
	callq	tga_error
	movl	$.L.str, %edi
	movq	%rax, %rsi
	xorl	%eax, %eax
	callq	printf
	movl	$1, %edi
	callq	exit
.Lfunc_end6:
	.size	main, .Lfunc_end6-main
	.cfi_endproc
                                        # -- End function
	.type	.L.str,@object                  # @.str
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str:
	.asciz	"Targa error: %s\n"
	.size	.L.str, 17

	.type	.L.str.1,@object                # @.str.1
.L.str.1:
	.asciz	"Loaded %dx%dx%dbpp targa (\"%s\").\n"
	.size	.L.str.1, 34

	.type	.L.str.3,@object                # @.str.3
.L.str.3:
	.asciz	"lenna.tga"
	.size	.L.str.3, 10

	.type	.L.str.4,@object                # @.str.4
.L.str.4:
	.asciz	"%4d "
	.size	.L.str.4, 5

	.type	.L.str.6,@object                # @.str.6
.L.str.6:
	.asciz	"processed %d/%d blocks.\r"
	.size	.L.str.6, 25

	.type	.L.str.7,@object                # @.str.7
.L.str.7:
	.asciz	"%d non-zero coefficients"
	.size	.L.str.7, 25

	.type	.L.str.8,@object                # @.str.8
.L.str.8:
	.asciz	"%d data RLE bytes"
	.size	.L.str.8, 18

	.type	.L.str.10,@object               # @.str.10
.L.str.10:
	.asciz	"out.tga"
	.size	.L.str.10, 8

	.type	.Lstr,@object                   # @str
.Lstr:
	.asciz	"Width and height must be multiples of 8"
	.size	.Lstr, 40

	.type	.Lstr.11,@object                # @str.11
.Lstr.11:
	.asciz	";"
	.size	.Lstr.11, 2

	.ident	"Ubuntu clang version 11.0.0-2"
	.section	".note.GNU-stack","",@progbits
	.addrsig
