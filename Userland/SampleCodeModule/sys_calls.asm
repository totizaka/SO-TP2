GLOBAL syscall_read
GLOBAL syscall_write
GLOBAL syscall_time
GLOBAL syscall_getWidth_vd
GLOBAL syscall_getHeight_vd
GLOBAL syscall_drawPixel
GLOBAL syscall_drawSquare
GLOBAL syscall_sleep
GLOBAL syscall_seconds
GLOBAL syscall_paintAll_vd
GLOBAL syscall_erraseChar
GLOBAL syscall_increaseFS
GLOBAL syscall_decreaseFS
GLOBAL syscall_setPixelSize
GLOBAL syscall_getCurrentPixelSize
GLOBAL syscall_erraseLine
GLOBAL syscall_beep
GLOBAL syscall_regs

section .text

syscall_read:
    mov rax, 0x00
    int 80h
    ret

syscall_write:
    mov rax, 0x01
    int 80h
    ret

syscall_time:
    mov rax, 0x02
    int 80h
    ret

syscall_drawPixel:
    mov rax, 0x03
    int 80h
    ret

syscall_drawSquare:
    mov rax, 0x04
    mov r10, rcx        ;4to parametro de syscall es R10
    int 80h
    ret

syscall_getWidth_vd:
    mov rax, 0x05
    int 80h
    ret

syscall_getHeight_vd:
    mov rax, 0x06
    int 80h
    ret

syscall_sleep:
    mov rax, 0x07
    int 80h
    ret 

syscall_paintAll_vd:
    mov rax, 0x08
    int 80h
    ret

syscall_erraseChar:
    mov rax, 0x09
    int 80h
    ret

syscall_increaseFS:
    mov rax, 0x0A
    int 80h
    ret

syscall_decreaseFS:
    mov rax, 0x0B
    int 80h
    ret

syscall_setPixelSize:
    mov rax, 0x0C
    int 80h
    ret

syscall_getCurrentPixelSize:
    mov rax, 0x0D
    int 80h
    ret

syscall_erraseLine:
    mov rax, 0x0E
    int 80h
    ret
    
syscall_beep:
    mov rax, 0x0F
    int 80h
    ret

syscall_regs:
    mov rax, 0x10
    int 80h
    ret

