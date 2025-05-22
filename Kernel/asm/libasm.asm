GLOBAL cpuVendor
GLOBAL RTC
GLOBAL key_pressed
GLOBAL inb
GLOBAL outb
GLOBAL aqcuire
GLOBAL release



section .text

aqcuire:
mov al, 0
.ciclo:
xchg byte[rdi], al
test al, al
jnz ciclo
ret

release:
mov byte[rdi], 1
ret




outb:
	push rbp
    mov rbp, rsp

	mov rdx, rdi
	mov rax, rsi

	out dx, al

	mov rsp, rbp
	pop rbp
	ret



inb:
	push rbp
	mov rbp, rsp 

	mov rdx, rdi
	in al, dx
	
	mov rsp,rbp
	pop rbp
	ret


	
cpuVendor:
	push rbp
	mov rbp, rsp

	push rbx

	mov rax, 0
	cpuid


	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx

	mov byte [rdi+13], 0

	mov rax, rdi

	pop rbx

	mov rsp, rbp
	pop rbp
	ret

RTC:
push rbp
mov rbp, rsp

mov rax, rdi
out 70h, al
in al, 71h 

mov rsp, rbp
pop rbp
ret

key_pressed:
    push rbp
    mov rbp, rsp

    mov rax, 0
.inicio: 
    in al, 64h                ;64h  read   8042 status register, lee el puerto de estado del controlador de teclado
    and al, 0x01              ;verifica si el bit 0 está activo. El bit 0 de este puerto indica si hay un byte de datos listo para ser leído desde el teclado.
    je .inicio                ;si el bit 0 es 0 (no hay dato listo), salta a .inicio y vuelve a verificar. Esto crea un bucle de espera hasta que haya una tecla presionada.
    in al, 60h                ;60h  read   8042 output register, puerto donde se reciben los códigos de tecla del teclado, (should only be read if Bit 0 of status port is set to 1)

    mov rsp, rbp
    pop rbp
    ret

