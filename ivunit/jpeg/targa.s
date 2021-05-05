	.text
	.file	"targa.c"
	.globl	tga_get_attribute_bits          # -- Begin function tga_get_attribute_bits
	.p2align	4, 0x90
	.type	tga_get_attribute_bits,@function
tga_get_attribute_bits:                 # @tga_get_attribute_bits
	.cfi_startproc
# %bb.0:
	movb	19(%rdi), %al
	andb	$15, %al
	retq
.Lfunc_end0:
	.size	tga_get_attribute_bits, .Lfunc_end0-tga_get_attribute_bits
	.cfi_endproc
                                        # -- End function
	.globl	tga_is_right_to_left            # -- Begin function tga_is_right_to_left
	.p2align	4, 0x90
	.type	tga_is_right_to_left,@function
tga_is_right_to_left:                   # @tga_is_right_to_left
	.cfi_startproc
# %bb.0:
	movzbl	19(%rdi), %eax
	shrl	$4, %eax
	andl	$1, %eax
	retq
.Lfunc_end1:
	.size	tga_is_right_to_left, .Lfunc_end1-tga_is_right_to_left
	.cfi_endproc
                                        # -- End function
	.globl	tga_is_top_to_bottom            # -- Begin function tga_is_top_to_bottom
	.p2align	4, 0x90
	.type	tga_is_top_to_bottom,@function
tga_is_top_to_bottom:                   # @tga_is_top_to_bottom
	.cfi_startproc
# %bb.0:
	movzbl	19(%rdi), %eax
	shrl	$5, %eax
	andl	$1, %eax
	retq
.Lfunc_end2:
	.size	tga_is_top_to_bottom, .Lfunc_end2-tga_is_top_to_bottom
	.cfi_endproc
                                        # -- End function
	.globl	tga_is_colormapped              # -- Begin function tga_is_colormapped
	.p2align	4, 0x90
	.type	tga_is_colormapped,@function
tga_is_colormapped:                     # @tga_is_colormapped
	.cfi_startproc
# %bb.0:
	movb	2(%rdi), %cl
	andb	$-9, %cl
	xorl	%eax, %eax
	cmpb	$1, %cl
	sete	%al
	retq
.Lfunc_end3:
	.size	tga_is_colormapped, .Lfunc_end3-tga_is_colormapped
	.cfi_endproc
                                        # -- End function
	.globl	tga_is_rle                      # -- Begin function tga_is_rle
	.p2align	4, 0x90
	.type	tga_is_rle,@function
tga_is_rle:                             # @tga_is_rle
	.cfi_startproc
# %bb.0:
	movb	2(%rdi), %cl
	addb	$-9, %cl
	xorl	%eax, %eax
	cmpb	$3, %cl
	setb	%al
	retq
.Lfunc_end4:
	.size	tga_is_rle, .Lfunc_end4-tga_is_rle
	.cfi_endproc
                                        # -- End function
	.globl	tga_is_mono                     # -- Begin function tga_is_mono
	.p2align	4, 0x90
	.type	tga_is_mono,@function
tga_is_mono:                            # @tga_is_mono
	.cfi_startproc
# %bb.0:
	movb	2(%rdi), %cl
	andb	$-9, %cl
	xorl	%eax, %eax
	cmpb	$3, %cl
	sete	%al
	retq
.Lfunc_end5:
	.size	tga_is_mono, .Lfunc_end5-tga_is_mono
	.cfi_endproc
                                        # -- End function
	.globl	tga_error                       # -- Begin function tga_error
	.p2align	4, 0x90
	.type	tga_error,@function
tga_error:                              # @tga_error
	.cfi_startproc
# %bb.0:
	cmpl	$17, %edi
	ja	.LBB6_2
# %bb.1:
	movslq	%edi, %rax
	movq	.Lswitch.table.tga_error(,%rax,8), %rax
	retq
.LBB6_2:
	movl	$.L.str.18, %eax
	retq
.Lfunc_end6:
	.size	tga_error, .Lfunc_end6-tga_error
	.cfi_endproc
                                        # -- End function
	.globl	tga_read                        # -- Begin function tga_read
	.p2align	4, 0x90
	.type	tga_read,@function
tga_read:                               # @tga_read
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	pushq	%r14
	.cfi_def_cfa_offset 24
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset %rbx, -32
	.cfi_offset %r14, -24
	.cfi_offset %rbp, -16
	movq	%rsi, %rax
	movq	%rdi, %r14
	movl	$.L.str.19, %esi
	movq	%rax, %rdi
	callq	fopen
	testq	%rax, %rax
	je	.LBB7_2
# %bb.1:
	movq	%rax, %rbx
	movq	%r14, %rdi
	movq	%rax, %rsi
	callq	tga_read_from_FILE
	movl	%eax, %ebp
	movq	%rbx, %rdi
	callq	fclose
	movl	%ebp, %eax
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	retq
.LBB7_2:
	.cfi_def_cfa_offset 32
	movl	$1, %eax
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end7:
	.size	tga_read, .Lfunc_end7-tga_read
	.cfi_endproc
                                        # -- End function
	.globl	tga_read_from_FILE              # -- Begin function tga_read_from_FILE
	.p2align	4, 0x90
	.type	tga_read_from_FILE,@function
tga_read_from_FILE:                     # @tga_read_from_FILE
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
	subq	$72, %rsp
	.cfi_def_cfa_offset 128
	.cfi_offset %rbx, -56
	.cfi_offset %r12, -48
	.cfi_offset %r13, -40
	.cfi_offset %r14, -32
	.cfi_offset %r15, -24
	.cfi_offset %rbp, -16
	movq	%rsi, %r12
	movq	%rdi, %rbp
	leaq	24(%rdi), %r14
	leaq	32(%rdi), %rbx
	leaq	40(%rdi), %r13
	xorps	%xmm0, %xmm0
	movups	%xmm0, 24(%rdi)
	movq	$0, 40(%rdi)
	movl	$1, %esi
	movl	$1, %edx
	movq	%r12, %rcx
	callq	fread
	cmpq	$1, %rax
	jne	.LBB8_13
# %bb.1:
	leaq	1(%rbp), %r15
	movl	$1, %esi
	movl	$1, %edx
	movq	%r15, %rdi
	movq	%r12, %rcx
	callq	fread
	cmpq	$1, %rax
	jne	.LBB8_13
# %bb.2:
	cmpb	$2, (%r15)
	jae	.LBB8_20
# %bb.3:
	movq	%rbx, (%rsp)                    # 8-byte Spill
	movq	%r13, 8(%rsp)                   # 8-byte Spill
	leaq	2(%rbp), %r13
	movl	$1, %esi
	movl	$1, %edx
	movq	%r13, %rdi
	movq	%r12, %rcx
	callq	fread
	cmpq	$1, %rax
	jne	.LBB8_53
# %bb.4:
	movzbl	(%r13), %eax
	cmpq	$11, %rax
	ja	.LBB8_66
# %bb.5:
	movl	$3598, %ecx                     # imm = 0xE0E
	btq	%rax, %rcx
	jae	.LBB8_60
# %bb.6:
	andb	$-9, %al
	movb	(%r15), %cl
	cmpb	$1, %al
	jne	.LBB8_25
# %bb.7:
	testb	%cl, %cl
	jne	.LBB8_31
# %bb.8:
	movq	(%r14), %rdi
	testq	%rdi, %rdi
	je	.LBB8_10
# %bb.9:
	callq	free
	movq	$0, (%r14)
.LBB8_10:
	movq	(%rsp), %rbp                    # 8-byte Reload
	movq	(%rbp), %rdi
	testq	%rdi, %rdi
	movq	8(%rsp), %rbx                   # 8-byte Reload
	je	.LBB8_12
# %bb.11:
	callq	free
	movq	$0, (%rbp)
.LBB8_12:
	movq	(%rbx), %rdi
	movl	$7, %ebp
	jmp	.LBB8_58
.LBB8_13:
	movq	(%r14), %rdi
	testq	%rdi, %rdi
	je	.LBB8_15
# %bb.14:
	callq	free
	movq	$0, (%r14)
.LBB8_15:
	movq	(%rbx), %rdi
	testq	%rdi, %rdi
	je	.LBB8_17
# %bb.16:
	callq	free
	movq	$0, (%rbx)
.LBB8_17:
	movq	(%r13), %rdi
	movl	$2, %ebp
	testq	%rdi, %rdi
	je	.LBB8_19
.LBB8_18:
	callq	free
	movq	$0, (%r13)
.LBB8_19:
	movl	%ebp, %eax
	addq	$72, %rsp
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
.LBB8_20:
	.cfi_def_cfa_offset 128
	movq	(%r14), %rdi
	testq	%rdi, %rdi
	je	.LBB8_22
# %bb.21:
	callq	free
	movq	$0, (%r14)
.LBB8_22:
	movq	(%rbx), %rdi
	testq	%rdi, %rdi
	je	.LBB8_24
# %bb.23:
	callq	free
	movq	$0, (%rbx)
.LBB8_24:
	movq	(%r13), %rdi
	movl	$4, %ebp
	testq	%rdi, %rdi
	jne	.LBB8_18
	jmp	.LBB8_19
.LBB8_25:
	cmpb	$1, %cl
	jne	.LBB8_31
# %bb.26:
	movq	(%r14), %rdi
	testq	%rdi, %rdi
	je	.LBB8_28
# %bb.27:
	callq	free
	movq	$0, (%r14)
.LBB8_28:
	movq	(%rsp), %rbp                    # 8-byte Reload
	movq	(%rbp), %rdi
	testq	%rdi, %rdi
	movq	8(%rsp), %rbx                   # 8-byte Reload
	je	.LBB8_30
# %bb.29:
	callq	free
	movq	$0, (%rbp)
.LBB8_30:
	movq	(%rbx), %rdi
	movl	$8, %ebp
	jmp	.LBB8_58
.LBB8_31:
	leaq	4(%rbp), %rbx
	movl	$2, %esi
	movl	$1, %edx
	movq	%rbx, %rdi
	movq	%r12, %rcx
	callq	fread
	cmpq	$1, %rax
	jne	.LBB8_53
# %bb.32:
	leaq	6(%rbp), %rdi
	movl	$2, %esi
	movl	$1, %edx
	movq	%rdi, 32(%rsp)                  # 8-byte Spill
	movq	%r12, %rcx
	callq	fread
	cmpq	$1, %rax
	jne	.LBB8_53
# %bb.33:
	leaq	8(%rbp), %rdi
	movl	$1, %esi
	movl	$1, %edx
	movq	%rdi, 24(%rsp)                  # 8-byte Spill
	movq	%r12, %rcx
	callq	fread
	cmpq	$1, %rax
	jne	.LBB8_53
# %bb.34:
	cmpb	$1, (%r15)
	jne	.LBB8_38
# %bb.35:
	movq	32(%rsp), %rax                  # 8-byte Reload
	cmpw	$0, (%rax)
	je	.LBB8_71
# %bb.36:
	movq	24(%rsp), %rax                  # 8-byte Reload
	movzbl	(%rax), %eax
	cmpq	$32, %rax
	ja	.LBB8_76
# %bb.37:
	movabsq	$4311810048, %rcx               # imm = 0x101010000
	btq	%rax, %rcx
	jae	.LBB8_76
.LBB8_38:
	leaq	10(%rbp), %rdi
	movl	$2, %esi
	movl	$1, %edx
	movq	%r12, %rcx
	callq	fread
	cmpq	$1, %rax
	jne	.LBB8_53
# %bb.39:
	leaq	12(%rbp), %rdi
	movl	$2, %esi
	movl	$1, %edx
	movq	%r12, %rcx
	callq	fread
	cmpq	$1, %rax
	jne	.LBB8_53
# %bb.40:
	leaq	14(%rbp), %rdi
	movl	$2, %esi
	movl	$1, %edx
	movq	%rdi, 64(%rsp)                  # 8-byte Spill
	movq	%r12, %rcx
	callq	fread
	cmpq	$1, %rax
	jne	.LBB8_53
# %bb.41:
	leaq	16(%rbp), %rdi
	movl	$2, %esi
	movl	$1, %edx
	movq	%rdi, 56(%rsp)                  # 8-byte Spill
	movq	%r12, %rcx
	callq	fread
	cmpq	$1, %rax
	jne	.LBB8_53
# %bb.42:
	movq	64(%rsp), %rax                  # 8-byte Reload
	cmpw	$0, (%rax)
	je	.LBB8_81
# %bb.43:
	movq	56(%rsp), %rax                  # 8-byte Reload
	cmpw	$0, (%rax)
	je	.LBB8_81
# %bb.44:
	leaq	18(%rbp), %rdi
	movl	$1, %esi
	movl	$1, %edx
	movq	%rdi, 48(%rsp)                  # 8-byte Spill
	movq	%r12, %rcx
	callq	fread
	cmpq	$1, %rax
	jne	.LBB8_96
# %bb.45:
	movq	48(%rsp), %rax                  # 8-byte Reload
	movb	(%rax), %al
	leal	-8(%rax), %ecx
	rolb	$5, %cl
	cmpb	$3, %cl
	ja	.LBB8_48
# %bb.46:
	cmpb	$8, %al
	je	.LBB8_86
# %bb.47:
	movb	(%r13), %al
	andb	$-9, %al
	cmpb	$1, %al
	jne	.LBB8_86
.LBB8_48:
	movq	(%r14), %rdi
	testq	%rdi, %rdi
	je	.LBB8_50
# %bb.49:
	callq	free
	movq	$0, (%r14)
.LBB8_50:
	movq	(%rsp), %rax                    # 8-byte Reload
	movq	(%rax), %rdi
	testq	%rdi, %rdi
	je	.LBB8_52
# %bb.51:
	callq	free
	movq	(%rsp), %rax                    # 8-byte Reload
	movq	$0, (%rax)
.LBB8_52:
	movq	8(%rsp), %rax                   # 8-byte Reload
	movq	(%rax), %rdi
	movl	$12, %ebp
	jmp	.LBB8_101
