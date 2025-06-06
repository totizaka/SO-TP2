GLOBAL syscall_read
GLOBAL syscall_write
GLOBAL syscall_time
GLOBAL syscall_get_width_vd
GLOBAL syscall_get_height_vd
GLOBAL syscall_draw_pixel
GLOBAL syscall_draw_square
GLOBAL syscall_sleep
GLOBAL syscall_seconds
GLOBAL syscall_paint_all_vd
GLOBAL syscall_errase_char
GLOBAL syscall_increase_FS
GLOBAL syscall_decrease_FS
GLOBAL syscall_set_pixel_size
GLOBAL syscall_get_current_pixel_size
GLOBAL syscall_errase_line
GLOBAL syscall_beep
GLOBAL syscall_regs
GLOBAL syscall_my_getpid
GLOBAL syscall_my_create_process
GLOBAL syscall_my_nice
GLOBAL syscall_my_kill
GLOBAL syscall_my_block
GLOBAL syscall_my_unblock
GLOBAL syscall_my_sem_open
GLOBAL syscall_my_sem_wait
GLOBAL syscall_my_sem_post
GLOBAL syscall_my_sem_close
GLOBAL syscall_my_yield
GLOBAL syscall_my_wait
GLOBAL syscall_my_malloc
GLOBAL syscall_my_free
GLOBAL syscall_my_sem_open_get_id
GLOBAL syscall_my_get_processes
GLOBAL syscall_my_free_processes
GLOBAL syscall_my_sleep
GLOBAL syscall_my_open_pipe:
GLOBAL syscall_my_write_pipe
GLOBAL syscall_my_read_pipe
GLOBAL syscall_my_close_pipe

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

syscall_draw_pixel:
    mov rax, 0x03
    int 80h
    ret

syscall_draw_square:
    mov rax, 0x04
    mov r10, rcx        ;4to parametro de syscall es R10
    int 80h
    ret

syscall_get_width_vd:
    mov rax, 0x05
    int 80h
    ret

syscall_get_height_vd:
    mov rax, 0x06
    int 80h
    ret

syscall_sleep:
    mov rax, 0x07
    int 80h
    ret 

syscall_paint_all_vd:
    mov rax, 0x08
    int 80h
    ret

syscall_errase_char:
    mov rax, 0x09
    int 80h
    ret

syscall_increase_FS:
    mov rax, 0x0A
    int 80h
    ret

syscall_decrease_FS:
    mov rax, 0x0B
    int 80h
    ret

syscall_set_pixel_size:
    mov rax, 0x0C
    int 80h
    ret

syscall_get_current_pixel_size:
    mov rax, 0x0D
    int 80h
    ret

syscall_errase_line:
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

syscall_my_getpid:
    mov rax, 0x11
    int 80h
    ret

syscall_my_create_process:
    mov rax, 0x12
    int 80h
    ret

syscall_my_nice:
    mov rax, 0x13
    int 80h
    ret

syscall_my_kill:
    mov rax, 0x14
    int 80h
    ret

syscall_my_block:
    mov rax, 0x15
    int 80h
    ret

syscall_my_unblock:
    mov rax, 0x16
    int 80h
    ret

syscall_my_sem_open:
    mov rax, 0x17
    int 80h
    ret

syscall_my_sem_wait:
    mov rax, 0x18
    int 80h
    ret

syscall_my_sem_post:
    mov rax, 0x19
    int 80h
    ret

syscall_my_sem_close:
    mov rax, 0x1A
    int 80h
    ret

syscall_my_yield:
    mov rax, 0x1B
    int 80h
    ret

syscall_my_wait:
    mov rax, 0x1C
    int 80h
    ret

syscall_my_malloc:
    mov rax, 0x1D
    int 80h
    ret

syscall_my_free:
    mov rax, 0x1E
    int 80h
    ret


syscall_my_get_processes:
    mov rax, 0x1F
    int 80h
    ret

syscall_my_free_processes:
    mov rax, 0x20
    int 80h
    ret
    
syscall_my_sem_open_get_id:
    mov rax, 0x21
    int 80h
    ret

syscall_my_sleep:
    mov rax, 0x22
    int 80h
    ret


syscall_my_open_pipe:
    mov rax, 0x23
    int 80h
    ret
syscall_my_write_pipe:
    mov rax, 0x24
    int 80h
    ret
syscall_my_read_pipe:
    mov rax, 0x25
    int 80h
    ret

syscall_my_close_pipe:
    mov rax, 0x26
    int 80h
    ret