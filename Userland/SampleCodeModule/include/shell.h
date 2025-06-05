#ifndef _SHELL_H_
#define _SHELL_H_

#include <stdio.h>
#include <stdlib_user.h>
#include <snake.h>

#define menuDIM 11
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


#endif