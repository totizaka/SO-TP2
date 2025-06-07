#ifndef _SHELL_H_
#define _SHELL_H_

#include <stdio.h>
#include <stdlib_user.h>
#include <snake.h>
#include <philos.h>

#define menuDIM 13
#define BLACK 0x000000

typedef struct module {
    char *name;
    void (*function)();
} module;

void help();
void snake();
void show_regs();
void font_size();
void printTime(); 
void div0_exc();
void opcode_exc();
void show_time();
extern void div0();
extern void opcode();
void command_wait();
void shell();
void mm_test_shell();
void prio_test_shell();
void proc_test_shell();
void ps();
void sync_test_shell();
void no_sync_test_shell();
void show_mem_state();
void shell_kill(int64_t pid);
void shell_nice(int64_t pid, int64_t new_prio);//ver si tenemos en userland un enum de prios??
void shell_block(int64_t pid);
void shell_unblock(int64_t pid);
void philos_shell();

#endif