.LBB8_53:
	movq	(%r14), %rdi
	testq	%rdi, %rdi
	je	.LBB8_55
# %bb.54:
	callq	free
	movq	$0, (%r14)
.LBB8_55:
	movq	(%rsp), %rbp                    # 8-byte Reload
	movq	(%rbp), %rdi
	testq	%rdi, %rdi
	movq	8(%rsp), %rbx                   # 8-byte Reload
	je	.LBB8_57
# %bb.56:
	callq	free
	movq	$0, (%rbp)
.LBB8_57:
	movq	(%rbx), %rdi
	movl	$2, %ebp
.LBB8_58:
	testq	%rdi, %rdi
	je	.LBB8_19
# %bb.59:
	callq	free
	movq	$0, (%rbx)
	jmp	.LBB8_19
.LBB8_60:
	testq	%rax, %rax
	jne	.LBB8_66
# %bb.61:
	movq	(%r14), %rdi
	testq	%rdi, %rdi
	je	.LBB8_63
# %bb.62:
	callq	free
	movq	$0, (%r14)
.LBB8_63:
	movq	(%rsp), %rbp                    # 8-byte Reload
	movq	(%rbp), %rdi
	testq	%rdi, %rdi
	movq	8(%rsp), %rbx                   # 8-byte Reload
	je	.LBB8_65
# %bb.64:
	callq	free
	movq	$0, (%rbp)
.LBB8_65:
	movq	(%rbx), %rdi
	movl	$6, %ebp
	jmp	.LBB8_58
.LBB8_66:
	movq	(%r14), %rdi
	testq	%rdi, %rdi
	je	.LBB8_68
# %bb.67:
	callq	free
	movq	$0, (%r14)
.LBB8_68:
	movq	(%rsp), %rbp                    # 8-byte Reload
	movq	(%rbp), %rdi
	testq	%rdi, %rdi
	movq	8(%rsp), %rbx                   # 8-byte Reload
	je	.LBB8_70
# %bb.69:
	callq	free
	movq	$0, (%rbp)
.LBB8_70:
	movq	(%rbx), %rdi
	movl	$5, %ebp
	jmp	.LBB8_58
.LBB8_71:
	movq	(%r14), %rdi
	testq	%rdi, %rdi
	je	.LBB8_73
# %bb.72:
	callq	free
	movq	$0, (%r14)
.LBB8_73:
	movq	(%rsp), %rbp                    # 8-byte Reload
	movq	(%rbp), %rdi
	testq	%rdi, %rdi
	movq	8(%rsp), %rbx                   # 8-byte Reload
	je	.LBB8_75
# %bb.74:
	callq	free
	movq	$0, (%rbp)
.LBB8_75:
	movq	(%rbx), %rdi
	movl	$9, %ebp
	jmp	.LBB8_58
.LBB8_76:
	movq	(%r14), %rdi
	testq	%rdi, %rdi
	je	.LBB8_78
# %bb.77:
	callq	free
	movq	$0, (%r14)
.LBB8_78:
	movq	(%rsp), %rbp                    # 8-byte Reload
	movq	(%rbp), %rdi
	testq	%rdi, %rdi
	movq	8(%rsp), %rbx                   # 8-byte Reload
	je	.LBB8_80
# %bb.79:
	callq	free
	movq	$0, (%rbp)
.LBB8_80:
	movq	(%rbx), %rdi
	movl	$10, %ebp
	jmp	.LBB8_58
.LBB8_81:
	movq	(%r14), %rdi
	testq	%rdi, %rdi
	je	.LBB8_83
# %bb.82:
	callq	free
	movq	$0, (%r14)
.LBB8_83:
	movq	(%rsp), %rbx                    # 8-byte Reload
	movq	(%rbx), %rdi
	testq	%rdi, %rdi
	je	.LBB8_85
# %bb.84:
	callq	free
	movq	$0, (%rbx)
.LBB8_85:
	movq	8(%rsp), %rbx                   # 8-byte Reload
	movq	(%rbx), %rdi
	movl	$11, %ebp
	jmp	.LBB8_58
.LBB8_86:
	leaq	19(%rbp), %rdi
	movl	$1, %esi
	movl	$1, %edx
	movq	%r12, %rcx
	callq	fread
	cmpq	$1, %rax
	jne	.LBB8_96
# %bb.87:
	movzbl	(%rbp), %ebp
	testq	%rbp, %rbp
	je	.LBB8_90
# %bb.88:
	movq	%rbp, %rdi
	callq	malloc
	movq	%rax, (%r14)
	testq	%rax, %rax
	je	.LBB8_120
# %bb.89:
	movl	$1, %edx
	movq	%rax, %rdi
	movq	%rbp, %rsi
	movq	%r12, %rcx
	callq	fread
	cmpq	$1, %rax
	jne	.LBB8_96
.LBB8_90:
	cmpb	$1, (%r15)
	jne	.LBB8_93
# %bb.91:
	movzwl	(%rbx), %ebp
	movq	32(%rsp), %rax                  # 8-byte Reload
	movzwl	(%rax), %r15d
	leaq	(%r15,%rbp), %rdi
	movq	24(%rsp), %rax                  # 8-byte Reload
	movzbl	(%rax), %ebx
	imulq	%rbx, %rdi
	shrq	$3, %rdi
	callq	malloc
	movq	(%rsp), %rcx                    # 8-byte Reload
	movq	%rax, (%rcx)
	testq	%rax, %rax
	je	.LBB8_118
# %bb.92:
	imulq	%rbx, %rbp
	shrq	$3, %rbp
	addq	%rbp, %rax
	imulq	%rbx, %r15
	shrq	$3, %r15
	movl	$1, %edx
	movq	%rax, %rdi
	movq	%r15, %rsi
	movq	%r12, %rcx
	callq	fread
	cmpq	$1, %rax
	jne	.LBB8_96
.LBB8_93:
	movq	64(%rsp), %rax                  # 8-byte Reload
	movzwl	(%rax), %eax
	movq	56(%rsp), %rcx                  # 8-byte Reload
	movzwl	(%rcx), %ecx
	imull	%eax, %ecx
	movq	48(%rsp), %rax                  # 8-byte Reload
	movzbl	(%rax), %eax
	movl	%ecx, 32(%rsp)                  # 4-byte Spill
	movl	%ecx, %ebp
	movq	%rax, 24(%rsp)                  # 8-byte Spill
	imull	%eax, %ebp
	shrl	$3, %ebp
	movq	%rbp, %rdi
	callq	malloc
	movq	%rax, %rcx
	movq	8(%rsp), %rax                   # 8-byte Reload
	movq	%rcx, (%rax)
	testq	%rcx, %rcx
	je	.LBB8_103
# %bb.94:
	movq	%rcx, %rbx
	movb	(%r13), %al
	addb	$-9, %al
	cmpb	$2, %al
	jbe	.LBB8_104
# %bb.95:
	movl	$1, %edx
	movq	%rbx, %rdi
	movq	%rbp, %rsi
	movq	%r12, %rcx
	callq	fread
	xorl	%ebp, %ebp
	cmpq	$1, %rax
	je	.LBB8_19
.LBB8_96:
	movq	(%r14), %rdi
	testq	%rdi, %rdi
	je	.LBB8_98
# %bb.97:
	callq	free
	movq	$0, (%r14)
.LBB8_98:
	movq	(%rsp), %rax                    # 8-byte Reload
	movq	(%rax), %rdi
	testq	%rdi, %rdi
	je	.LBB8_100
# %bb.99:
	callq	free
	movq	(%rsp), %rax                    # 8-byte Reload
	movq	$0, (%rax)
.LBB8_100:
	movq	8(%rsp), %rax                   # 8-byte Reload
	movq	(%rax), %rdi
	movl	$2, %ebp
.LBB8_101:
	testq	%rdi, %rdi
	je	.LBB8_19
# %bb.102:
	callq	free
	movq	8(%rsp), %rax                   # 8-byte Reload
	movq	$0, (%rax)
	jmp	.LBB8_19
.LBB8_103:
	movq	(%r14), %rdi
	testq	%rdi, %rdi
	jne	.LBB8_119
	jmp	.LBB8_120
.LBB8_104:
	cmpl	$0, 32(%rsp)                    # 4-byte Folded Reload
	je	.LBB8_129
# %bb.105:
	shrq	$3, 24(%rsp)                    # 8-byte Folded Spill
	xorl	%r13d, %r13d
.LBB8_106:                              # =>This Loop Header: Depth=1
                                        #     Child Loop BB8_114 Depth 2
	movq	%r12, %rdi
	callq	feof
	testl	%eax, %eax
	jne	.LBB8_129
# %bb.107:                              #   in Loop: Header=BB8_106 Depth=1
	movl	$1, %esi
	movl	$1, %edx
	leaq	23(%rsp), %rdi
	movq	%r12, %rcx
	callq	fread
	movl	$2, %ebp
	cmpq	$1, %rax
	jne	.LBB8_124
# %bb.108:                              #   in Loop: Header=BB8_106 Depth=1
	movzbl	23(%rsp), %r15d
	testb	%r15b, %r15b
	js	.LBB8_112
# %bb.109:                              #   in Loop: Header=BB8_106 Depth=1
	addq	$1, %r15
	movl	%r13d, %eax
	addl	%r15d, %eax
	cmpl	32(%rsp), %eax                  # 4-byte Folded Reload
	ja	.LBB8_123
# %bb.110:                              #   in Loop: Header=BB8_106 Depth=1
	movl	%eax, %r13d
	movzbl	24(%rsp), %eax                  # 1-byte Folded Reload
	imulq	%rax, %r15
	movl	$1, %edx
	movq	%rbx, %rdi
	movq	%r15, %rsi
	movq	%r12, %rcx
	callq	fread
	cmpq	$1, %rax
	jne	.LBB8_124
# %bb.111:                              #   in Loop: Header=BB8_106 Depth=1
	addq	%r15, %rbx
	jmp	.LBB8_116
.LBB8_112:                              #   in Loop: Header=BB8_106 Depth=1
	movl	$1, %edx
	leaq	44(%rsp), %rdi
	movq	24(%rsp), %rsi                  # 8-byte Reload
	movq	%r12, %rcx
	callq	fread
	cmpq	$1, %rax
	jne	.LBB8_131
# %bb.113:                              #   in Loop: Header=BB8_106 Depth=1
	andb	$127, %r15b
	addb	$1, %r15b
.LBB8_114:                              #   Parent Loop BB8_106 Depth=1
                                        # =>  This Inner Loop Header: Depth=2
	movl	%r13d, %eax
	addl	$1, %eax
	cmpl	32(%rsp), %eax                  # 4-byte Folded Reload
	ja	.LBB8_123
# %bb.115:                              #   in Loop: Header=BB8_114 Depth=2
	movl	%eax, %r13d
	movq	%rbx, %rdi
	leaq	44(%rsp), %rsi
	movq	24(%rsp), %rbp                  # 8-byte Reload
	movq	%rbp, %rdx
	callq	memcpy
	addq	%rbp, %rbx
	addb	$-1, %r15b
	jne	.LBB8_114
.LBB8_116:                              #   in Loop: Header=BB8_106 Depth=1
	cmpl	32(%rsp), %r13d                 # 4-byte Folded Reload
	jb	.LBB8_106
.LBB8_129:
	xorl	%ebp, %ebp
	jmp	.LBB8_19
.LBB8_118:
	movq	(%r14), %rdi
	testq	%rdi, %rdi
	je	.LBB8_122
.LBB8_119:
	callq	free
	movq	$0, (%r14)
.LBB8_120:
	movq	(%rsp), %rax                    # 8-byte Reload
	movq	(%rax), %rdi
	testq	%rdi, %rdi
	je	.LBB8_122
# %bb.121:
	callq	free
	movq	(%rsp), %rax                    # 8-byte Reload
	movq	$0, (%rax)
.LBB8_122:
	movq	8(%rsp), %rax                   # 8-byte Reload
	movq	(%rax), %rdi
	movl	$13, %ebp
	jmp	.LBB8_101
.LBB8_123:
	movl	$15, %ebp
.LBB8_124:
	movq	(%r14), %rdi
	testq	%rdi, %rdi
	je	.LBB8_126
.LBB8_125:
	callq	free
	movq	$0, (%r14)
.LBB8_126:
	movq	(%rsp), %rax                    # 8-byte Reload
	movq	(%rax), %rdi
	testq	%rdi, %rdi
	je	.LBB8_128
# %bb.127:
	callq	free
	movq	(%rsp), %rax                    # 8-byte Reload
	movq	$0, (%rax)
.LBB8_128:
	movq	8(%rsp), %rax                   # 8-byte Reload
	movq	(%rax), %rdi
	jmp	.LBB8_101
.LBB8_131:
	movl	$2, %ebp
	movq	(%r14), %rdi
	testq	%rdi, %rdi
	jne	.LBB8_125
	jmp	.LBB8_126
.Lfunc_end8:
	.size	tga_read_from_FILE, .Lfunc_end8-tga_read_from_FILE
	.cfi_endproc
                                        # -- End function
	.globl	tga_free_buffers                # -- Begin function tga_free_buffers
	.p2align	4, 0x90
	.type	tga_free_buffers,@function
tga_free_buffers:                       # @tga_free_buffers
	.cfi_startproc
# %bb.0:
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset %rbx, -16
	movq	%rdi, %rbx
	movq	24(%rdi), %rdi
	testq	%rdi, %rdi
	je	.LBB9_2
# %bb.1:
	callq	free
	movq	$0, 24(%rbx)
.LBB9_2:
	movq	32(%rbx), %rdi
	testq	%rdi, %rdi
	je	.LBB9_4
# %bb.3:
	callq	free
	movq	$0, 32(%rbx)
.LBB9_4:
	movq	40(%rbx), %rdi
	testq	%rdi, %rdi
	je	.LBB9_6
