#include <pipe.h>
#include <stdint.h>

#define READER 0
#define WRITER 1


typedef struct pipe {
    pid_t pids[2] ;//0:reader, 1:writer

    uint64_t id;

    uint64_t sem; 
    uint64_t where_reading; // Indica la posicion de lectura
    uint64_t where_writing; // Indica la posicion de escritura

} pipe_t;


pipe_t pipe_array[MAX_PIPES] = {0}; // Array de pipes. Los arreglo por id 


int invalid_ID_pipe(uint64_t pipe_id){
    return invalid_ID(pipe_id, MAX_PIPES);
}

void initialize_pipes(){
    for (int i = 0; i < MAX_PIPES; i++) {
        pipe_array[i].id = i;
        pipe_array[i].pids[0] = -1; // Inicializa el lector como no asignado
        pipe_array[i].pids[1] = -1; // Inicializa el escritor como no asignado
        pipe_array[i].where_reading = 0;
        pipe_array[i].where_writing = 0;

    }
}

int pipe_read( uint64_t id){
    if (invalid_ID_pipe(id) || pipe_array[id].pids[READER] == -1 || pipe_array[id].pids[READER] != get_running()->pid) {// IDEA:  pipe_array[id].pids[READER] != get_running()->pid chequea que el proceso sea el asignadocomo lector 
        return -1;
    }

    uint64_t written= pipe_array[id].where_writing;
    uint64_t read=0; //bytes leidos


    return read;

}