GLOBAL get_hs
GLOBAL get_mins
GLOBAL get_secs

section .text

	get_secs:
	push rbp
	mov rbp, rsp

	mov al, 0x00			;carga en al registro de segs
	out 70h, al				;escribe el valor en el 70h, seleccionando registor de segs
	in al, 71h				;lee el valor de los segs en el 71h

	mov rsp, rbp
	pop rbp
	ret

	get_mins:
	push rbp
	mov rbp, rsp

	mov al, 0x02
	out 70h, al
	in al, 71h

	mov rsp, rbp
	pop rbp
	ret

	get_hs:
	push rbp
	mov rbp, rsp

	mov al, 0x04
	out 70h, al
	in al, 71h

	mov rsp, rbp
	pop rbp
	ret