# %bb.5:
	callq	free
	movq	$0, 40(%rbx)
.LBB9_6:
	popq	%rbx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end9:
	.size	tga_free_buffers, .Lfunc_end9-tga_free_buffers
	.cfi_endproc
                                        # -- End function
	.globl	tga_write                       # -- Begin function tga_write
	.p2align	4, 0x90
	.type	tga_write,@function
tga_write:                              # @tga_write
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	pushq	%r14
	.cfi_def_cfa_offset 24
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset %rbx, -32
	.cfi_offset %r14, -24
	.cfi_offset %rbp, -16
	movq	%rsi, %r14
	movl	$.L.str.20, %esi
	callq	fopen
	testq	%rax, %rax
	je	.LBB10_2
# %bb.1:
	movq	%rax, %rbx
	movq	%rax, %rdi
	movq	%r14, %rsi
	callq	tga_write_to_FILE
	movl	%eax, %ebp
	movq	%rbx, %rdi
	callq	fclose
	movl	%ebp, %eax
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	retq
.LBB10_2:
	.cfi_def_cfa_offset 32
	movl	$1, %eax
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end10:
	.size	tga_write, .Lfunc_end10-tga_write
	.cfi_endproc
                                        # -- End function
	.globl	tga_write_to_FILE               # -- Begin function tga_write_to_FILE
	.p2align	4, 0x90
	.type	tga_write_to_FILE,@function
tga_write_to_FILE:                      # @tga_write_to_FILE
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
	subq	$88, %rsp
	.cfi_def_cfa_offset 144
	.cfi_offset %rbx, -56
	.cfi_offset %r12, -48
	.cfi_offset %r13, -40
	.cfi_offset %r14, -32
	.cfi_offset %r15, -24
	.cfi_offset %rbp, -16
	movq	%rsi, %rbx
	movq	%rdi, %r14
	movl	$1, %esi
	movl	$1, %edx
	movq	%rbx, %rdi
	movq	%r14, %rcx
	callq	fwrite
	cmpq	$1, %rax
	jne	.LBB11_44
# %bb.1:
	movl	$4, %eax
	cmpb	$1, 1(%rbx)
	ja	.LBB11_45
# %bb.2:
	leaq	1(%rbx), %r12
	movl	$1, %esi
	movl	$1, %edx
	movq	%r12, %rdi
	movq	%r14, %rcx
	callq	fwrite
	cmpq	$1, %rax
	jne	.LBB11_44
# %bb.3:
	movzbl	2(%rbx), %ecx
	cmpq	$11, %rcx
	ja	.LBB11_16
# %bb.4:
	movl	$3598, %eax                     # imm = 0xE0E
	btq	%rcx, %rax
	jae	.LBB11_15
# %bb.5:
	leaq	2(%rbx), %r15
	movl	$1, %esi
	movl	$1, %edx
	movq	%r15, %rdi
	movq	%r14, %rcx
	callq	fwrite
	cmpq	$1, %rax
	jne	.LBB11_44
# %bb.6:
	movb	(%r15), %al
	andb	$-9, %al
	movb	(%r12), %cl
	cmpb	$1, %al
	jne	.LBB11_17
# %bb.7:
	testb	%cl, %cl
	je	.LBB11_39
# %bb.8:
	cmpb	$1, %cl
	jne	.LBB11_18
# %bb.9:
	cmpw	$0, 6(%rbx)
	je	.LBB11_41
# %bb.10:
	movzbl	8(%rbx), %ecx
	movl	$10, %eax
	cmpq	$32, %rcx
	ja	.LBB11_45
# %bb.11:
	movabsq	$4311810048, %rdx               # imm = 0x101010000
	btq	%rcx, %rdx
	jb	.LBB11_18
	jmp	.LBB11_45
.LBB11_15:
	movl	$6, %eax
	testq	%rcx, %rcx
	je	.LBB11_45
.LBB11_16:
	movl	$5, %eax
	jmp	.LBB11_45
.LBB11_17:
	movl	$8, %eax
	cmpb	$1, %cl
	je	.LBB11_45
.LBB11_18:
	movzwl	4(%rbx), %eax
	movw	%ax, 30(%rsp)
	leaq	30(%rsp), %rdi
	movl	$2, %esi
	movl	$1, %edx
	movq	%r14, %rcx
	callq	fwrite
	cmpq	$1, %rax
	jne	.LBB11_44
# %bb.19:
	movzwl	6(%rbx), %eax
	movw	%ax, 28(%rsp)
	leaq	28(%rsp), %rdi
	movl	$2, %esi
	movl	$1, %edx
	movq	%r14, %rcx
	callq	fwrite
	cmpq	$1, %rax
	jne	.LBB11_44
# %bb.20:
	leaq	8(%rbx), %rdi
	movl	$1, %esi
	movl	$1, %edx
	movq	%r14, %rcx
	callq	fwrite
	cmpq	$1, %rax
	jne	.LBB11_44
# %bb.21:
	movzwl	10(%rbx), %eax
	movw	%ax, 26(%rsp)
	leaq	26(%rsp), %rdi
	movl	$2, %esi
	movl	$1, %edx
	movq	%r14, %rcx
	callq	fwrite
	cmpq	$1, %rax
	jne	.LBB11_44
# %bb.22:
	movzwl	12(%rbx), %eax
	movw	%ax, 24(%rsp)
	leaq	24(%rsp), %rdi
	movl	$2, %esi
	movl	$1, %edx
	movq	%r14, %rcx
	callq	fwrite
	cmpq	$1, %rax
	jne	.LBB11_44
# %bb.23:
	movzwl	14(%rbx), %ecx
	movl	$11, %eax
	testw	%cx, %cx
	je	.LBB11_45
# %bb.24:
	cmpw	$0, 16(%rbx)
	je	.LBB11_45
# %bb.25:
	movw	%cx, 22(%rsp)
	leaq	22(%rsp), %rdi
	movl	$2, %esi
	movl	$1, %edx
	movq	%r14, %rcx
	callq	fwrite
	cmpq	$1, %rax
	jne	.LBB11_44
# %bb.26:
	movzwl	16(%rbx), %eax
	movw	%ax, 20(%rsp)
	leaq	20(%rsp), %rdi
	movl	$2, %esi
	movl	$1, %edx
	movq	%r14, %rcx
	callq	fwrite
	cmpq	$1, %rax
	jne	.LBB11_44
# %bb.27:
	movb	18(%rbx), %cl
	leal	-8(%rcx), %edx
	rolb	$5, %dl
	movl	$12, %eax
	cmpb	$3, %dl
	ja	.LBB11_45
# %bb.28:
	cmpb	$8, %cl
	je	.LBB11_30
# %bb.29:
	movb	(%r15), %cl
	andb	$-9, %cl
	cmpb	$1, %cl
	je	.LBB11_45
.LBB11_30:
	leaq	18(%rbx), %rdi
	movl	$1, %esi
	movl	$1, %edx
	movq	%r14, %rcx
	callq	fwrite
	cmpq	$1, %rax
	jne	.LBB11_44
# %bb.31:
	leaq	19(%rbx), %rdi
	movl	$1, %esi
	movl	$1, %edx
	movq	%r14, %rcx
	callq	fwrite
	cmpq	$1, %rax
	jne	.LBB11_44
# %bb.32:
	movzbl	(%rbx), %esi
	testq	%rsi, %rsi
	je	.LBB11_34
# %bb.33:
	leaq	24(%rbx), %rdi
	movl	$1, %edx
	movq	%r14, %rcx
	callq	fwrite
	cmpq	$1, %rax
	jne	.LBB11_44
.LBB11_34:
	cmpb	$1, (%r12)
	jne	.LBB11_36
# %bb.35:
	movzwl	4(%rbx), %edi
	movzbl	8(%rbx), %eax
	imulq	%rax, %rdi
	shrq	$3, %rdi
	addq	32(%rbx), %rdi
	movzwl	6(%rbx), %esi
	imulq	%rax, %rsi
	shrq	$3, %rsi
	movl	$1, %edx
	movq	%r14, %rcx
	callq	fwrite
	cmpq	$1, %rax
	jne	.LBB11_44
.LBB11_36:
	movb	(%r15), %al
	addb	$-9, %al
	cmpb	$2, %al
	jbe	.LBB11_51
# %bb.37:
	movq	40(%rbx), %rdi
	movzwl	14(%rbx), %eax
	movzwl	16(%rbx), %ecx
	imull	%eax, %ecx
	movzbl	18(%rbx), %esi
	imull	%ecx, %esi
	shrl	$3, %esi
	movl	$1, %edx
	movq	%r14, %rcx
	callq	fwrite
	movq	%rax, %rcx
	movl	$3, %eax
	cmpq	$1, %rcx
	jne	.LBB11_45
	jmp	.LBB11_92
.LBB11_39:
	movl	$7, %eax
	jmp	.LBB11_45
.LBB11_41:
	movl	$9, %eax
	jmp	.LBB11_45
.LBB11_51:
	movzwl	16(%rbx), %ecx
	testw	%cx, %cx
	je	.LBB11_92
# %bb.52:
	movzwl	14(%rbx), %eax
	xorl	%edx, %edx
	movq	%rdx, 56(%rsp)                  # 8-byte Spill
.LBB11_53:                              # =>This Loop Header: Depth=1
                                        #     Child Loop BB11_55 Depth 2
                                        #       Child Loop BB11_74 Depth 3
                                        #       Child Loop BB11_61 Depth 3
                                        #       Child Loop BB11_67 Depth 3
	xorl	%r13d, %r13d
	testw	%ax, %ax
	je	.LBB11_90
# %bb.54:                               #   in Loop: Header=BB11_53 Depth=1
	movq	40(%rbx), %rcx
	movzwl	%ax, %edx
	movq	56(%rsp), %rsi                  # 8-byte Reload
	movl	%esi, %edi
	imull	%edx, %edi
	movzbl	18(%rbx), %ebp
	imull	%ebp, %edi
	shrl	$3, %edi
	addq	%rcx, %rdi
	movq	%rdi, 32(%rsp)                  # 8-byte Spill
	movq	%rbp, %rdi
	shrq	$3, %rdi
	movq	%rdi, 8(%rsp)                   # 8-byte Spill
	imull	%esi, %edx
	movq	%rbp, 64(%rsp)                  # 8-byte Spill
                                        # kill: def $ebp killed $ebp killed $rbp def $rbp
	imull	%edx, %ebp
	shrl	$3, %ebp
	addq	%rcx, %rbp
	movq	%rbp, 72(%rsp)                  # 8-byte Spill
	movl	$0, 48(%rsp)                    # 4-byte Folded Spill
.LBB11_55:                              #   Parent Loop BB11_53 Depth=1
                                        # =>  This Loop Header: Depth=2
                                        #       Child Loop BB11_74 Depth 3
                                        #       Child Loop BB11_61 Depth 3
                                        #       Child Loop BB11_67 Depth 3
	movzwl	%ax, %eax
	movq	%rax, 40(%rsp)                  # 8-byte Spill
	addl	$-1, %eax
	movb	$1, %r12b
	movl	%eax, 52(%rsp)                  # 4-byte Spill
	cmpl	%r13d, %eax
	je	.LBB11_82
# %bb.56:                               #   in Loop: Header=BB11_55 Depth=2
	movl	%r13d, %edi
	movq	8(%rsp), %rdx                   # 8-byte Reload
	imull	%edx, %edi
	movq	32(%rsp), %rax                  # 8-byte Reload
	addq	%rax, %rdi
	leal	1(%r13), %r15d
	imull	%edx, %r15d
	addq	%rax, %r15
	movq	%rdi, 80(%rsp)                  # 8-byte Spill
	movq	%r15, %rsi
	callq	bcmp
	movq	40(%rsp), %rcx                  # 8-byte Reload
	addl	$-2, %ecx
	movl	%ecx, 16(%rsp)                  # 4-byte Spill
	testl	%eax, %eax
	je	.LBB11_64
.LBB11_57:                              #   in Loop: Header=BB11_55 Depth=2
	movb	$2, %r12b
	cmpl	%r13d, 16(%rsp)                 # 4-byte Folded Reload
	je	.LBB11_82
# %bb.58:                               #   in Loop: Header=BB11_55 Depth=2
	cmpb	$15, 64(%rsp)                   # 1-byte Folded Reload
	jbe	.LBB11_72
# %bb.59:                               #   in Loop: Header=BB11_55 Depth=2
	movl	$2, %r12d
	jmp	.LBB11_61
.LBB11_60:                              #   in Loop: Header=BB11_61 Depth=3
	addl	$1, %r12d
	cmpl	$128, %r12d
	je	.LBB11_81
.LBB11_61:                              #   Parent Loop BB11_53 Depth=1
                                        #     Parent Loop BB11_55 Depth=2
                                        # =>    This Inner Loop Header: Depth=3
	leal	(%r12,%r13), %eax
	cmpl	40(%rsp), %eax                  # 4-byte Folded Reload
	jae	.LBB11_82
# %bb.62:                               #   in Loop: Header=BB11_61 Depth=3
	movzwl	%ax, %esi
	cmpl	%esi, 52(%rsp)                  # 4-byte Folded Reload
	je	.LBB11_60
# %bb.63:                               #   in Loop: Header=BB11_61 Depth=3
	movl	%esi, %edi
	movq	8(%rsp), %rdx                   # 8-byte Reload
	imull	%edx, %edi
	movq	32(%rsp), %rax                  # 8-byte Reload
	addq	%rax, %rdi
	addl	$1, %esi
	imull	%edx, %esi
	addq	%rax, %rsi
	callq	bcmp
	testl	%eax, %eax
	jne	.LBB11_60
	jmp	.LBB11_82
.LBB11_64:                              #   in Loop: Header=BB11_55 Depth=2
	cmpb	$15, 64(%rsp)                   # 1-byte Folded Reload
	jbe	.LBB11_79
