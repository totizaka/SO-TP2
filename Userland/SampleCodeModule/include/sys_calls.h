#ifndef _SYSCALLS_H_
#define _SYSCALLS_H_

#include <stdint.h>

uint64_t syscall_read(uint64_t fd, char* buf);

uint64_t syscall_write(uint64_t fd, const char *buf, uint64_t count);

uint64_t syscall_time();
uint64_t syscall_sleep();

void syscall_beep();

#endif