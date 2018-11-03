	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 13
	.intel_syntax noprefix
	.globl	_main                   ## -- Begin function main
	.p2align	4, 0x90
_main:                                  ## @main
## BB#0:
	push	rbp
	mov	rbp, rsp
	sub	rsp, 32
	mov	eax, 4
	mov	ecx, eax
	mov	dword ptr [rbp - 4], 0
	mov	dword ptr [rbp - 8], edi
	mov	qword ptr [rbp - 16], rsi
	mov	rdi, rcx
	call	_malloc
	xor	edx, edx
	mov	qword ptr [rbp - 24], rax
	mov	rax, qword ptr [rbp - 24]
	mov	dword ptr [rax], 30
	mov	eax, edx
	add	rsp, 32
	pop	rbp
	ret
                                        ## -- End function

.subsections_via_symbols