.LBB11_65:                              #   in Loop: Header=BB11_55 Depth=2
	movb	$2, %r12b
	cmpl	%r13d, 16(%rsp)                 # 4-byte Folded Reload
	je	.LBB11_71
# %bb.66:                               #   in Loop: Header=BB11_55 Depth=2
	movl	%r13d, %r15d
	movl	40(%rsp), %eax                  # 4-byte Reload
	movq	%rax, 40(%rsp)                  # 8-byte Spill
	leaq	2(%r15), %rbp
	imulq	8(%rsp), %rbp                   # 8-byte Folded Reload
	addq	72(%rsp), %rbp                  # 8-byte Folded Reload
	movl	$2, %r12d
.LBB11_67:                              #   Parent Loop BB11_53 Depth=1
                                        #     Parent Loop BB11_55 Depth=2
                                        # =>    This Inner Loop Header: Depth=3
	leaq	(%r15,%r12), %rax
	cmpq	40(%rsp), %rax                  # 8-byte Folded Reload
	jae	.LBB11_71
# %bb.68:                               #   in Loop: Header=BB11_67 Depth=3
	movq	80(%rsp), %rdi                  # 8-byte Reload
	movq	%rbp, %rsi
	movq	8(%rsp), %rdx                   # 8-byte Reload
	callq	bcmp
	testl	%eax, %eax
	jne	.LBB11_71
# %bb.69:                               #   in Loop: Header=BB11_67 Depth=3
	addq	$1, %r12
	addq	8(%rsp), %rbp                   # 8-byte Folded Reload
	cmpq	$128, %r12
	jne	.LBB11_67
# %bb.70:                               #   in Loop: Header=BB11_55 Depth=2
	movb	$-128, %r12b
.LBB11_71:                              #   in Loop: Header=BB11_55 Depth=2
	leal	-1(%r12), %eax
	orb	$-128, %al
	movb	$1, %r15b
	jmp	.LBB11_83
.LBB11_72:                              #   in Loop: Header=BB11_55 Depth=2
	movl	$2, %r12d
	jmp	.LBB11_74
.LBB11_73:                              #   in Loop: Header=BB11_74 Depth=3
	addl	$1, %r12d
	cmpl	$128, %r12d
	je	.LBB11_81
.LBB11_74:                              #   Parent Loop BB11_53 Depth=1
                                        #     Parent Loop BB11_55 Depth=2
                                        # =>    This Inner Loop Header: Depth=3
	leal	(%r12,%r13), %eax
	cmpl	40(%rsp), %eax                  # 4-byte Folded Reload
	jae	.LBB11_82
# %bb.75:                               #   in Loop: Header=BB11_74 Depth=3
	movzwl	%ax, %ebp
	cmpl	%ebp, 52(%rsp)                  # 4-byte Folded Reload
	je	.LBB11_73
# %bb.76:                               #   in Loop: Header=BB11_74 Depth=3
	movl	%ebp, %edi
	movq	8(%rsp), %rdx                   # 8-byte Reload
	imull	%edx, %edi
	movq	32(%rsp), %rax                  # 8-byte Reload
	addq	%rax, %rdi
	leal	1(%rbp), %r15d
	imull	%edx, %r15d
	addq	%rax, %r15
	movq	%r15, %rsi
	callq	bcmp
	cmpl	%ebp, 16(%rsp)                  # 4-byte Folded Reload
	jle	.LBB11_73
# %bb.77:                               #   in Loop: Header=BB11_74 Depth=3
	testl	%eax, %eax
	jne	.LBB11_73
# %bb.78:                               #   in Loop: Header=BB11_74 Depth=3
	addl	$2, %ebp
	movq	8(%rsp), %rdx                   # 8-byte Reload
	imull	%edx, %ebp
	addq	32(%rsp), %rbp                  # 8-byte Folded Reload
	movq	%r15, %rdi
	movq	%rbp, %rsi
	callq	bcmp
	testl	%eax, %eax
	jne	.LBB11_73
	jmp	.LBB11_82
.LBB11_79:                              #   in Loop: Header=BB11_55 Depth=2
	cmpl	%r13d, 16(%rsp)                 # 4-byte Folded Reload
	jle	.LBB11_57
# %bb.80:                               #   in Loop: Header=BB11_55 Depth=2
	leal	2(%r13), %esi
	movq	8(%rsp), %rdx                   # 8-byte Reload
	imull	%edx, %esi
	addq	32(%rsp), %rsi                  # 8-byte Folded Reload
	movq	%r15, %rdi
	callq	bcmp
	testl	%eax, %eax
	jne	.LBB11_57
	jmp	.LBB11_65
.LBB11_81:                              #   in Loop: Header=BB11_55 Depth=2
	movb	$-128, %r12b
.LBB11_82:                              #   in Loop: Header=BB11_55 Depth=2
	leal	-1(%r12), %eax
	xorl	%r15d, %r15d
.LBB11_83:                              #   in Loop: Header=BB11_55 Depth=2
	movb	%al, 7(%rsp)
	movl	$1, %esi
	movl	$1, %edx
	leaq	7(%rsp), %rdi
	movq	%r14, %rcx
	callq	fwrite
	cmpq	$1, %rax
	jne	.LBB11_44
# %bb.84:                               #   in Loop: Header=BB11_55 Depth=2
	movzbl	%r12b, %ebp
	imull	8(%rsp), %r13d                  # 4-byte Folded Reload
	addq	32(%rsp), %r13                  # 8-byte Folded Reload
	testb	%r15b, %r15b
	je	.LBB11_86
# %bb.85:                               #   in Loop: Header=BB11_55 Depth=2
	movl	$1, %edx
	movq	%r13, %rdi
	movq	8(%rsp), %rsi                   # 8-byte Reload
	jmp	.LBB11_87
.LBB11_86:                              #   in Loop: Header=BB11_55 Depth=2
	movl	%ebp, %esi
	imull	8(%rsp), %esi                   # 4-byte Folded Reload
	movl	$1, %edx
	movq	%r13, %rdi
.LBB11_87:                              #   in Loop: Header=BB11_55 Depth=2
	movq	%r14, %rcx
	callq	fwrite
	cmpq	$1, %rax
	jne	.LBB11_44
# %bb.88:                               #   in Loop: Header=BB11_55 Depth=2
	movl	48(%rsp), %eax                  # 4-byte Reload
	addl	%ebp, %eax
	movl	%eax, 48(%rsp)                  # 4-byte Spill
	movzwl	%ax, %r13d
	movzwl	14(%rbx), %eax
	cmpw	%ax, %r13w
	jb	.LBB11_55
# %bb.89:                               #   in Loop: Header=BB11_53 Depth=1
	movzwl	16(%rbx), %ecx
	jmp	.LBB11_91
.LBB11_90:                              #   in Loop: Header=BB11_53 Depth=1
	xorl	%eax, %eax
.LBB11_91:                              #   in Loop: Header=BB11_53 Depth=1
	movq	56(%rsp), %rdi                  # 8-byte Reload
	addq	$1, %rdi
	movzwl	%cx, %edx
	movq	%rdi, %rsi
	movq	%rdi, 56(%rsp)                  # 8-byte Spill
	cmpq	%rdx, %rdi
	jb	.LBB11_53
.LBB11_92:
	movl	$tga_id, %edi
	movl	$26, %esi
	movl	$1, %edx
	movq	%r14, %rcx
	callq	fwrite
	xorl	%ecx, %ecx
	cmpq	$1, %rax
	setne	%cl
	leal	(%rcx,%rcx,2), %eax
	jmp	.LBB11_45
.LBB11_44:
	movl	$3, %eax
.LBB11_45:
	addq	$88, %rsp
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
.Lfunc_end11:
	.size	tga_write_to_FILE, .Lfunc_end11-tga_write_to_FILE
	.cfi_endproc
                                        # -- End function
	.globl	tga_write_mono                  # -- Begin function tga_write_mono
	.p2align	4, 0x90
	.type	tga_write_mono,@function
tga_write_mono:                         # @tga_write_mono
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	subq	$56, %rsp
	.cfi_def_cfa_offset 80
	.cfi_offset %rbx, -24
	.cfi_offset %rbp, -16
	movw	$0, 8(%rsp)
	movl	$0, 12(%rsp)
	movb	$0, 16(%rsp)
	movl	$0, 18(%rsp)
	movw	%dx, 22(%rsp)
	movw	%cx, 24(%rsp)
	movw	$8200, 26(%rsp)                 # imm = 0x2008
	xorps	%xmm0, %xmm0
	movups	%xmm0, 32(%rsp)
	movq	%rsi, 48(%rsp)
	movb	$3, 10(%rsp)
	movl	$.L.str.20, %esi
	callq	fopen
	testq	%rax, %rax
	je	.LBB12_1
# %bb.2:
	movq	%rax, %rbx
	leaq	8(%rsp), %rsi
	movq	%rax, %rdi
	callq	tga_write_to_FILE
	movl	%eax, %ebp
	movq	%rbx, %rdi
	callq	fclose
	jmp	.LBB12_3
.LBB12_1:
	movl	$1, %ebp
.LBB12_3:
	movl	%ebp, %eax
	addq	$56, %rsp
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end12:
	.size	tga_write_mono, .Lfunc_end12-tga_write_mono
	.cfi_endproc
                                        # -- End function
	.globl	tga_write_mono_rle              # -- Begin function tga_write_mono_rle
	.p2align	4, 0x90
	.type	tga_write_mono_rle,@function
tga_write_mono_rle:                     # @tga_write_mono_rle
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	subq	$56, %rsp
	.cfi_def_cfa_offset 80
	.cfi_offset %rbx, -24
	.cfi_offset %rbp, -16
	movw	$0, 8(%rsp)
	movl	$0, 12(%rsp)
	movb	$0, 16(%rsp)
	movl	$0, 18(%rsp)
	movw	%dx, 22(%rsp)
	movw	%cx, 24(%rsp)
	movw	$8200, 26(%rsp)                 # imm = 0x2008
	xorps	%xmm0, %xmm0
	movups	%xmm0, 32(%rsp)
	movq	%rsi, 48(%rsp)
	movb	$11, 10(%rsp)
	movl	$.L.str.20, %esi
	callq	fopen
	testq	%rax, %rax
	je	.LBB13_1
# %bb.2:
	movq	%rax, %rbx
	leaq	8(%rsp), %rsi
	movq	%rax, %rdi
	callq	tga_write_to_FILE
	movl	%eax, %ebp
	movq	%rbx, %rdi
	callq	fclose
	jmp	.LBB13_3
.LBB13_1:
	movl	$1, %ebp
.LBB13_3:
	movl	%ebp, %eax
	addq	$56, %rsp
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end13:
	.size	tga_write_mono_rle, .Lfunc_end13-tga_write_mono_rle
	.cfi_endproc
                                        # -- End function
	.globl	tga_write_bgr                   # -- Begin function tga_write_bgr
	.p2align	4, 0x90
	.type	tga_write_bgr,@function
tga_write_bgr:                          # @tga_write_bgr
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	subq	$56, %rsp
	.cfi_def_cfa_offset 80
	.cfi_offset %rbx, -24
	.cfi_offset %rbp, -16
	movw	$0, 8(%rsp)
	movl	$0, 12(%rsp)
	movb	$0, 16(%rsp)
	movl	$0, 18(%rsp)
	movw	%dx, 22(%rsp)
	movw	%cx, 24(%rsp)
	movb	%r8b, 26(%rsp)
	movb	$32, 27(%rsp)
	xorps	%xmm0, %xmm0
	movups	%xmm0, 32(%rsp)
	movq	%rsi, 48(%rsp)
	movb	$2, 10(%rsp)
	movl	$.L.str.20, %esi
	callq	fopen
	testq	%rax, %rax
	je	.LBB14_1
# %bb.2:
	movq	%rax, %rbx
	leaq	8(%rsp), %rsi
	movq	%rax, %rdi
	callq	tga_write_to_FILE
	movl	%eax, %ebp
	movq	%rbx, %rdi
	callq	fclose
	jmp	.LBB14_3
.LBB14_1:
	movl	$1, %ebp
.LBB14_3:
	movl	%ebp, %eax
	addq	$56, %rsp
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end14:
	.size	tga_write_bgr, .Lfunc_end14-tga_write_bgr
	.cfi_endproc
                                        # -- End function
	.globl	tga_write_bgr_rle               # -- Begin function tga_write_bgr_rle
	.p2align	4, 0x90
	.type	tga_write_bgr_rle,@function
tga_write_bgr_rle:                      # @tga_write_bgr_rle
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	subq	$56, %rsp
	.cfi_def_cfa_offset 80
	.cfi_offset %rbx, -24
	.cfi_offset %rbp, -16
	movw	$0, 8(%rsp)
	movl	$0, 12(%rsp)
	movb	$0, 16(%rsp)
	movl	$0, 18(%rsp)
	movw	%dx, 22(%rsp)
	movw	%cx, 24(%rsp)
	movb	%r8b, 26(%rsp)
	movb	$32, 27(%rsp)
	xorps	%xmm0, %xmm0
	movups	%xmm0, 32(%rsp)
	movq	%rsi, 48(%rsp)
	movb	$10, 10(%rsp)
	movl	$.L.str.20, %esi
	callq	fopen
	testq	%rax, %rax
	je	.LBB15_1
# %bb.2:
	movq	%rax, %rbx
	leaq	8(%rsp), %rsi
	movq	%rax, %rdi
	callq	tga_write_to_FILE
	movl	%eax, %ebp
	movq	%rbx, %rdi
	callq	fclose
	jmp	.LBB15_3
.LBB15_1:
	movl	$1, %ebp
.LBB15_3:
	movl	%ebp, %eax
	addq	$56, %rsp
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end15:
	.size	tga_write_bgr_rle, .Lfunc_end15-tga_write_bgr_rle
	.cfi_endproc
                                        # -- End function
	.globl	tga_write_rgb                   # -- Begin function tga_write_rgb
	.p2align	4, 0x90
	.type	tga_write_rgb,@function
