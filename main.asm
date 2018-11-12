	 global _main
	 segment .text
_main:
	 push rbp
	 mov rbp, rsp
	 sub rsp, 16
	 mov rax, 2
	 leave
	 ret
