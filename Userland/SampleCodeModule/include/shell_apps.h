#ifndef _SHELL_APPS_H_
#define _SHELL_APPS_H_

#include <stdint.h>
#include <stdlib_user.h>

#define MAX_LINE 128
#define EOF 0 // Valor especial para Ctrl+D

void shell_loop(char** argv, uint64_t argc);
void shell_cat();
void shell_wc();
void shell_filter();

#endif