tga_write_rgb:                          # @tga_write_rgb
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	subq	$56, %rsp
	.cfi_def_cfa_offset 80
	.cfi_offset %rbx, -24
	.cfi_offset %rbp, -16
	movq	%rdi, %rbx
	movw	$0, 8(%rsp)
	movl	$0, 12(%rsp)
	movb	$0, 16(%rsp)
	movl	$0, 18(%rsp)
	movw	%dx, 22(%rsp)
	movw	%cx, 24(%rsp)
	movb	%r8b, 26(%rsp)
	movb	$32, 27(%rsp)
	xorps	%xmm0, %xmm0
	movups	%xmm0, 32(%rsp)
	movq	%rsi, 48(%rsp)
	movb	$2, 10(%rsp)
	leaq	8(%rsp), %rdi
	callq	tga_swap_red_blue
	movl	$.L.str.20, %esi
	movq	%rbx, %rdi
	callq	fopen
	testq	%rax, %rax
	je	.LBB16_1
# %bb.2:
	movq	%rax, %rbx
	leaq	8(%rsp), %rsi
	movq	%rax, %rdi
	callq	tga_write_to_FILE
	movl	%eax, %ebp
	movq	%rbx, %rdi
	callq	fclose
	jmp	.LBB16_3
.LBB16_1:
	movl	$1, %ebp
.LBB16_3:
	movl	%ebp, %eax
	addq	$56, %rsp
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end16:
	.size	tga_write_rgb, .Lfunc_end16-tga_write_rgb
	.cfi_endproc
                                        # -- End function
	.globl	tga_swap_red_blue               # -- Begin function tga_swap_red_blue
	.p2align	4, 0x90
	.type	tga_swap_red_blue,@function
tga_swap_red_blue:                      # @tga_swap_red_blue
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	pushq	%r14
	.cfi_def_cfa_offset 24
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset %rbx, -32
	.cfi_offset %r14, -24
	.cfi_offset %rbp, -16
	movzbl	18(%rdi), %ebx
	movl	$12, %eax
	cmpq	$32, %rbx
	ja	.LBB17_19
# %bb.1:
	movabsq	$4311810048, %rcx               # imm = 0x101010000
	btq	%rbx, %rcx
	jae	.LBB17_19
# %bb.2:
	movq	%rbx, %r8
	shrq	$3, %r8
	movzwl	14(%rdi), %eax
	movzwl	16(%rdi), %ecx
	imulq	%rax, %rcx
	addq	$-1, %rcx
	movq	%r8, %r9
	shlq	$32, %r9
	imulq	%r9, %rcx
	xorl	%eax, %eax
	testq	%rcx, %rcx
	jle	.LBB17_19
# %bb.3:
	movq	40(%rdi), %rsi
                                        # implicit-def: $r10b
                                        # implicit-def: $r11b
                                        # implicit-def: $cl
                                        # implicit-def: $r14b
	.p2align	4, 0x90
.LBB17_4:                               # =>This Inner Loop Header: Depth=1
	movzbl	%bl, %edx
	addl	$-8, %edx
	roll	$29, %edx
	cmpl	$3, %edx
	ja	.LBB17_10
# %bb.5:                                #   in Loop: Header=BB17_4 Depth=1
	jmpq	*.LJTI17_0(,%rdx,8)
.LBB17_9:                               #   in Loop: Header=BB17_4 Depth=1
	movzbl	(%rsi), %r14d
	xorl	%r10d, %r10d
	movl	%r14d, %ecx
	movl	%r14d, %r11d
	cmpb	$16, %bl
	jne	.LBB17_11
	jmp	.LBB17_15
	.p2align	4, 0x90
.LBB17_8:                               #   in Loop: Header=BB17_4 Depth=1
	movzbl	1(%rsi), %ecx
	leal	(%rcx,%rcx), %r14d
                                        # kill: def $ecx killed $ecx killed $rcx
	shll	$8, %ecx
	movswl	%cx, %r10d
	movzbl	(%rsi), %edx
	orl	%edx, %ecx
	leal	(,%rdx,8), %r11d
	shrl	$2, %ecx
	andb	$-8, %cl
	andb	$-8, %r14b
	shrl	$15, %r10d
	cmpb	$16, %bl
	jne	.LBB17_11
	jmp	.LBB17_15
	.p2align	4, 0x90
.LBB17_7:                               #   in Loop: Header=BB17_4 Depth=1
	movzbl	(%rsi), %r11d
	movzbl	1(%rsi), %ecx
	movzbl	2(%rsi), %r14d
	xorl	%r10d, %r10d
	.p2align	4, 0x90
.LBB17_10:                              #   in Loop: Header=BB17_4 Depth=1
	cmpb	$16, %bl
	je	.LBB17_15
.LBB17_11:                              #   in Loop: Header=BB17_4 Depth=1
	cmpb	$24, %bl
	je	.LBB17_14
# %bb.12:                               #   in Loop: Header=BB17_4 Depth=1
	cmpb	$32, %bl
	jne	.LBB17_17
# %bb.13:                               #   in Loop: Header=BB17_4 Depth=1
	movb	%r14b, (%rsi)
	movb	%cl, 1(%rsi)
	movb	%r11b, 2(%rsi)
	movl	$3, %edx
	movl	%r10d, %ebx
	jmp	.LBB17_16
	.p2align	4, 0x90
.LBB17_14:                              #   in Loop: Header=BB17_4 Depth=1
	movb	%r14b, (%rsi)
	movb	%cl, 1(%rsi)
	movl	$2, %edx
	movl	%r11d, %ebx
	jmp	.LBB17_16
	.p2align	4, 0x90
.LBB17_6:                               #   in Loop: Header=BB17_4 Depth=1
	movzbl	(%rsi), %r11d
	movzbl	1(%rsi), %ecx
	movzbl	2(%rsi), %r14d
	movzbl	3(%rsi), %r10d
	cmpb	$16, %bl
	jne	.LBB17_11
	.p2align	4, 0x90
.LBB17_15:                              #   in Loop: Header=BB17_4 Depth=1
	movl	%r14d, %edx
	shrb	$3, %dl
	movl	%ecx, %ebx
	shrb	$3, %bl
	movzbl	%bl, %ebx
	shll	$5, %ebx
	movzbl	%dl, %edx
	orl	%ebx, %edx
	movl	%r11d, %ebx
	shrb	$3, %bl
	movzbl	%bl, %ebp
	shll	$10, %ebp
	orl	%edx, %ebp
	movl	%ebp, %ebx
	orl	$-32768, %ebx                   # imm = 0x8000
	testb	%r10b, %r10b
	cmovnsl	%ebp, %ebx
	movb	%bl, (%rsi)
	shrl	$8, %ebx
	movl	$1, %edx
.LBB17_16:                              #   in Loop: Header=BB17_4 Depth=1
	movb	%bl, (%rsi,%rdx)
.LBB17_17:                              #   in Loop: Header=BB17_4 Depth=1
	addq	%r8, %rsi
	movzwl	14(%rdi), %edx
	movzwl	16(%rdi), %ebx
	imulq	%rdx, %rbx
	addq	$-1, %rbx
	imulq	%r9, %rbx
	sarq	$32, %rbx
	addq	40(%rdi), %rbx
	cmpq	%rbx, %rsi
	jae	.LBB17_19
# %bb.18:                               #   in Loop: Header=BB17_4 Depth=1
	movzbl	18(%rdi), %ebx
	jmp	.LBB17_4
.LBB17_19:
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end17:
	.size	tga_swap_red_blue, .Lfunc_end17-tga_swap_red_blue
	.cfi_endproc
	.section	.rodata,"a",@progbits
	.p2align	3
.LJTI17_0:
	.quad	.LBB17_9
	.quad	.LBB17_8
	.quad	.LBB17_7
	.quad	.LBB17_6
                                        # -- End function
	.text
	.globl	tga_write_rgb_rle               # -- Begin function tga_write_rgb_rle
	.p2align	4, 0x90
	.type	tga_write_rgb_rle,@function
tga_write_rgb_rle:                      # @tga_write_rgb_rle
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	subq	$56, %rsp
	.cfi_def_cfa_offset 80
	.cfi_offset %rbx, -24
	.cfi_offset %rbp, -16
	movq	%rdi, %rbx
	movw	$0, 8(%rsp)
	movl	$0, 12(%rsp)
	movb	$0, 16(%rsp)
	movl	$0, 18(%rsp)
	movw	%dx, 22(%rsp)
	movw	%cx, 24(%rsp)
	movb	%r8b, 26(%rsp)
	movb	$32, 27(%rsp)
	xorps	%xmm0, %xmm0
	movups	%xmm0, 32(%rsp)
	movq	%rsi, 48(%rsp)
	movb	$10, 10(%rsp)
	leaq	8(%rsp), %rdi
	callq	tga_swap_red_blue
	movl	$.L.str.20, %esi
	movq	%rbx, %rdi
	callq	fopen
	testq	%rax, %rax
	je	.LBB18_1
# %bb.2:
	movq	%rax, %rbx
	leaq	8(%rsp), %rsi
	movq	%rax, %rdi
	callq	tga_write_to_FILE
	movl	%eax, %ebp
	movq	%rbx, %rdi
	callq	fclose
	jmp	.LBB18_3
.LBB18_1:
	movl	$1, %ebp
.LBB18_3:
	movl	%ebp, %eax
	addq	$56, %rsp
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end18:
	.size	tga_write_rgb_rle, .Lfunc_end18-tga_write_rgb_rle
	.cfi_endproc
                                        # -- End function
	.globl	tga_flip_horiz                  # -- Begin function tga_flip_horiz
	.p2align	4, 0x90
	.type	tga_flip_horiz,@function
tga_flip_horiz:                         # @tga_flip_horiz
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
	subq	$40, %rsp
	.cfi_def_cfa_offset 96
	.cfi_offset %rbx, -56
	.cfi_offset %r12, -48
	.cfi_offset %r13, -40
	.cfi_offset %r14, -32
	.cfi_offset %r15, -24
	.cfi_offset %rbp, -16
	movzbl	18(%rdi), %r12d
	leal	-8(%r12), %ecx
	rolb	$5, %cl
	movl	$12, %eax
	cmpb	$3, %cl
	ja	.LBB19_9
# %bb.1:
	movzwl	16(%rdi), %ebp
	testw	%bp, %bp
	je	.LBB19_8
# %bb.2:
	shrq	$3, %r12
	movq	%r12, %r14
	negq	%r14
	xorl	%esi, %esi
	leaq	36(%rsp), %r15
	movq	%rdi, 8(%rsp)                   # 8-byte Spill
	jmp	.LBB19_3
	.p2align	4, 0x90
.LBB19_7:                               #   in Loop: Header=BB19_3 Depth=1
	addl	$1, %esi
	movzwl	%bp, %ecx
	cmpl	%ecx, %esi
	jae	.LBB19_8
.LBB19_3:                               # =>This Loop Header: Depth=1
                                        #     Child Loop BB19_5 Depth 2
	movzwl	14(%rdi), %ecx
	movq	%rcx, %rax
	addq	$-1, %rcx
	imulq	%r12, %rcx
	testq	%rcx, %rcx
	jle	.LBB19_7
# %bb.4:                                #   in Loop: Header=BB19_3 Depth=1
	movq	40(%rdi), %rbp
	movl	%esi, %ebx
	imulq	%rax, %rbx
	imulq	%r12, %rbx
	addq	%rbp, %rbx
	addq	%rcx, %rbx
	movl	%esi, 4(%rsp)                   # 4-byte Spill
	imull	%esi, %eax
	leaq	1(%rax), %rcx
	imulq	%r12, %rcx
	movq	%rcx, 16(%rsp)                  # 8-byte Spill
	imulq	%r12, %rax
	movq	%rax, 24(%rsp)                  # 8-byte Spill
	.p2align	4, 0x90
.LBB19_5:                               #   Parent Loop BB19_3 Depth=1
                                        # =>  This Inner Loop Header: Depth=2
	movq	24(%rsp), %rax                  # 8-byte Reload
	leaq	(%rax,%rbp), %r13
	movq	%r15, %rdi
	movq	%r13, %rsi
	movq	%r12, %rdx
	callq	memcpy
	movq	%r13, %rdi
	movq	%rbx, %rsi
	movq	%r12, %rdx
	callq	memcpy
	movq	%rbx, %rdi
	movq	%r15, %rsi
	movq	%r12, %rdx
	callq	memcpy
	addq	%r14, %rbx
	movq	16(%rsp), %rax                  # 8-byte Reload
	addq	%rbp, %rax
	addq	%r12, %rbp
	cmpq	%rbx, %rax
	jb	.LBB19_5
# %bb.6:                                #   in Loop: Header=BB19_3 Depth=1
	movq	8(%rsp), %rdi                   # 8-byte Reload
	movzwl	16(%rdi), %ebp
	movl	4(%rsp), %esi                   # 4-byte Reload
	jmp	.LBB19_7
.LBB19_8:
	xorb	$16, 19(%rdi)
	xorl	%eax, %eax
.LBB19_9:
	addq	$40, %rsp
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
.Lfunc_end19:
	.size	tga_flip_horiz, .Lfunc_end19-tga_flip_horiz
	.cfi_endproc
                                        # -- End function
	.globl	tga_flip_vert                   # -- Begin function tga_flip_vert
	.p2align	4, 0x90
	.type	tga_flip_vert,@function
tga_flip_vert:                          # @tga_flip_vert
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
	subq	$24, %rsp
	.cfi_def_cfa_offset 80
	.cfi_offset %rbx, -56
	.cfi_offset %r12, -48
	.cfi_offset %r13, -40
	.cfi_offset %r14, -32
	.cfi_offset %r15, -24
	.cfi_offset %rbp, -16
	movzbl	18(%rdi), %r12d
	leal	-8(%r12), %ecx
	rolb	$5, %cl
	movl	$12, %eax
	cmpb	$3, %cl
	ja	.LBB20_9
