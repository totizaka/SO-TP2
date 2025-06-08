#ifndef _SHELL_H_
#define _SHELL_H_

#include <stdio.h>
#include <stdlib_user.h>
#include <snake.h>
#include <philos.h>
#include <shell_apps.h>

#define menuDIM 22
#define BLACK 0x000000
#define BUILTIN 1
#define NOT_BUILTIN 0

typedef struct {
    char name[64];
    char *args[8];
    int argc;
} parsed_command;

typedef struct module {
    char *name;
    void (*function)();
    char **(*arg_preparer)(parsed_command cmd);
    int is_builtin;
} module;

typedef char **(*arg_preparer_t)(parsed_command cmd);

typedef struct {
    char *name;
    arg_preparer_t preparer;
} arg_preparer_map_t;

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
void shell_kill(char ** argv, uint64_t argc);
void shell_nice(char ** argv, uint64_t argc);//ver si tenemos en userland un enum de prios??
void shell_block(char ** argv, uint64_t argc);
void shell_unblock(char ** argv, uint64_t argc);
void test_a();
void write_process_test();
void read_process_test();

void slow_writer_test(void);
void fast_reader_test(void);
void blocking_reader_test(void);
void orphan_writer_test(void);
void eof_reader_test(void);

//void philos_shell();

#endif