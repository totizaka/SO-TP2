GLOBAL div0
global opcode_exc

section .text

div0:
push rbp
mov rbp, rsp

;Descomentar para ver que se imprimen bien los registros 

mov rax, 0
; mov rbx, 1
; mov rcx, 2
mov rdx, 3
;mov rsi, 4
;mov rdi, 5
;mov rbp, 6
;mov r8, 8
;mov r9, 9
;mov r10, 10
;mov r11, 11
;mov r12, 12
;mov r13, 13
;mov r14, 14
;mov r15, 15

div rax

mov rsp, rbp
pop rbp
ret 


opcode_exc:
push rbp
mov rbp, rsp


;mov rax, 0
;mov rbx, 1
;mov rcx, 2
;mov rdx, 3
;mov rsi, 4
;mov rdi, 5
;mov rbp, 6
;mov r8, 8
;mov r9, 9
;mov r10, 10
;mov r11, 11
;mov r12, 12
;mov r13, 13
;mov r14, 14
;mov r15, 15

jmp 0

mov rsp, rbp
pop rbp
ret