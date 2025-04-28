#include <stdio.h>
#include <stdint.h>
#include <syscall.h>

extern uint64_t syscall_write();
extern uint64_t syscall_time();
extern uint64_t syscall_drawPixel();
extern uint64_t syscall_drawSquare();
extern uint64_t syscall_getWidth_vd();
extern uint64_t syscall_getHeight_vd();

void print(const char* buf, uint64_t count);
uint64_t itoa(uint64_t number, char* s);
uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base);
void getTime(char* buffer);
void printTime();
uint64_t get_seconds();
char getCharUser();
uint64_t getWidth_vd();
uint64_t getHeight_vd();

void drawPixel(uint32_t color, uint64_t x, uint64_t y);
void drawSquare(uint32_t color, uint64_t x, uint64_t y, uint64_t thickness);
void nano_sleep(uint64_t secs);
void err_print(char* buff, int count);
int strlen(char * str);
void getString(char* buff, int count);
int strcmp(char * s1, char * s2);


// static uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base);
// void printBase(uint64_t value, uint32_t base);
// void printDec(uint64_t value);
// void printHex(uint64_t value);
void paintAll_vd(uint32_t hexColor);
void erraseChar(uint32_t hexColor);
void increaseFontSize();
void decreaseFontSize();
void setPixelSize();
uint8_t getCurrentPixelSize();
void erraseLine();
void makeBeep(int secs, int freq);
uint64_t register_snapshot(uint64_t * regs);
