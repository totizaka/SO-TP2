
#include <stdio.h>
#include <stdint.h>
#include <sys_calls.h>

#define MAXBUFF 100

void print(const char* buf, uint64_t count);
uint64_t itoa(uint64_t number, char* s);
uint32_t uint_to_base(uint64_t value, char * buffer, uint32_t base);
void get_time(char* buffer);
void print_time();
uint64_t get_seconds();
char get_char_user();
uint64_t get_width_vd();
uint64_t get_height_vd();

void draw_pixel(uint32_t color, uint64_t x, uint64_t y);
void draw_square(uint32_t color, uint64_t x, uint64_t y, uint64_t thickness);
void nano_sleep(uint64_t secs);
void err_print(char* buff, int count);
int strlen_user(char * str);
void get_string(char* buff, int count);
int strcmp_user(char * s1, char * s2);


// static uint32_t uint_to_base(uint64_t value, char * buffer, uint32_t base);
// void printBase(uint64_t value, uint32_t base);
// void printDec(uint64_t value);
// void printHex(uint64_t value);
char get_char_user();
void paint_all_vd(uint32_t hexColor);
void errase_char(uint32_t hexColor);
void increaseFontSize();
void decreaseFontSize();
void set_pixel_size();
uint8_t get_current_pixel_size();
void errase_line();
void make_beep(int secs, int freq);
uint64_t register_snapshot(uint64_t * regs);

int64_t my_getpid();
int64_t my_create_process(uint64_t rip, uint8_t priority, char ** argv, uint64_t argc);
int64_t my_nice(uint64_t pid, uint64_t new_prio);
int64_t my_kill(uint64_t pid);
int64_t my_yield();
int64_t my_wait(uint64_t pid, int64_t *ret);
int64_t my_block(uint64_t pid);
int64_t my_unblock(uint64_t pid);
void* my_malloc(uint64_t size);
void my_free(uint64_t ptr);
int64_t sem_open ( int64_t sem_id, int value);
int64_t sem_post ( int64_t sem_id );
int64_t sem_wait ( int64_t sem_id);
int64_t sem_close ( int64_t sem_id);
void my_ps();
void my_free_ps();
char *my_strcpy(char *dest, const char *src);
int64_t exit_proc(uint64_t res, uint64_t pid);
