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
    (void*)syscall_my_sem_post_handler, 
    (void*)syscall_my_sem_close_handler, 
    (void*)syscall_my_yield_handler, 
    (void*)syscall_my_wait_handler,
    (void*)syscall_malloc_handler, 
    (void*)syscall_free_handler, 
    (void*)syscall_get_processes_handler,
    (void*)syscall_free_processses_handler
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

static int64_t syscall_my_create_process_handler(uint64_t rip,  char ** argv, uint64_t argc, int8_t background, int64_t fds[FD_MAX]){
    return new_process((void(*))rip, 1, argv, argc, background, fds);
}

static int64_t syscall_my_nice_handler(uint64_t pid, uint64_t new_prio){
    return nice(pid, new_prio);
}

static int64_t syscall_my_kill_handler(uint64_t pid){
    return kill_process(pid); 
}

static void syscall_my_block_handler(uint64_t pid){
    block(get_pcb(pid));
}

static void syscall_my_unblock_handler(uint64_t pid){
    ready(get_pcb(pid));
}

//falta implementar correctamente sem funciones
static int64_t syscall_my_sem_open_handler(char sem_id, uint64_t initialValue){
    return my_sem_open(sem_id, initialValue, 0);
}
static int64_t syscall_my_sem_open_get_id_handler(uint64_t initialValue){
    return my_sem_open_get_id(initialValue);
}
static int64_t syscall_my_sem_wait_handler(char sem_id){
    return my_sem_wait(sem_id);
}
static int64_t syscall_my_sem_post_handler(char sem_id){
    return my_sem_post(sem_id);
}
static int64_t syscall_my_sem_close_handler(char sem_id){
    return my_sem_close(sem_id);
}

static void syscall_my_yield_handler(){
    yield();
}

static int64_t syscall_my_wait_handler(int64_t pid, int64_t *ret){
    return wait(pid, ret);
}

static void* syscall_malloc_handler(uint64_t size) {
    return my_malloc(get_memory_manager(), size);
}

static void syscall_free_handler(uint64_t ptr) {
    my_free(get_memory_manager(), (void *)ptr);
}

static process_info_list* syscall_get_processes_handler() {
    return get_all_processes();
}

static void syscall_free_processses_handler(process_info_list *processes) {
    free_process_list(processes);
}

int64_t syscall_read (int64_t fd, char* buffer, int numBytes){
    PCB* running = get_running();
    if (fd!=STDIN) {
        return -1; // Invalid file descriptor
    }

    target_t target = running->fd[fd]; 
    if (target == STDIN) {
        return 0;//Hay que implemntar read_terminal, esto es para q compile
       //return read_terminal(buffer, numBytes); //IMPLEMENTAR ESTA FUNCION 
    } 
    else {
        if (target == STDOUT || target == STDERR) {
            return -1; // No se puede leer desde ahi 
        }
        return pipe_read(target-FD_MAX, buffer, numBytes); //IMPLEMENTAR ESTA FUNCION en Pipe
    }
}

int64_t syscall_write (int64_t fd, char* buffer, int numBytes){
    PCB* running = get_running();
    if (fd!=STDOUT && fd!=STDERR) {
        return -1; // Invalid file descriptor
    }

    target_t target = running->fd[fd]; // Get the target from the file descriptor

    if (target == STDOUT || target == STDERR) {
        return 0;//Hay que implemntar print_stdout, esto es para q compile
       //return print_stdout(buffer, numBytes); //IMPLEMENTAR ESTA FUNCION
    } 
    else {
            if (target == STDIN) {
             return -1; // No se puede leer desde ahi 
        }
       return pipe_write(target-FD_MAX, buffer, numBytes); // IMPLEMENTAR ESTA FUNCION en Pipe
    }
}
