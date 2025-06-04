#ifndef _SYSCALL_DISPATCHER_H_
#define _SYSCALL_DISPATCHER_H_
#include <stdint.h>
#include <naiveConsole.h>
#include <time.h>
#include <videoDriver.h>
#include <mm_dummy.h>
#include <scheduler.h>
#include <process.h>
#include <semaphores.h>
#include <fd.h>
#include <pipe.h>

static void syscall_write_handler(int fd, char *buffer, uint64_t length);
static uint64_t syscall_read_handler(int fd, char *buffer);
static uint64_t syscall_time_handler();
static void syscall_draw_pixel_handler(uint32_t color, uint64_t x, uint64_t y); 
static void syscall_draw_square_handler(uint32_t color, uint64_t x, uint64_t y, uint64_t thickness);
static uint64_t syscall_get_width_vd_handler();
static uint64_t syscall_get_height_vd_handler();
static void syscall_sleep_handler(uint64_t secs);
static void syscall_beep_handler(int secs, int freq);

static void syscall_paint_all_vd_handler(uint32_t hexColor);
static void syscall_errase_char_handler(uint32_t hexColor);
static void syscall_increase_FS_handler();
static void syscall_decrease_FS_handler();
static void syscall_setPixelSize_handler(uint8_t size);
static uint8_t syscall_get_current_pixel_size_handler();
static void syscall_errase_line_handler();
static uint64_t syscall_regs_values(uint64_t *regs);

// SO

static void* syscall_malloc_handler(size_t memoryToAllocate);
static void syscall_free_handler(uint64_t ptr);
static int64_t syscall_my_getpid_handler();
static int64_t syscall_my_create_process_handler(uint64_t rip, uint8_t priority, char ** argv, uint64_t argc, int64_t fds[FD_MAX]);
static int64_t syscall_my_nice_handler(uint64_t pid, uint64_t newPrio);
static int64_t syscall_my_kill_handler(uint64_t pid);
static void syscall_my_block_handler(uint64_t pid);
static void syscall_my_unblock_handler(uint64_t pid);
static int64_t syscall_my_sem_open_handler(char sem_id, uint64_t initialValue);
static int64_t syscall_my_sem_wait_handler(char sem_id);
static int64_t syscall_my_sem_post_handler(char sem_id);
static int64_t syscall_my_sem_close_handler(char sem_id);
static void syscall_my_yield_handler();
static int64_t syscall_my_wait_handler(int64_t pid, int64_t * ret);
static void syscall_list_processes();

#endif  