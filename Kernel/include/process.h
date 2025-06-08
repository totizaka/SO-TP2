#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>
#include <defs.h>
#include <mm_dummy.h>
#include <pcb.h>
#include <kernel.h>
#include <interrupts.h>
#include <shared_defs.h>
#include <pipe.h>
#include <scheduler.h>

#define MAX_PID 50

typedef int ( *main_function ) ( char ** argv, uint64_t argc );

int64_t find_free_pcb();
uint64_t new_process(uint64_t rip, uint8_t priority, char ** argv, uint64_t argc, int8_t background, fd_t fds[FD_MAX]);
int64_t block_process(uint64_t pid);
int64_t ready_process(uint64_t pid);
int64_t kill_process(uint64_t pid);
int64_t get_pid();
int64_t nice(int64_t pid, uint8_t new_prio);
uint64_t load_stack(uint64_t rip, uint64_t rsp,  char ** argv, uint64_t argc, uint64_t pid);
PCB* get_pcb(uint64_t pid);
void list_processes();
void set_idle(uint64_t rip, uint8_t priority, char ** argv, uint64_t argc);
process_info_list * get_all_processes();
void ctrl_c_handler();
void ctrl_d_handler();
int64_t piping(pid_t pid, fd_t fds[]);
pid_t wait(pid_t pid, int64_t *ret);
int64_t kill_process_no_yield(uint64_t pid);

#endif