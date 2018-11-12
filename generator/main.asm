	 global _main
	 segment .text
_main:
	 push rbp
	 mov rbp, rsp
	 sub rsp, 16
	 mov rax, 0
	 cmp rax, 0
	 mov rax, 0
	 sete al
	 leave
	 ret
