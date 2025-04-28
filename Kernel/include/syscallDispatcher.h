#ifndef _SYSCALL_DISPATCHER_H_
#define _SYSCALL_DISPATCHER_H_
#include <stdint.h>
#include <naiveConsole.h>
#include <time.h>
#include <videoDriver.h>

static uint64_t syscall_write_handler(int fd, char *buffer, uint64_t length);
static uint64_t syscall_read_handler(int fd, char *buffer);
static uint64_t syscall_time_handler();
static void syscall_drawPixel_handler(uint32_t color, uint64_t x, uint64_t y); 
static void syscall_drawSquare_handler(uint32_t color, uint64_t x, uint64_t y, uint64_t thickness);
static uint64_t syscall_getWidth_vd_handler();
static uint64_t syscall_getHeight_vd_handler();
static void syscall_sleep_handler(uint64_t secs);
static uint8_t syscall_seconds_handler();
static void syscall_beep_handler(int secs, int freq);

#endif  