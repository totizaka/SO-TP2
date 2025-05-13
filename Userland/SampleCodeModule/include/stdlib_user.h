#include <stdio.h>
#include <stdint.h>
#include <sys_calls.h>

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
int strlen(char * str);
void get_string(char* buff, int count);
int strcmp(char * s1, char * s2);


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
