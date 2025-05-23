#include <stdint.h>
#include <naiveConsole.h>
#include <time.h>
#include <videoDriver.h>
#include <speaker.h>
#include <syscallDispatcher.h>
#include <lib.h>
#include <keyboard.h>
#include <kernel.h>


#define STDIN  0
#define STDOUT 1
#define BLACK 0x000000


extern uint64_t reg_shot_available;
extern uint64_t data_regs[18];

// Array de punteros a funciones que reciben los mismos argumentos
void (*syscalls_arr[])(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8) = {

    (void*)syscall_read_handler, 
    (void*)syscall_write_handler, 
    (void*)syscall_time_handler,
    (void*)syscall_draw_pixel_handler, 
    (void*)syscall_draw_square_handler, 
    (void*)syscall_get_width_vd_handler, 
    (void*)syscall_get_height_vd_handler, 
    (void*)syscall_sleep_handler, 
    (void*)syscall_paint_all_vd_handler,
    (void*)syscall_errase_char_handler, 
    (void*)syscall_increase_FS_handler, 
    (void*)syscall_decrease_FS_handler, 
    (void*)syscall_setPixelSize_handler, 
    (void*)syscall_get_current_pixel_size_handler, 
    (void*)syscall_errase_line_handler, 
    (void*)syscall_beep_handler, 
    (void*)syscall_regs_values, 
    (void*)syscall_my_getpid_handler, 
    (void*)syscall_my_create_process_handler, 
    (void*)syscall_my_nice_handler, 
    (void*)syscall_my_kill_handler, 
    (void*)syscall_my_block_handler, 
    (void*)syscall_my_unblock_handler, 
    (void*)syscall_my_sem_open_handler, 
    (void*)syscall_my_sem_wait_handler, 
    (void*)syscall_my_sem_post, 
    (void*)syscall_my_sem_close, 
    (void*)syscall_my_yield_handler, 
    (void*)syscall_my_wait_handler,
    (void*)syscall_malloc_handler, 
    (void*)syscall_free_handler, 
};

void syscallDispatcher(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t rax) {
    
    if (rax < (sizeof(syscalls_arr) / sizeof(syscalls_arr[0])) && syscalls_arr[rax]!=0) {
        syscalls_arr[rax](rdi, rsi, rdx, r10, r8);
    }
    return;

}

static void syscall_write_handler(int fd, char *buffer, uint64_t length) {

    draw_word(0xffffff, buffer);
}

static uint64_t syscall_read_handler(int fd, char *buffer){

    if (fd != 0){
        return -1;
    }

    *buffer = get_char_pressed();
    return 0;
}

static uint64_t syscall_time_handler(){
    return get_current_time_binary();
}

static void syscall_draw_pixel_handler(uint32_t color, uint64_t x, uint64_t y){
    put_pixel(color, x, y);
}

static void syscall_draw_square_handler(uint32_t color, uint64_t x, uint64_t y, uint64_t thickness){
    put_square_pixel(color, x, y, thickness);
}

static uint64_t syscall_get_width_vd_handler(){
    return get_width_vd();
}

static uint64_t syscall_get_height_vd_handler(){
    return get_height_vd();
}

static void syscall_sleep_handler(uint64_t secs){
    sleep(secs);
}

static void syscall_paint_all_vd_handler(uint32_t hexColor){
    paint_all_vd(hexColor);
}

static void syscall_errase_char_handler(uint32_t hexColor){
    errase_char(hexColor);
}

static void syscall_increase_FS_handler(){
    increase_pixel_size();
}

static void syscall_decrease_FS_handler(){
    decrease_pixel_size();
}

static void syscall_setPixelSize_handler(uint8_t size){
    set_pixel_size(size);
}

static uint8_t syscall_get_current_pixel_size_handler(){
    return get_current_pixel_size();
}

static void syscall_errase_line_handler(){
    errase_line();
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

static int64_t syscall_my_getpid_handler(){
    return get_pid();
}

extern int64_t syscall_my_create_process_handler(char *name, uint64_t argc, char *argv[]){
    //return new_process();
    //rip??? priority???
}
extern int64_t syscall_my_nice_handler(uint64_t pid, uint64_t new_prio){
    return nice(pid, new_prio);
}
extern int64_t syscall_my_kill_handler(uint64_t pid){
    return kill_process(pid); //hay q implementar
}

//a ambas funciones les falta el manejo de la cola para el scheduler
extern int64_t syscall_my_block_handler(uint64_t pid){
    return block_process(pid);
}
extern int64_t syscall_my_unblock_handler(uint64_t pid){
        return ready_process(pid);
}
extern int64_t syscall_my_sem_open_handler(char *sem_id, uint64_t initialValue){
    return my_sem_open(sem_id[0], initialValue, 0);
}
extern int64_t syscall_my_sem_wait_handler(char *sem_id){
    return my_sem_wait(sem_id[0], 0);
}
extern int64_t syscall_my_sem_post(char *sem_id){
    return my_sem_post(sem_id[0], 0);
}
extern int64_t syscall_my_sem_close(char *sem_id){
    return my_sem_close(sem_id[0], 0);
}
extern int64_t syscall_my_yield_handler(){
    
}
extern int64_t syscall_my_wait_handler(int64_t pid){

}

static void* syscall_malloc_handler(uint64_t size) {
    return my_malloc(get_memory_manager(), size);
}

static void syscall_free_handler(uint64_t ptr) {
    my_free(get_memory_manager(), (void *)ptr);
}