# %bb.1:
	movzwl	14(%rdi), %r14d
	testq	%r14, %r14
	je	.LBB20_8
# %bb.2:
	shrq	$3, %r12
	movl	%r14d, %eax
	imulq	%r12, %r14
	movq	%r14, %rbp
	negq	%rbp
	xorl	%ebx, %ebx
	leaq	20(%rsp), %r15
	movq	%rdi, (%rsp)                    # 8-byte Spill
	jmp	.LBB20_3
	.p2align	4, 0x90
.LBB20_7:                               #   in Loop: Header=BB20_3 Depth=1
	addq	$1, %rbx
	movzwl	%ax, %ecx
	cmpq	%rcx, %rbx
	jae	.LBB20_8
.LBB20_3:                               # =>This Loop Header: Depth=1
                                        #     Child Loop BB20_5 Depth 2
	movzwl	16(%rdi), %r13d
	addq	$-1, %r13
	imulq	%r14, %r13
	testq	%r13, %r13
	jle	.LBB20_7
# %bb.4:                                #   in Loop: Header=BB20_3 Depth=1
	movq	%rbx, 8(%rsp)                   # 8-byte Spill
	imulq	%r12, %rbx
	addq	40(%rdi), %rbx
	addq	%rbx, %r13
	.p2align	4, 0x90
.LBB20_5:                               #   Parent Loop BB20_3 Depth=1
                                        # =>  This Inner Loop Header: Depth=2
	movq	%r15, %rdi
	movq	%rbx, %rsi
	movq	%r12, %rdx
	callq	memcpy
	movq	%rbx, %rdi
	movq	%r13, %rsi
	movq	%r12, %rdx
	callq	memcpy
	movq	%r13, %rdi
	movq	%r15, %rsi
	movq	%r12, %rdx
	callq	memcpy
	addq	%r14, %rbx
	addq	%rbp, %r13
	cmpq	%r13, %rbx
	jb	.LBB20_5
# %bb.6:                                #   in Loop: Header=BB20_3 Depth=1
	movq	(%rsp), %rdi                    # 8-byte Reload
	movzwl	14(%rdi), %eax
	movq	8(%rsp), %rbx                   # 8-byte Reload
	jmp	.LBB20_7
.LBB20_8:
	xorb	$32, 19(%rdi)
	xorl	%eax, %eax
.LBB20_9:
	addq	$24, %rsp
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
.Lfunc_end20:
	.size	tga_flip_vert, .Lfunc_end20-tga_flip_vert
	.cfi_endproc
                                        # -- End function
	.globl	tga_color_unmap                 # -- Begin function tga_color_unmap
	.p2align	4, 0x90
	.type	tga_color_unmap,@function
tga_color_unmap:                        # @tga_color_unmap
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
	movb	2(%rdi), %al
	andb	$-9, %al
	movl	$14, %r15d
	cmpb	$1, %al
	jne	.LBB21_11
# %bb.1:
	movq	%rdi, %r13
	movl	$12, %r15d
	cmpb	$8, 18(%rdi)
	jne	.LBB21_11
# %bb.2:
	movzbl	8(%r13), %r14d
	leal	-8(%r14), %eax
	rolb	$5, %al
	movl	$10, %r15d
	cmpb	$3, %al
	ja	.LBB21_11
# %bb.3:
	shrq	$3, %r14
	movq	40(%r13), %rdi
	movzwl	14(%r13), %esi
	movzwl	16(%r13), %eax
	imull	%r14d, %esi
	imull	%eax, %esi
	callq	realloc
	testq	%rax, %rax
	je	.LBB21_4
# %bb.5:
	movq	%rax, %rdi
	movq	%rax, 40(%r13)
	movzwl	14(%r13), %ecx
	movzwl	16(%r13), %eax
	imull	%ecx, %eax
	testl	%eax, %eax
	je	.LBB21_10
# %bb.6:
	movslq	%eax, %rbp
	movzbl	-1(%rdi,%rbp), %esi
	movzwl	4(%r13), %eax
	movzwl	6(%r13), %ecx
	addl	%eax, %ecx
	movl	$16, %r15d
	cmpl	%esi, %ecx
	jbe	.LBB21_11
# %bb.7:
	movzbl	%r14b, %r12d
	leaq	-1(%rbp), %rbx
	imulq	%r12, %rbx
	.p2align	4, 0x90
.LBB21_8:                               # =>This Inner Loop Header: Depth=1
	imull	%r14d, %esi
	addq	32(%r13), %rsi
	addq	%rbx, %rdi
	movq	%r14, %rdx
	callq	memcpy
	cmpq	$2, %rbp
	jl	.LBB21_10
# %bb.9:                                #   in Loop: Header=BB21_8 Depth=1
	movq	40(%r13), %rdi
	movzbl	-2(%rdi,%rbp), %esi
	movzwl	4(%r13), %eax
	movzwl	6(%r13), %ecx
	addl	%eax, %ecx
	addq	$-1, %rbp
	subq	%r12, %rbx
	cmpl	%esi, %ecx
	ja	.LBB21_8
	jmp	.LBB21_11
.LBB21_4:
	movl	$13, %r15d
	jmp	.LBB21_11
.LBB21_10:
	movb	$2, 2(%r13)
	movb	8(%r13), %al
	movb	%al, 18(%r13)
	movq	32(%r13), %rdi
	callq	free
	movq	$0, 32(%r13)
	movb	$0, 1(%r13)
	movl	$0, 4(%r13)
	movb	$0, 8(%r13)
	xorl	%r15d, %r15d
.LBB21_11:
	movl	%r15d, %eax
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
.Lfunc_end21:
	.size	tga_color_unmap, .Lfunc_end21-tga_color_unmap
	.cfi_endproc
                                        # -- End function
	.globl	tga_find_pixel                  # -- Begin function tga_find_pixel
	.p2align	4, 0x90
	.type	tga_find_pixel,@function
tga_find_pixel:                         # @tga_find_pixel
	.cfi_startproc
# %bb.0:
	movzwl	14(%rdi), %r9d
	cmpw	%si, %r9w
	jbe	.LBB22_4
# %bb.1:
	movzwl	16(%rdi), %ecx
	cmpw	%dx, %cx
	jbe	.LBB22_4
# %bb.2:
	movb	19(%rdi), %r8b
	movl	%edx, %eax
	notl	%eax
	addl	%eax, %ecx
	testb	$32, %r8b
	cmovnel	%edx, %ecx
	movl	%esi, %eax
	notl	%eax
	addl	%r9d, %eax
	testb	$16, %r8b
	cmovel	%esi, %eax
	movzwl	%cx, %ecx
	imull	%r9d, %ecx
	movzwl	%ax, %edx
	addl	%ecx, %edx
	movzbl	18(%rdi), %eax
	imull	%edx, %eax
	shrl	$3, %eax
	addq	40(%rdi), %rax
	retq
.LBB22_4:
	xorl	%eax, %eax
	retq
.Lfunc_end22:
	.size	tga_find_pixel, .Lfunc_end22-tga_find_pixel
	.cfi_endproc
                                        # -- End function
	.globl	tga_unpack_pixel                # -- Begin function tga_unpack_pixel
	.p2align	4, 0x90
	.type	tga_unpack_pixel,@function
tga_unpack_pixel:                       # @tga_unpack_pixel
	.cfi_startproc
# %bb.0:
                                        # kill: def $esi killed $esi def $rsi
	addl	$-8, %esi
	roll	$29, %esi
	cmpl	$3, %esi
	ja	.LBB23_8
# %bb.1:
	jmpq	*.LJTI23_0(,%rsi,8)
.LBB23_2:
	testq	%rdx, %rdx
	je	.LBB23_4
# %bb.3:
	movb	(%rdi), %al
	movb	%al, (%rdx)
.LBB23_4:
	testq	%rcx, %rcx
	je	.LBB23_6
# %bb.5:
	movb	(%rdi), %al
	movb	%al, (%rcx)
.LBB23_6:
	testq	%r8, %r8
	je	.LBB23_24
# %bb.7:
	movb	(%rdi), %al
	jmp	.LBB23_23
.LBB23_8:
	movl	$12, %eax
	retq
.LBB23_9:
	movzbl	1(%rdi), %eax
	movzbl	(%rdi), %esi
	testq	%rdx, %rdx
	je	.LBB23_11
# %bb.10:
	leal	(,%rsi,8), %edi
	movb	%dil, (%rdx)
.LBB23_11:
	movl	%eax, %edx
	shll	$8, %edx
	testq	%rcx, %rcx
	je	.LBB23_13
# %bb.12:
	orl	%edx, %esi
	shrl	$2, %esi
	andb	$-8, %sil
	movb	%sil, (%rcx)
.LBB23_13:
	testq	%r8, %r8
	je	.LBB23_15
# %bb.14:
	addb	%al, %al
	andb	$-8, %al
	movb	%al, (%r8)
.LBB23_15:
	xorl	%eax, %eax
	testq	%r9, %r9
	je	.LBB23_34
# %bb.16:
	movswl	%dx, %ecx
	shrl	$15, %ecx
	movb	%cl, (%r9)
	retq
.LBB23_17:
	testq	%rdx, %rdx
	je	.LBB23_19
# %bb.18:
	movb	(%rdi), %al
	movb	%al, (%rdx)
.LBB23_19:
	testq	%rcx, %rcx
	je	.LBB23_21
# %bb.20:
	movb	1(%rdi), %al
	movb	%al, (%rcx)
.LBB23_21:
	testq	%r8, %r8
	je	.LBB23_24
# %bb.22:
	movb	2(%rdi), %al
.LBB23_23:
	movb	%al, (%r8)
.LBB23_24:
	xorl	%eax, %eax
	testq	%r9, %r9
	je	.LBB23_34
# %bb.25:
	movb	$0, (%r9)
.LBB23_34:
	retq
.LBB23_26:
	testq	%rdx, %rdx
	je	.LBB23_28
# %bb.27:
	movb	(%rdi), %al
	movb	%al, (%rdx)
.LBB23_28:
	testq	%rcx, %rcx
	je	.LBB23_30
# %bb.29:
	movb	1(%rdi), %al
	movb	%al, (%rcx)
.LBB23_30:
	testq	%r8, %r8
	je	.LBB23_32
# %bb.31:
	movb	2(%rdi), %al
	movb	%al, (%r8)
.LBB23_32:
	xorl	%eax, %eax
	testq	%r9, %r9
	je	.LBB23_34
# %bb.33:
	movb	3(%rdi), %cl
	movb	%cl, (%r9)
	retq
.Lfunc_end23:
	.size	tga_unpack_pixel, .Lfunc_end23-tga_unpack_pixel
	.cfi_endproc
	.section	.rodata,"a",@progbits
	.p2align	3
.LJTI23_0:
	.quad	.LBB23_2
	.quad	.LBB23_9
	.quad	.LBB23_17
	.quad	.LBB23_26
                                        # -- End function
	.text
	.globl	tga_pack_pixel                  # -- Begin function tga_pack_pixel
	.p2align	4, 0x90
	.type	tga_pack_pixel,@function
tga_pack_pixel:                         # @tga_pack_pixel
	.cfi_startproc
# %bb.0:
	cmpb	$16, %sil
	je	.LBB24_5
# %bb.1:
	cmpb	$24, %sil
	je	.LBB24_4
# %bb.2:
	movl	$12, %eax
	cmpb	$32, %sil
	jne	.LBB24_7
# %bb.3:
	movb	%dl, (%rdi)
	movb	%cl, 1(%rdi)
	movb	%r8b, 2(%rdi)
	movl	$3, %eax
	jmp	.LBB24_6
.LBB24_5:
	shrb	$3, %dl
	shrb	$3, %cl
	movzbl	%cl, %eax
	shll	$5, %eax
	movzbl	%dl, %ecx
	orl	%eax, %ecx
	shrb	$3, %r8b
	movzbl	%r8b, %eax
	shll	$10, %eax
	orl	%ecx, %eax
	movl	%eax, %ecx
	orl	$-32768, %ecx                   # imm = 0x8000
	testb	%r9b, %r9b
	cmovnsl	%eax, %ecx
	movb	%cl, (%rdi)
	shrl	$8, %ecx
	movl	$1, %eax
	movl	%ecx, %r9d
	jmp	.LBB24_6
.LBB24_4:
	movb	%dl, (%rdi)
	movb	%cl, 1(%rdi)
	movl	$2, %eax
	movl	%r8d, %r9d
.LBB24_6:
	movb	%r9b, (%rdi,%rax)
	xorl	%eax, %eax
.LBB24_7:
	retq
.Lfunc_end24:
	.size	tga_pack_pixel, .Lfunc_end24-tga_pack_pixel
	.cfi_endproc
                                        # -- End function
	.globl	tga_desaturate                  # -- Begin function tga_desaturate
	.p2align	4, 0x90
	.type	tga_desaturate,@function
tga_desaturate:                         # @tga_desaturate
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
	movb	2(%rdi), %al
	andb	$-9, %al
	cmpb	$3, %al
	je	.LBB25_1
# %bb.2:
	movl	%ecx, %r15d
	movl	%edx, %r12d
	movl	%esi, %r13d
	movq	%rdi, %rbx
	movzbl	18(%rdi), %r14d
	movl	%r14d, %edx
	cmpb	$1, %al
	jne	.LBB25_5
# %bb.3:
	movl	%r8d, %ebp
	movq	%rbx, %rdi
	callq	tga_color_unmap
	testl	%eax, %eax
	jne	.LBB25_20
# %bb.4:
	movb	18(%rbx), %dl
	movl	%ebp, %r8d
.LBB25_5:
	movl	$12, %eax
	cmpb	$32, %dl
	ja	.LBB25_20
# %bb.6:
	movzbl	%dl, %ecx
	movabsq	$4311810048, %rsi               # imm = 0x101010000
	btq	%rcx, %rsi
	jae	.LBB25_20
