// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
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

    (void*)syscall_read, 
    (void*)syscall_write,
    (void*)syscall_time_handler,
    (void*)syscall_draw_pixel_handler, 
    (void*)syscall_draw_square_handler, 
    (void*)syscall_get_width_vd_handler, 
    (void*)syscall_get_height_vd_handler, 
    (void*)syscall_sleep_handler, 
    (void*)syscall_paint_all_vd_handler,
    (void*)syscall_errase_char_handler, //10
    (void*)syscall_increase_FS_handler, 
    (void*)syscall_decrease_FS_handler, 
    (void*)syscall_setPixelSize_handler, 
    (void*)syscall_get_current_pixel_size_handler, 
    (void*)syscall_errase_line_handler, 
    (void*)syscall_beep_handler, 
    (void*)syscall_regs_values, 
    (void*)syscall_my_getpid_handler, 
    (void*)syscall_my_create_process_handler, 
    (void*)syscall_my_nice_handler, //20, 0x13
    (void*)syscall_my_kill_handler, 
    (void*)syscall_my_block_handler, 
    (void*)syscall_my_unblock_handler, 
    (void*)syscall_my_sem_open_handler, 
    (void*)syscall_my_sem_wait_handler, 
    (void*)syscall_my_sem_post_handler, 
    (void*)syscall_my_sem_close_handler, 
    (void*)syscall_my_yield_handler, 
    (void*)syscall_my_wait_handler,
    (void*)syscall_malloc_handler, //30, 0x1D
    (void*)syscall_free_handler, 
    (void*)syscall_get_processes_handler,
    (void*)syscall_free_processses_handler,
    (void*)syscall_my_mem_state_handler,
    (void*)syscall_my_free_mem_state,
    (void*)syscall_my_sem_open_get_id_handler,
    (void*)syscall_create_pipe,
    (void*)syscall_open_pipe,
    (void*)syscall_write_pipe ,
    (void*)syscall_read_pipe,//40
    (void*)syscall_close_pipe,//41
    (void*)syscall_get_available_pipe_id
};

void syscallDispatcher(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t rax) {
    
    if (rax < (sizeof(syscalls_arr) / sizeof(syscalls_arr[0])) && syscalls_arr[rax]!=0) {
        syscalls_arr[rax](rdi, rsi, rdx, r10, r8);
    }
    return;

}


int64_t syscall_read (int64_t fd, char* buffer, int num_bytes) {
    PCB* running = get_running();

    if (fd < 0 || fd >= FD_MAX || running->fd[fd] == -1)
        return -1;

    target_t target = running->fd[fd];

    if (target == STDOUT || target == STDERR)
        return -1;

    if (target == STDIN) {
        return read_stdin(buffer, num_bytes);
        }
    else {
        return syscall_read_pipe(target, buffer, num_bytes);
    }
}

int64_t syscall_write (int64_t fd, char* buffer, int num_bytes) {
    PCB* running = get_running();
    if (fd < 0 || fd >= FD_MAX || running->fd[fd] == -1){
        return -1;
    }

    target_t target = running->fd[fd];
    if (target == STDOUT || target == STDERR) {
        draw_word(0xffffff, buffer);
        return num_bytes;

    } else if (target == STDIN) {
        return -1; // No se puede escribir en STDIN
    } else {
        return syscall_write_pipe(target, buffer, num_bytes);
    }
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

static int64_t syscall_my_sem_open_handler(char sem_id, uint64_t initialValue,int16_t id_by_hand){
    return my_sem_open(sem_id, initialValue, 0, id_by_hand);
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

static memory_state* syscall_my_mem_state_handler(memory_manager_adt manager){
    return my_mem_state(get_memory_manager());
}

void syscall_my_free_mem_state(memory_state *state) {
    my_free(get_memory_manager(), state);
}

static int64_t syscall_my_sem_open_get_id_handler(uint64_t initialValue){
    return my_sem_open_get_id(initialValue, 0);
}


void syscall_create_pipe( int64_t id ){
     create_pipe( id );
}

int8_t syscall_open_pipe(int64_t target, int role){
    return open_pipe( target-FD_MAX,  role, get_pid());
}
int64_t syscall_write_pipe(int64_t target, char * buffer,int num_bytes){
    return write_pipe(target-FD_MAX, buffer, num_bytes);
}
int64_t syscall_read_pipe(int64_t target, char * buffer,  int num_bytes){
    return read_pipe(target-FD_MAX, buffer, num_bytes);
}

int8_t syscall_close_pipe(int64_t target){
    return close_pipe(target-FD_MAX, get_pid());
}

int64_t syscall_get_available_pipe_id(){
    return get_available_pipe_id();
}

