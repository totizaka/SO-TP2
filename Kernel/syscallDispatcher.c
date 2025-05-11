#include <stdint.h>
#include <naiveConsole.h>
#include <time.h>
#include <videoDriver.h>
#include "speaker.h"
#include <syscallDispatcher.h>
#include "lib.h"
#include "keyboard.h"

#define STDIN  0
#define STDOUT 1
#define BLACK 0x000000


static void syscall_write_handler(int fd, char *buffer, uint64_t length);
static uint64_t syscall_read_handler(int fd, char *buffer);
static uint64_t syscall_time_handler();
static void syscall_drawPixel_handler(uint32_t color, uint64_t x, uint64_t y);
static void syscall_drawSquare_handler(uint32_t color, uint64_t x, uint64_t y, uint64_t thickness);
static uint64_t syscall_getWidth_vd_handler();
static uint64_t syscall_getHeight_vd_handler();
static void syscall_sleep_handler(uint64_t secs);
static void syscall_paintAll_vd_handler(uint32_t hexColor);
static void syscall_erraseChar_handler(uint32_t hexColor);
static void syscall_increaseFS_handler();
static void syscall_decreaseFS_handler();
static void syscall_setPixelSize_handler(uint8_t size);
static uint8_t syscall_getCurrentPixelSize_handler();
static void syscall_erraseLine_handler();
static void syscall_beep_handler( int secs, int frec );
static uint64_t syscall_regs_values(uint64_t *regs);
static void* syscall_malloc(size_t memoryToAllocate);
static void syscall_free();

extern uint64_t reg_shot_available;
extern uint64_t data_regs[18];

// Array de punteros a funciones que reciben los mismos argumentos
void (*syscalls_arr[])(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8) = {(void*)syscall_read_handler, (void*)syscall_write_handler, (void*)syscall_time_handler,
    (void*)syscall_drawPixel_handler, (void*)syscall_drawSquare_handler, (void*)syscall_getWidth_vd_handler, (void*)syscall_getHeight_vd_handler, (void*)syscall_sleep_handler, (void*)syscall_paintAll_vd_handler,
    (void*)syscall_erraseChar_handler, (void*)syscall_increaseFS_handler, (void*)syscall_decreaseFS_handler, (void*)syscall_setPixelSize_handler, (void*)syscall_getCurrentPixelSize_handler, (void*)syscall_erraseLine_handler, (void*)syscall_beep_handler, 
    (void*)syscall_regs_values, (void*)syscall_malloc, (void*)syscall_free};

void syscallDispatcher(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t rax) {
    
    if (rax < (sizeof(syscalls_arr) / sizeof(syscalls_arr[0])) && syscalls_arr[rax]!=0) {
        syscalls_arr[rax](rdi, rsi, rdx, r10, r8);
    }
    return;

}

static void syscall_write_handler(int fd, char *buffer, uint64_t length) {

    drawWord(0xffffff, buffer);
}

static uint64_t syscall_read_handler(int fd, char *buffer){

    if (fd != 0){
        return -1;
    }

    *buffer = getCharPressed();
    return 0;
}

static uint64_t syscall_time_handler(){
    return getCurrentTimeBinary();
}

static void syscall_drawPixel_handler(uint32_t color, uint64_t x, uint64_t y){
    putPixel(color, x, y);
}

static void syscall_drawSquare_handler(uint32_t color, uint64_t x, uint64_t y, uint64_t thickness){
    putSquarePixel(color, x, y, thickness);
}

static uint64_t syscall_getWidth_vd_handler(){
    return getWidth_vd();
}

static uint64_t syscall_getHeight_vd_handler(){
    return getHeight_vd();
}

static void syscall_sleep_handler(uint64_t secs){
    sleep(secs);
}

static void syscall_paintAll_vd_handler(uint32_t hexColor){
    paintAll_vd(hexColor);
}

static void syscall_erraseChar_handler(uint32_t hexColor){
    erraseChar(hexColor);
}

static void syscall_increaseFS_handler(){
    increasePixelSize();
}

static void syscall_decreaseFS_handler(){
    decreasePixelSize();
}

static void syscall_setPixelSize_handler(uint8_t size){
    setPixelSize(size);
}

static uint8_t syscall_getCurrentPixelSize_handler(){
    return getCurrentPixelSize();
}

static void syscall_erraseLine_handler(){
    erraseLine();
}

static void syscall_beep_handler(int secs, int freq){
    beep(secs, freq);
}

static uint64_t syscall_regs_values(uint64_t *regs){
    if(!reg_shot_available){
        return 0;
    }
   for(int i=0; i<18; i++){
    regs[i]=data_regs[i];
   }

    return 1;
}

// Hacerlo

// static void* syscall_malloc(size_t memoryToAllocate){
//     return my_malloc((MemoryManagerADT)0x500000, memoryToAllocate);
// }

// static void syscall_free(){
//     my_free((MemoryManagerADT)0x500000);
//     return;
// }