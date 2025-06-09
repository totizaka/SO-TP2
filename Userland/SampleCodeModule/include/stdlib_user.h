#ifndef STD_LIB_USER_H
#define STD_LIB_USER_H

#include <stdio.h>
#include <stdint.h>
#include <sys_calls.h>
#include <shared_defs.h>

#define MAXBUFF 200

typedef struct comands_pipe {
    char cm1[MAXBUFF];
    char cm2[MAXBUFF];
    int pipe; //flag si hay un pipe o no 
    int background; //flag si hay un background o no
}comands_pipe;

typedef struct comand_background {
    char cm[MAXBUFF];
    int background; //flag si hay un background o no
}comand_background;


// my_syscalls wrappers some functions
void make_beep(int secs, int freq);
uint64_t register_snapshot(uint64_t * regs);
void nano_sleep(uint64_t secs);

// my_syscalls wrappers videoSize functions
uint64_t get_width_vd();
uint64_t get_height_vd();

// my_syscalls wrappers time functions
void get_time(char* buffer);
void print_time();
uint64_t get_seconds();

// my_syscalls wrappers write/read functions
void print(const char* buf, uint64_t count);
void err_print(char* buff, int count);
void errase_line();
void errase_char(uint32_t hexColor);
void paint_all_vd(uint32_t hexColor);
void draw_pixel(uint32_t color, uint64_t x, uint64_t y);
void draw_square(uint32_t color, uint64_t x, uint64_t y, uint64_t thickness);
char get_char_user();

// my_syscalls wrappers font size functions
void increaseFontSize();
void decreaseFontSize();
void set_pixel_size();
uint8_t get_current_pixel_size();

// my_syscalls wrappers memory functions
void* my_malloc(uint64_t size);
void my_free(uint64_t ptr);
void my_free_mem_state(memory_state *state);

// my_syscalls wrappers process functions
int64_t my_getpid();
int64_t my_create_process(uint64_t rip, char ** argv, uint64_t argc, int8_t background, fd_t fds[FD_MAX]);
int64_t my_nice(uint64_t pid, uint64_t new_prio);
int64_t my_kill(uint64_t pid);
int64_t my_yield();
int64_t my_wait(uint64_t pid, int64_t *ret);
int64_t my_block(uint64_t pid);
int64_t my_unblock(uint64_t pid);
int64_t exit_proc(uint64_t res, uint64_t pid);
void my_free_ps(process_info_list *plist);

// my_syscalls wrappers semaphores functions
int64_t sem_open ( int64_t sem_id, int value);
int64_t sem_open_get_id(int value);
int64_t sem_post ( int64_t sem_id );
int64_t sem_wait ( int64_t sem_id);
int64_t sem_close ( int64_t sem_id);

// my_syscalls wrappers pipes functions
void my_create_pipe(int64_t id);
int8_t my_open_pipe(int64_t target, int role);
int8_t my_close_pipe(int64_t target);
int64_t my_get_available_pipe_id();
// int64_t my_write_pipe(int64_t target, int * buffer,int num_bytes);
// int64_t my_read_pipe(int64_t target, int * buffer,  int num_bytes);
comands_pipe get_comands_pipe(char* input);

// my_read/write functions
int64_t my_read(int64_t fd, char* buffer, int num_bytes);
int64_t my_write(int64_t fd, char* buffer, int num_bytes);

//my_str functions
int my_strlen(char * str);
void my_get_string(char* buff, int count);
int my_strcmp(char * s1, char * s2);
int my_strcpy(char *dest, const char *src);
void my_strcat(char *dest, const char *src);
char * my_strtok(char *str, const char *delim);
char * my_strchr(const char *s, int c);
char * my_strncpy(char *dest, const char *src, int n);

// my number managers functions
uint64_t itoa(uint64_t number, char* s);
uint32_t uint_to_base(uint64_t value, char * buffer, uint32_t base);

// my_get_comands functions
comand_background get_comand_background(char* input);
comands_pipe get_comand_background_pipe(char* input);

//borrar

void t_a();

#endif