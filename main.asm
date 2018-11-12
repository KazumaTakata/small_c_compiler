	 global _main
	 segment .text
_main:
	 push rbp
	 mov rbp, rsp
	 sub rsp, 16
	 mov rax, 3
	 push rax
	 mov rax, 4
	 pop rcx
	 add rax, rcx
	 leave
	 ret
