#ifndef _SYSCALLS_H_
#define _SYSCALLS_H_

#include <stdint.h>
#include <shared_defs.h>

// extern uint64_t syscall_read(uint64_t fd, char* buf);
// extern uint64_t syscall_write(uint64_t fd, const char *buf, uint64_t count);
extern uint64_t syscall_time();
extern uint64_t syscall_draw_pixel(uint32_t color, uint64_t x, uint64_t y);
extern uint64_t syscall_draw_square(uint32_t color, uint64_t x, uint64_t y, uint64_t thickness);
extern uint64_t syscall_get_width_vd();
extern uint64_t syscall_get_height_vd();
extern uint64_t syscall_sleep(uint64_t secs);
extern uint64_t syscall_paint_all_vd(uint32_t hexColor);
extern uint64_t syscall_errase_char(uint32_t hexColor);
extern uint64_t syscall_increase_FS();
extern uint64_t syscall_decrease_FS();
extern uint64_t syscall_set_pixel_size();
extern uint64_t syscall_get_current_pixel_size();
extern uint64_t syscall_errase_line();
extern void syscall_beep(int secs, int freq);
extern uint64_t syscall_regs(uint64_t * regs);

// SO

extern int64_t syscall_my_getpid();
extern int64_t syscall_my_create_process(void* rip, char ** argv, uint64_t argc, int8_t background, fd_t fds[FD_MAX]);
extern int64_t syscall_my_nice(uint64_t pid, uint64_t newPrio);
extern int64_t syscall_my_kill(uint64_t pid);
extern int64_t syscall_my_block(uint64_t pid);
extern int64_t syscall_my_unblock(uint64_t pid);
extern int64_t syscall_my_yield();
extern int64_t syscall_my_wait(int64_t pid,int64_t* ret );
extern int64_t syscall_my_malloc(uint64_t size);
extern int64_t syscall_my_free(uint64_t ptr);
extern int64_t syscall_my_sem_open ( int64_t sem_id, int value);
extern int64_t syscall_my_sem_post ( int64_t sem_id );
extern int64_t syscall_my_sem_wait ( int64_t sem_id);
extern int64_t syscall_my_sem_close ( int64_t sem_id);
extern int64_t syscall_my_sem_open_get_id ( int value);
extern process_info_list * syscall_my_get_processes();
extern int64_t syscall_my_free_processes(process_info_list *processes);
extern memory_state* syscall_my_mem_state();
extern void syscall_my_free_mem_state(memory_state *state);
extern void syscall_my_create_pipe(int64_t id);
extern int8_t syscall_my_open_pipe(int64_t target, int role);
extern int64_t syscall_my_write_pipe(int64_t target, char * buffer,int num_bytes);
extern int64_t syscall_my_read_pipe(int64_t target, char * buffer,  int num_bytes);
extern int8_t syscall_my_close_pipe(int64_t target);
extern int64_t syscall_my_get_available_pipe_id();
extern int64_t syscall_my_read (int64_t fd, char* buffer, int num_bytes);
extern int64_t syscall_my_write (int64_t fd, char* buffer, int num_bytes);


#endif