#ifndef _SYSCALLS_H_
#define _SYSCALLS_H_

#include <stdint.h>

extern uint64_t syscall_read(uint64_t fd, char* buf);
extern uint64_t syscall_write(uint64_t fd, const char *buf, uint64_t count);
extern uint64_t syscall_time();
extern uint64_t syscall_draw_pixel();
extern uint64_t syscall_draw_square();
extern uint64_t syscall_get_width_vd();
extern uint64_t syscall_get_height_vd();
extern uint64_t syscall_sleep();
extern uint64_t syscall_paint_all_vd();
extern uint64_t syscall_errase_char();
extern uint64_t syscall_increase_FS();
extern uint64_t syscall_decrease_FS();
extern uint64_t syscall_set_pixel_size();
extern uint64_t syscall_get_current_pixel_size();
extern uint64_t syscall_errase_line();
extern void syscall_beep();
extern uint64_t syscall_regs();

// SO


extern int64_t syscall_my_getpid();
extern int64_t syscall_my_create_process(char *name, uint64_t argc, char *argv[]);
extern int64_t syscall_my_nice(uint64_t pid, uint64_t newPrio);
extern int64_t syscall_my_kill(uint64_t pid);
extern int64_t syscall_my_block(uint64_t pid);
extern int64_t syscall_my_unblock(uint64_t pid);
extern int64_t syscall_my_sem_open(char *sem_id, uint64_t initialValue);
extern int64_t syscall_my_sem_wait(char *sem_id);
extern int64_t syscall_my_sem_post(char *sem_id);
extern int64_t syscall_my_sem_close(char *sem_id);
extern int64_t syscall_my_yield();
extern int64_t syscall_my_wait(int64_t pid);

#endif