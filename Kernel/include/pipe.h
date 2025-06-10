#ifndef _PIPE_H
#define _PIPE_H

#include <semaphores.h>
#include <stdint.h>

#define MAX_PIPES 25
#define BUFFER 1024

#define PIPE_READ 0
#define PIPE_WRITE 1

typedef struct pipe {
    pid_t pids[2];          //0:reader, 1:writer
    int64_t id; 
    int64_t available;
    char buffer[BUFFER];    // Buffer del pipe
    int64_t where_reading;  // Indice lectura
    int64_t where_writing;  // Indice escritura
    int64_t sem_write;      
    int64_t sem_read;       
    int64_t mutex ;        
} pipe_t;

extern pipe_t pipe_array[MAX_PIPES];

int initialize_pipes();
int8_t open_pipe(int64_t id, int role, pid_t pid);
int8_t close_pipe(int64_t id , pid_t current_pid);
int64_t write_pipe(int64_t id, char* buffer, uint64_t num_bytes);
int64_t read_pipe(int64_t id, char* buffer, int num_bytes);
pid_t get_pid_from_pipe(int64_t pipe_id, int role);
void create_pipe( int64_t id );
int64_t get_available_pipe_id() ;

#endif