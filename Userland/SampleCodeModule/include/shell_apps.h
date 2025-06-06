#ifndef _SHELL_APPS_H_
#define _SHELL_APPS_H_

#include <stdint.h>

void shell_mem();
void shell_ps();
void shell_loop();
void shell_kill(int64_t pid);
void shell_nice(int64_t pid, int64_t new_prio);//ver si tenemos en userland un enum de prios??
void shell_block(int64_t pid);
void shell_unblock(int64_t pid);
void shell_cat();
void shell_wc();
void shell_filter();

#endif