# %bb.7:
	movl	%r8d, %r10d
	shrq	$3, %r14
	movq	40(%rbx), %r8
	movzwl	14(%rbx), %ecx
	movzwl	16(%rbx), %eax
	imull	%ecx, %eax
	movl	%eax, %ecx
	imull	%r14d, %ecx
	testl	%ecx, %ecx
	je	.LBB25_8
# %bb.11:
	movq	%r8, %rsi
                                        # implicit-def: $cl
                                        # implicit-def: $bpl
                                        # implicit-def: $r9b
	.p2align	4, 0x90
.LBB25_12:                              # =>This Inner Loop Header: Depth=1
	movzbl	%dl, %eax
	addl	$-8, %eax
	roll	$29, %eax
	cmpl	$3, %eax
	ja	.LBB25_17
# %bb.13:                               #   in Loop: Header=BB25_12 Depth=1
	jmpq	*.LJTI25_0(,%rax,8)
.LBB25_14:                              #   in Loop: Header=BB25_12 Depth=1
	movzbl	(%rsi), %r9d
	movzbl	1(%rsi), %ebp
	movzbl	2(%rsi), %ecx
	jmp	.LBB25_17
	.p2align	4, 0x90
.LBB25_16:                              #   in Loop: Header=BB25_12 Depth=1
	movzbl	(%rsi), %r9d
	movl	%r9d, %ebp
	movl	%r9d, %ecx
	jmp	.LBB25_17
	.p2align	4, 0x90
.LBB25_15:                              #   in Loop: Header=BB25_12 Depth=1
	movzbl	1(%rsi), %eax
	leal	(%rax,%rax), %ecx
	movl	%eax, %ebp
	shll	$8, %ebp
	movzbl	(%rsi), %eax
	orl	%eax, %ebp
	leal	(,%rax,8), %r9d
	shrl	$2, %ebp
	andb	$-8, %bpl
	andb	$-8, %cl
	.p2align	4, 0x90
.LBB25_17:                              #   in Loop: Header=BB25_12 Depth=1
	movzbl	%r9b, %eax
	imull	%r15d, %eax
	movzbl	%bpl, %edx
	imull	%r12d, %edx
	addl	%eax, %edx
	movzbl	%cl, %eax
	imull	%r13d, %eax
	addl	%edx, %eax
	cltd
	idivl	%r10d
	movb	%al, (%r8)
	addq	%r14, %rsi
	movq	40(%rbx), %rdi
	movzwl	14(%rbx), %edx
	movzwl	16(%rbx), %eax
	imull	%edx, %eax
	movl	%eax, %edx
	imull	%r14d, %edx
	addq	%rdi, %rdx
	cmpq	%rdx, %rsi
	jae	.LBB25_9
# %bb.18:                               #   in Loop: Header=BB25_12 Depth=1
	addq	$1, %r8
	movzbl	18(%rbx), %edx
	jmp	.LBB25_12
.LBB25_1:
	movl	$17, %eax
	jmp	.LBB25_20
.LBB25_8:
	movq	%r8, %rdi
.LBB25_9:
	movl	%eax, %esi
	callq	realloc
	testq	%rax, %rax
	je	.LBB25_10
# %bb.19:
	movq	%rax, 40(%rbx)
	movb	$8, 18(%rbx)
	movb	$3, 2(%rbx)
	xorl	%eax, %eax
	jmp	.LBB25_20
.LBB25_10:
	movl	$13, %eax
.LBB25_20:
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
.Lfunc_end25:
	.size	tga_desaturate, .Lfunc_end25-tga_desaturate
	.cfi_endproc
	.section	.rodata,"a",@progbits
	.p2align	3
.LJTI25_0:
	.quad	.LBB25_16
	.quad	.LBB25_15
	.quad	.LBB25_14
	.quad	.LBB25_14
                                        # -- End function
	.text
	.globl	tga_desaturate_rec_601_1        # -- Begin function tga_desaturate_rec_601_1
	.p2align	4, 0x90
	.type	tga_desaturate_rec_601_1,@function
tga_desaturate_rec_601_1:               # @tga_desaturate_rec_601_1
	.cfi_startproc
# %bb.0:
	movl	$2989, %esi                     # imm = 0xBAD
	movl	$5866, %edx                     # imm = 0x16EA
	movl	$1145, %ecx                     # imm = 0x479
	movl	$10000, %r8d                    # imm = 0x2710
	jmp	tga_desaturate                  # TAILCALL
.Lfunc_end26:
	.size	tga_desaturate_rec_601_1, .Lfunc_end26-tga_desaturate_rec_601_1
	.cfi_endproc
                                        # -- End function
	.globl	tga_desaturate_rec_709          # -- Begin function tga_desaturate_rec_709
	.p2align	4, 0x90
	.type	tga_desaturate_rec_709,@function
tga_desaturate_rec_709:                 # @tga_desaturate_rec_709
	.cfi_startproc
# %bb.0:
	movl	$2126, %esi                     # imm = 0x84E
	movl	$7152, %edx                     # imm = 0x1BF0
	movl	$722, %ecx                      # imm = 0x2D2
	movl	$10000, %r8d                    # imm = 0x2710
	jmp	tga_desaturate                  # TAILCALL
.Lfunc_end27:
	.size	tga_desaturate_rec_709, .Lfunc_end27-tga_desaturate_rec_709
	.cfi_endproc
                                        # -- End function
	.globl	tga_desaturate_itu              # -- Begin function tga_desaturate_itu
	.p2align	4, 0x90
	.type	tga_desaturate_itu,@function
tga_desaturate_itu:                     # @tga_desaturate_itu
	.cfi_startproc
# %bb.0:
	movl	$2220, %esi                     # imm = 0x8AC
	movl	$7067, %edx                     # imm = 0x1B9B
	movl	$713, %ecx                      # imm = 0x2C9
	movl	$10000, %r8d                    # imm = 0x2710
	jmp	tga_desaturate                  # TAILCALL
.Lfunc_end28:
	.size	tga_desaturate_itu, .Lfunc_end28-tga_desaturate_itu
	.cfi_endproc
                                        # -- End function
	.globl	tga_desaturate_avg              # -- Begin function tga_desaturate_avg
	.p2align	4, 0x90
	.type	tga_desaturate_avg,@function
tga_desaturate_avg:                     # @tga_desaturate_avg
	.cfi_startproc
# %bb.0:
	movl	$1, %esi
	movl	$1, %edx
	movl	$1, %ecx
	movl	$3, %r8d
	jmp	tga_desaturate                  # TAILCALL
.Lfunc_end29:
	.size	tga_desaturate_avg, .Lfunc_end29-tga_desaturate_avg
	.cfi_endproc
                                        # -- End function
	.globl	tga_convert_depth               # -- Begin function tga_convert_depth
	.p2align	4, 0x90
	.type	tga_convert_depth,@function
tga_convert_depth:                      # @tga_convert_depth
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
	movl	%esi, %r13d
	movl	$12, %r14d
	cmpb	$32, %r13b
	ja	.LBB30_48
# %bb.1:
	movl	%r13d, %eax
	movabsq	$4311810048, %rcx               # imm = 0x101010000
	btq	%rax, %rcx
	jae	.LBB30_48
# %bb.2:
	movq	%rdi, %r12
	movb	18(%rdi), %sil
	leal	-8(%rsi), %eax
	rolb	$5, %al
	cmpb	$3, %al
	ja	.LBB30_48
# %bb.3:
	movb	2(%r12), %al
	andb	$-9, %al
	cmpb	$1, %al
	jne	.LBB30_6
# %bb.4:
	movq	%r12, %rdi
	callq	tga_color_unmap
	movl	%eax, %r14d
	testl	%eax, %eax
	jne	.LBB30_48
# %bb.5:
	movb	18(%r12), %sil
.LBB30_6:
	xorl	%r14d, %r14d
	cmpb	%r13b, %sil
	je	.LBB30_48
# %bb.7:
	movl	%esi, %ecx
	shrb	$3, %cl
	movl	%r13d, %edx
	shrb	$3, %dl
	movzwl	14(%r12), %edi
	movzwl	16(%r12), %eax
	imull	%edi, %eax
	movzbl	%cl, %r15d
	movl	%eax, %ecx
	imull	%r15d, %ecx
	movzbl	%dl, %ebx
	imull	%ebx, %eax
	cmpl	%eax, %ecx
	jbe	.LBB30_18
# %bb.8:
	movq	40(%r12), %rax
	movl	%ebx, 4(%rsp)                   # 4-byte Spill
	movl	%ebx, %r8d
	movl	%r15d, %r9d
	cmpb	$32, %r13b
	jne	.LBB30_29
# %bb.9:
	leaq	3(%rax), %rcx
                                        # implicit-def: $r11b
                                        # implicit-def: $bl
                                        # implicit-def: $dl
                                        # implicit-def: $r10b
	.p2align	4, 0x90
.LBB30_10:                              # =>This Inner Loop Header: Depth=1
	movzbl	%sil, %esi
	addl	$-8, %esi
	roll	$29, %esi
	cmpl	$3, %esi
	ja	.LBB30_16
# %bb.11:                               #   in Loop: Header=BB30_10 Depth=1
	jmpq	*.LJTI30_3(,%rsi,8)
.LBB30_12:                              #   in Loop: Header=BB30_10 Depth=1
	movzbl	(%rax), %r10d
	xorl	%r11d, %r11d
	movl	%r10d, %edx
	movl	%r10d, %ebx
	jmp	.LBB30_16
	.p2align	4, 0x90
.LBB30_13:                              #   in Loop: Header=BB30_10 Depth=1
	movzbl	1(%rax), %edx
	leal	(%rdx,%rdx), %ebx
                                        # kill: def $edx killed $edx killed $rdx
	shll	$8, %edx
	movswl	%dx, %r11d
	movzbl	(%rax), %esi
	orl	%esi, %edx
	leal	(,%rsi,8), %r10d
	shrl	$2, %edx
	andb	$-8, %dl
	andb	$-8, %bl
	shrl	$15, %r11d
	jmp	.LBB30_16
	.p2align	4, 0x90
.LBB30_14:                              #   in Loop: Header=BB30_10 Depth=1
	movzbl	(%rax), %r10d
	movzbl	1(%rax), %edx
	movzbl	2(%rax), %ebx
	xorl	%r11d, %r11d
	jmp	.LBB30_16
	.p2align	4, 0x90
.LBB30_15:                              #   in Loop: Header=BB30_10 Depth=1
	movzbl	(%rax), %r10d
	movzbl	1(%rax), %edx
	movzbl	2(%rax), %ebx
	movzbl	3(%rax), %r11d
	.p2align	4, 0x90
.LBB30_16:                              #   in Loop: Header=BB30_10 Depth=1
	movb	%r10b, -3(%rcx)
	movb	%dl, -2(%rcx)
	movb	%bl, -1(%rcx)
	movb	%r11b, (%rcx)
	addq	%r9, %rax
	movq	40(%r12), %rdi
	movzwl	14(%r12), %ebp
	movzwl	16(%r12), %esi
	imull	%ebp, %esi
	movl	%esi, %ebp
	imull	%r15d, %ebp
	addq	%rdi, %rbp
	cmpq	%rbp, %rax
	jae	.LBB30_43
# %bb.17:                               #   in Loop: Header=BB30_10 Depth=1
	movzbl	18(%r12), %esi
	addq	%r8, %rcx
	jmp	.LBB30_10
.LBB30_18:
	movl	%eax, %esi
	movq	40(%r12), %rdi
	callq	realloc
	testq	%rax, %rax
	je	.LBB30_47
# %bb.19:
	movq	%rax, 40(%r12)
	movzwl	14(%r12), %edx
	movzwl	16(%r12), %ecx
	imull	%edx, %ecx
	addl	$-1, %ecx
	movl	%ecx, %esi
	imull	%r15d, %esi
	testl	%esi, %esi
	js	.LBB30_45
# %bb.20:
	imull	%ebx, %ecx
	movslq	%ecx, %rdx
	movslq	%esi, %rcx
	addq	%rax, %rcx
	movl	%ebx, %r8d
	negq	%r8
	movl	%r15d, %r9d
	negq	%r9
	cmpb	$32, %r13b
	jne	.LBB30_49
# %bb.21:
	addq	%rdx, %rax
	addq	$3, %rax
                                        # implicit-def: $sil
                                        # implicit-def: $dl
                                        # implicit-def: $dil
                                        # implicit-def: $bpl
	jmp	.LBB30_24
.LBB30_22:                              #   in Loop: Header=BB30_24 Depth=1
	movzbl	(%rcx), %ebp
	movzbl	1(%rcx), %edi
	movzbl	2(%rcx), %edx
	movzbl	3(%rcx), %esi
	.p2align	4, 0x90
.LBB30_23:                              #   in Loop: Header=BB30_24 Depth=1
	movb	%bpl, -3(%rax)
	movb	%dil, -2(%rax)
	movb	%dl, -1(%rax)
	movb	%sil, (%rax)
	addq	%r9, %rcx
	addq	%r8, %rax
	cmpq	40(%r12), %rcx
	jb	.LBB30_45
.LBB30_24:                              # =>This Inner Loop Header: Depth=1
	movzbl	18(%r12), %ebx
	addl	$-8, %ebx
	roll	$29, %ebx
	cmpl	$3, %ebx
	ja	.LBB30_23
# %bb.25:                               #   in Loop: Header=BB30_24 Depth=1
	jmpq	*.LJTI30_1(,%rbx,8)
.LBB30_26:                              #   in Loop: Header=BB30_24 Depth=1
	movzbl	(%rcx), %ebp
	xorl	%esi, %esi
	movl	%ebp, %edi
	movl	%ebp, %edx
	jmp	.LBB30_23
