#ifndef _SHELL_H_
#define _SHELL_H_

#include <stdio.h>
#include <stdlib_user.h>
#include <philos.h>
#include <shell_apps.h>
#include <test_mm.h>
#include <test_prio.h>
#include <test_processes.h>
#include <test_sync.h>

#define menuDIM 15
#define BLACK 0x000000
#define BUILTIN 1
#define NOT_BUILTIN 0

#define IS_BACKGROUND 1
#define IS_FOREGROUND 0

typedef struct {
    char name[64];
    char *args[8];
    int argc;
} parsed_command;

typedef struct module {
    char *name;
    void (*function)();
    int is_builtin;
} module;

typedef int (*cmd_func_t)(char **args, int argc);

typedef struct {
    char *name;
    cmd_func_t func;
} special_command_t;


void help();
void shell();
void command_wait();
void shell_kill(char ** argv, uint64_t argc);
void shell_nice(char ** argv, uint64_t argc);
void shell_block(char ** argv, uint64_t argc);
void shell_unblock(char ** argv, uint64_t argc);
void philos_shell();
void write_process_test();
void read_process_test();

#endif