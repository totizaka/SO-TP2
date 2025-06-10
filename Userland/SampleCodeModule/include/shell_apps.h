#ifndef _SHELL_APPS_H_
#define _SHELL_APPS_H_

#define MYEOF 0

#include <stdint.h>
#include <stdlib_user.h>
#include <snake.h>

#define MAX_LINE 128
#define BLACK 0x000000

// SO
void shell_loop(char** argv, uint64_t argc);
void shell_cat();
void shell_wc();
void shell_filter();
void my_ps();
void my_mem_state();

//Arqui
void clear_screen();
void invalid_comand();
void opcode_exc();
void div0_exc();
void show_regs();
void show_time();
void font_size();
void snake();
extern void opcode();
extern void div0();
#endif
