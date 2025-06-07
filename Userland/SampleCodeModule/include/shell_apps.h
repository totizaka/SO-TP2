#ifndef _SHELL_APPS_H_
#define _SHELL_APPS_H_

#include <stdint.h>
#include <stdlib_user.h>

void shell_loop(uint8_t segs);
void shell_cat();
void shell_wc();
void shell_filter();

#endif
