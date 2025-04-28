#ifndef _SHELL_H_
#define _SHELL_H_

#include <stdio.h>

#include <stdlib_user.h>
#include "snake.h"

#define menuDIM 7
#define MAXBUFF 100
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
void div0Exc();
void opcodeExc();
void showTime();
extern void div0();
extern void opcode_exc();

extern module menu[];

extern uint64_t *regs;
extern char *regstxt[18];

void command_wait();
void shell();


#endif