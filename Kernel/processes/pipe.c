#include "pipe.h"
#include <stdint.h>

#define PIPE_READ 0
#define PIPE_WRITE 1
#define BUFFER 1024 // no se cto deberia ser

typedef struct pipe {

    pid_t pids[2] ;//0:reader, 1:writer
    int64_t id; 
    int64_t buffer[BUFFER]; // Buffer del pipe

    int64_t where_reading; // Indica la posicion de lectura
    int64_t where_writing; // Indica la posicion de escritura
    int64_t sem_write; //Bloqueo a la lectura si no hay datos 
    int64_t sem_read; //Bloqueo a la escritura si el buffer esta lleno

    int64_t mutex ; // mutex

} pipe_t;


pipe_t pipe_array[MAX_PIPES] = {0}; // Array de pipes. Los arreglo por id 


int invalid_ID_pipe(int64_t pipe_id){

    return invalid_ID(pipe_id, MAX_PIPES);
}

int invalid_role(int role){
    return (role!=PIPE_READ && role!=PIPE_WRITE);
}
    

pid_t get_pid_from_pipe(int64_t pipe_id, int role) {
    if (invalid_ID_pipe(pipe_id) || invalid_role(role)) {
        return -1; // ID invalido
    }
    return pipe_array[pipe_id].pids[role];
}

void create_pipe( int64_t id ){
        pipe_array[id].id = id;
        pipe_array[id].pids[0] = -1; // Inicializa el lector como no asignado
        pipe_array[id].pids[1] = -1; // Inicializa el escritor como no asignado
        pipe_array[id].where_reading = 0;
        pipe_array[id].where_writing = 0;
       
        pipe_array[id].sem_write = BUFFER; 
        pipe_array[id].sem_read = 0; 
        pipe_array[id].mutex = 0; // Inicializa el mutex como no bloqueado
}


void initialize_pipes(){ //inicializo todos? 
    for (int i = 0; i < MAX_PIPES; i++) {
        create_pipe(i);
    }
}


int pipe_write( int64_t id, char* buffer, uint64_t numBytes ){

    if (invalid_ID_pipe(id) ){
        return -1;
    }
    int written = 0; // bytes escritos
    for(written; written < numBytes; written++) {
    }
        //Si el pipe se escribe mas rapido de lo que se lee=>se llena
        //=> Escritura se bloquea (wait ) hasta q se lea 
    return 1 ;
}



int pipe_read(int64_t id, char* buffer, int numBytes){
    if (invalid_ID_pipe(id) || pipe_array[id].pids[PIPE_READ] == -1 || pipe_array[id].pids[PIPE_READ] != get_running()->pid) {// IDEA:  pipe_array[id].pids[READER] != get_running()->pid chequea que el proceso sea el asignadocomo lector 
        return -1;
    }

    int64_t written= pipe_array[id].where_writing;
    int64_t read=0; //bytes leidos


    return read;

}

int get_available_pipe_id() {
    for (int i = 0; i < MAX_PIPES; i++) {
        if (pipe_array[i].pids[PIPE_READ] == -1 && pipe_array[i].pids[PIPE_WRITE] == -1) {
            return i; // Retorna el primer ID de pipe disponible
        }
    }
    return -1; // No hay pipes disponibles
}

