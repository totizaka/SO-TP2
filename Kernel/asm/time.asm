GLOBAL getHs
GLOBAL getMins
GLOBAL getSecs

section .text

	getSecs:
	push rbp
	mov rbp, rsp

	mov al, 0x00			;carga en al registro de segs
	out 70h, al				;escribe el valor en el 70h, seleccionando registor de segs
	in al, 71h				;lee el valor de los segs en el 71h

	mov rsp, rbp
	pop rbp
	ret

	getMins:
	push rbp
	mov rbp, rsp

	mov al, 0x02
	out 70h, al
	in al, 71h

	mov rsp, rbp
	pop rbp
	ret

	getHs:
	push rbp
	mov rbp, rsp

	mov al, 0x04
	out 70h, al
	in al, 71h

	mov rsp, rbp
	pop rbp
	ret