.LBB30_27:                              #   in Loop: Header=BB30_24 Depth=1
	movzbl	1(%rcx), %esi
	leal	(%rsi,%rsi), %edx
	movl	%esi, %edi
	shll	$8, %edi
	movswl	%di, %esi
	movzbl	(%rcx), %ebp
	orl	%ebp, %edi
	leal	(,%rbp,8), %ebp
	shrl	$2, %edi
	andb	$-8, %dil
	andb	$-8, %dl
	shrl	$15, %esi
	jmp	.LBB30_23
.LBB30_28:                              #   in Loop: Header=BB30_24 Depth=1
	movzbl	(%rcx), %ebp
	movzbl	1(%rcx), %edi
	movzbl	2(%rcx), %edx
	xorl	%esi, %esi
	jmp	.LBB30_23
.LBB30_29:
	movq	%rax, %r14
                                        # implicit-def: $r10b
                                        # implicit-def: $bpl
                                        # implicit-def: $dl
                                        # implicit-def: $r11b
	.p2align	4, 0x90
.LBB30_30:                              # =>This Inner Loop Header: Depth=1
	movzbl	%sil, %esi
	addl	$-8, %esi
	roll	$29, %esi
	cmpl	$3, %esi
	ja	.LBB30_35
# %bb.31:                               #   in Loop: Header=BB30_30 Depth=1
	jmpq	*.LJTI30_2(,%rsi,8)
.LBB30_32:                              #   in Loop: Header=BB30_30 Depth=1
	movzbl	(%rax), %r11d
	xorl	%r10d, %r10d
	movl	%r11d, %edx
	movl	%r11d, %ebp
	cmpb	$16, %r13b
	jne	.LBB30_36
	.p2align	4, 0x90
.LBB30_39:                              #   in Loop: Header=BB30_30 Depth=1
	movl	%r11d, %ebx
	shrb	$3, %bl
	movl	%edx, %ecx
	shrb	$3, %cl
	movzbl	%cl, %ecx
	shll	$5, %ecx
	movzbl	%bl, %esi
	orl	%ecx, %esi
	movl	%ebp, %ecx
	shrb	$3, %cl
	movzbl	%cl, %ecx
	shll	$10, %ecx
	orl	%esi, %ecx
	movl	%ecx, %esi
	orl	$-32768, %esi                   # imm = 0x8000
	testb	%r10b, %r10b
	cmovnsl	%ecx, %esi
	movb	%sil, (%r14)
	shrl	$8, %esi
	movl	$1, %edi
	jmp	.LBB30_40
	.p2align	4, 0x90
.LBB30_33:                              #   in Loop: Header=BB30_30 Depth=1
	movzbl	1(%rax), %edx
	leal	(%rdx,%rdx), %ebp
                                        # kill: def $edx killed $edx killed $rdx
	shll	$8, %edx
	movswl	%dx, %r10d
	movzbl	(%rax), %esi
	orl	%esi, %edx
	leal	(,%rsi,8), %r11d
	shrl	$2, %edx
	andb	$-8, %dl
	andb	$-8, %bpl
	shrl	$15, %r10d
	cmpb	$16, %r13b
	jne	.LBB30_36
	jmp	.LBB30_39
	.p2align	4, 0x90
.LBB30_34:                              #   in Loop: Header=BB30_30 Depth=1
	movzbl	(%rax), %r11d
	movzbl	1(%rax), %edx
	movzbl	2(%rax), %ebp
	xorl	%r10d, %r10d
	.p2align	4, 0x90
.LBB30_35:                              #   in Loop: Header=BB30_30 Depth=1
	cmpb	$16, %r13b
	je	.LBB30_39
.LBB30_36:                              #   in Loop: Header=BB30_30 Depth=1
	cmpb	$24, %r13b
	jne	.LBB30_41
# %bb.37:                               #   in Loop: Header=BB30_30 Depth=1
	movb	%r11b, (%r14)
	movb	%dl, 1(%r14)
	movl	$2, %edi
	movl	%ebp, %esi
.LBB30_40:                              #   in Loop: Header=BB30_30 Depth=1
	movb	%sil, (%r14,%rdi)
.LBB30_41:                              #   in Loop: Header=BB30_30 Depth=1
	addq	%r9, %rax
	movq	40(%r12), %rdi
	movzwl	14(%r12), %ecx
	movzwl	16(%r12), %esi
	imull	%ecx, %esi
	movl	%esi, %ecx
	imull	%r15d, %ecx
	addq	%rdi, %rcx
	cmpq	%rcx, %rax
	jae	.LBB30_43
# %bb.42:                               #   in Loop: Header=BB30_30 Depth=1
	addq	%r8, %r14
	movzbl	18(%r12), %esi
	jmp	.LBB30_30
	.p2align	4, 0x90
.LBB30_38:                              #   in Loop: Header=BB30_30 Depth=1
	movzbl	(%rax), %r11d
	movzbl	1(%rax), %edx
	movzbl	2(%rax), %ebp
	movzbl	3(%rax), %r10d
	cmpb	$16, %r13b
	jne	.LBB30_36
	jmp	.LBB30_39
.LBB30_43:
	imull	4(%rsp), %esi                   # 4-byte Folded Reload
	callq	realloc
	testq	%rax, %rax
	je	.LBB30_47
# %bb.44:
	movq	%rax, 40(%r12)
	xorl	%r14d, %r14d
.LBB30_45:
	movb	%r13b, 18(%r12)
	jmp	.LBB30_48
.LBB30_47:
	movl	$13, %r14d
.LBB30_48:
	movl	%r14d, %eax
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
.LBB30_49:
	.cfi_def_cfa_offset 64
	addq	%rdx, %rax
                                        # implicit-def: $r10b
                                        # implicit-def: $sil
                                        # implicit-def: $dil
                                        # implicit-def: $r11b
	jmp	.LBB30_53
.LBB30_50:                              #   in Loop: Header=BB30_53 Depth=1
	movl	%r11d, %edx
	shrb	$3, %dl
	movl	%edi, %ebx
	shrb	$3, %bl
	movzbl	%bl, %ebx
	shll	$5, %ebx
	movzbl	%dl, %edx
	orl	%ebx, %edx
	movl	%esi, %ebx
	shrb	$3, %bl
	movzbl	%bl, %ebx
	shll	$10, %ebx
	orl	%edx, %ebx
	movl	%ebx, %edx
	orl	$-32768, %edx                   # imm = 0x8000
	testb	%r10b, %r10b
	cmovnsl	%ebx, %edx
	movb	%dl, (%rax)
	shrl	$8, %edx
	movl	$1, %ebp
.LBB30_51:                              #   in Loop: Header=BB30_53 Depth=1
	movb	%dl, (%rax,%rbp)
.LBB30_52:                              #   in Loop: Header=BB30_53 Depth=1
	addq	%r8, %rax
	addq	%r9, %rcx
	cmpq	40(%r12), %rcx
	jb	.LBB30_45
.LBB30_53:                              # =>This Inner Loop Header: Depth=1
	movzbl	18(%r12), %edx
	addl	$-8, %edx
	roll	$29, %edx
	cmpl	$3, %edx
	ja	.LBB30_58
# %bb.54:                               #   in Loop: Header=BB30_53 Depth=1
	jmpq	*.LJTI30_0(,%rdx,8)
.LBB30_55:                              #   in Loop: Header=BB30_53 Depth=1
	movzbl	(%rcx), %r11d
	xorl	%r10d, %r10d
	movl	%r11d, %edi
	movl	%r11d, %esi
	cmpb	$16, %r13b
	jne	.LBB30_59
	jmp	.LBB30_50
.LBB30_56:                              #   in Loop: Header=BB30_53 Depth=1
	movzbl	1(%rcx), %edx
	leal	(%rdx,%rdx), %esi
	movl	%edx, %edi
	shll	$8, %edi
	movswl	%di, %r10d
	movzbl	(%rcx), %edx
	orl	%edx, %edi
	leal	(,%rdx,8), %r11d
	shrl	$2, %edi
	andb	$-8, %dil
	andb	$-8, %sil
	shrl	$15, %r10d
	cmpb	$16, %r13b
	jne	.LBB30_59
	jmp	.LBB30_50
.LBB30_57:                              #   in Loop: Header=BB30_53 Depth=1
	movzbl	(%rcx), %r11d
	movzbl	1(%rcx), %edi
	movzbl	2(%rcx), %esi
	xorl	%r10d, %r10d
	.p2align	4, 0x90
.LBB30_58:                              #   in Loop: Header=BB30_53 Depth=1
	cmpb	$16, %r13b
	je	.LBB30_50
.LBB30_59:                              #   in Loop: Header=BB30_53 Depth=1
	cmpb	$24, %r13b
	jne	.LBB30_52
# %bb.60:                               #   in Loop: Header=BB30_53 Depth=1
	movb	%r11b, (%rax)
	movb	%dil, 1(%rax)
	movl	$2, %ebp
	movl	%esi, %edx
	jmp	.LBB30_51
.LBB30_61:                              #   in Loop: Header=BB30_53 Depth=1
	movzbl	(%rcx), %r11d
	movzbl	1(%rcx), %edi
	movzbl	2(%rcx), %esi
	movzbl	3(%rcx), %r10d
	cmpb	$16, %r13b
	jne	.LBB30_59
	jmp	.LBB30_50
.Lfunc_end30:
	.size	tga_convert_depth, .Lfunc_end30-tga_convert_depth
	.cfi_endproc
	.section	.rodata,"a",@progbits
	.p2align	3
.LJTI30_0:
	.quad	.LBB30_55
	.quad	.LBB30_56
	.quad	.LBB30_57
	.quad	.LBB30_61
.LJTI30_1:
	.quad	.LBB30_26
	.quad	.LBB30_27
	.quad	.LBB30_28
	.quad	.LBB30_22
.LJTI30_2:
	.quad	.LBB30_32
	.quad	.LBB30_33
	.quad	.LBB30_34
	.quad	.LBB30_38
.LJTI30_3:
	.quad	.LBB30_12
	.quad	.LBB30_13
	.quad	.LBB30_14
	.quad	.LBB30_15
                                        # -- End function
	.type	.L.str,@object                  # @.str
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str:
	.asciz	"no error"
	.size	.L.str, 9

	.type	.L.str.1,@object                # @.str.1
.L.str.1:
	.asciz	"error opening file"
	.size	.L.str.1, 19

	.type	.L.str.2,@object                # @.str.2
.L.str.2:
	.asciz	"premature end of file"
	.size	.L.str.2, 22

	.type	.L.str.3,@object                # @.str.3
.L.str.3:
	.asciz	"error writing to file"
	.size	.L.str.3, 22

	.type	.L.str.4,@object                # @.str.4
.L.str.4:
	.asciz	"invalid color map type"
	.size	.L.str.4, 23

	.type	.L.str.5,@object                # @.str.5
.L.str.5:
	.asciz	"invalid image type"
	.size	.L.str.5, 19

	.type	.L.str.6,@object                # @.str.6
.L.str.6:
	.asciz	"no image data included"
	.size	.L.str.6, 23

	.type	.L.str.7,@object                # @.str.7
.L.str.7:
	.asciz	"color-mapped image without color map"
	.size	.L.str.7, 37

	.type	.L.str.8,@object                # @.str.8
.L.str.8:
	.asciz	"non-color-mapped image with extraneous color map"
	.size	.L.str.8, 49

	.type	.L.str.9,@object                # @.str.9
.L.str.9:
	.asciz	"color map has zero length"
	.size	.L.str.9, 26

	.type	.L.str.10,@object               # @.str.10
.L.str.10:
	.asciz	"invalid color map depth"
	.size	.L.str.10, 24

	.type	.L.str.11,@object               # @.str.11
.L.str.11:
	.asciz	"the image dimensions are zero"
	.size	.L.str.11, 30

	.type	.L.str.12,@object               # @.str.12
.L.str.12:
	.asciz	"invalid pixel depth"
	.size	.L.str.12, 20

	.type	.L.str.13,@object               # @.str.13
.L.str.13:
	.asciz	"out of memory"
	.size	.L.str.13, 14

	.type	.L.str.14,@object               # @.str.14
.L.str.14:
	.asciz	"image is not color mapped"
	.size	.L.str.14, 26

	.type	.L.str.15,@object               # @.str.15
.L.str.15:
	.asciz	"RLE data is corrupt"
	.size	.L.str.15, 20

	.type	.L.str.16,@object               # @.str.16
.L.str.16:
	.asciz	"color map index out of range"
	.size	.L.str.16, 29

	.type	.L.str.17,@object               # @.str.17
.L.str.17:
	.asciz	"image is mono"
	.size	.L.str.17, 14

	.type	.L.str.18,@object               # @.str.18
.L.str.18:
	.asciz	"unknown error code"
	.size	.L.str.18, 19

	.type	.L.str.19,@object               # @.str.19
.L.str.19:
	.asciz	"rb"
	.size	.L.str.19, 3

	.type	.L.str.20,@object               # @.str.20
.L.str.20:
	.asciz	"wb"
	.size	.L.str.20, 3

	.type	tga_id,@object                  # @tga_id
	.section	.rodata,"a",@progbits
	.p2align	4
tga_id:
	.asciz	"\000\000\000\000\000\000\000\000TRUEVISION-XFILE."
	.size	tga_id, 26

	.type	.Lswitch.table.tga_error,@object # @switch.table.tga_error
	.p2align	3
.Lswitch.table.tga_error:
	.quad	.L.str
	.quad	.L.str.1
	.quad	.L.str.2
	.quad	.L.str.3
	.quad	.L.str.4
	.quad	.L.str.5
	.quad	.L.str.6
	.quad	.L.str.7
	.quad	.L.str.8
	.quad	.L.str.9
	.quad	.L.str.10
	.quad	.L.str.11
	.quad	.L.str.12
	.quad	.L.str.13
	.quad	.L.str.14
	.quad	.L.str.15
	.quad	.L.str.16
	.quad	.L.str.17
	.size	.Lswitch.table.tga_error, 144

	.ident	"Ubuntu clang version 11.0.0-2"
	.section	".note.GNU-stack","",@progbits
	.addrsig
	.addrsig_sym tga_id
