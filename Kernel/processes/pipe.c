#include "pipe.h"




pipe_t pipe_array[MAX_PIPES] ; // Array de pipes. Los arreglo por id 


int64_t invalid_ID_pipe(int64_t pipe_id){

    return invalid_ID(pipe_id, MAX_PIPES);
}

int8_t invalid_role(int role){
    return (role!=PIPE_READ && role!=PIPE_WRITE);
}
    

pid_t get_pid_from_pipe(int64_t pipe_id, int role) {
    if (invalid_ID_pipe(pipe_id) || invalid_role(role)) {
        return -1; // ID invalido
    }
    return pipe_array[pipe_id].pids[role];
}

void create_pipe( int64_t id ){
    return;
}


int initialize_pipes(){ //inicializo todos? 
    int i;
    for ( i = 0; i < MAX_PIPES; i++) {
        pipe_array[i].id = i;
        pipe_array[i].pids[PIPE_READ] = -1; // Inicializa el lector como no asignado
        pipe_array[i].pids[PIPE_WRITE] = -1; // Inicializa el escritor como no asignado
        pipe_array[i].available=1;

        pipe_array[i].where_reading = 0;
        pipe_array[i].where_writing = 0;
       
        pipe_array[i].sem_write =-1; 
        pipe_array[i].sem_read = -1; 
        pipe_array[i].mutex =-1; // Inicializa el mutex como no bloqueado
        
    }
    return i;
}

int64_t get_available_pipe_id() {
    for (int i = 0; i < MAX_PIPES; i++) {
        if (pipe_array[i].available && pipe_array[i].pids[PIPE_READ] == -1 && pipe_array[i].pids[PIPE_WRITE] == -1 ) {
            pipe_array[i].available=0;
            return i; // Retorna el primer ID de pipe disponible
        }
    }
    return -1; // No hay pipes disponibles
}



int8_t open_pipe( int64_t id , int role , pid_t pid){

    if (invalid_ID_pipe(id) || invalid_role(role)) {
        return -1; 
    }

    if (pipe_array[id].pids[role] != -1) {
        return -1; // el pipe ya esta abierto por este rol
    }

    if (pipe_array[id].pids[PIPE_READ]==-1 && pipe_array[id].pids[PIPE_WRITE]==-1){
        pipe_array[id].sem_write = my_sem_open_get_id(BUFFER, 1); 
        pipe_array[id].sem_read  = my_sem_open_get_id(0, 1); 
        pipe_array[id].mutex     = my_sem_open_get_id(1, 1);

    }

 
    pipe_array[id].pids[role] = pid; 

    return 0; 
}



int64_t next_in_buffer(int current){
    return (current + 1) % BUFFER;
}


int64_t write_pipe( int64_t id, char* buffer, uint64_t num_bytes ){

    if (invalid_ID_pipe(id) || pipe_array[id].pids[PIPE_WRITE] != get_running()->pid){
        return -1;
    }

    int64_t written = 0; // bytes escritos


    while (written< num_bytes){
        //El pipe no tiene nada implementado por si escribo y nadie lo lee

        if(my_sem_wait(pipe_array[id].sem_write)==-1) //CONTROLO EL ESPACIO EN BUFFER 
            return written;

        

        my_sem_wait(pipe_array[id].mutex);

        int64_t current=pipe_array[id].where_writing;


        pipe_array[id].buffer[current] = buffer[written++];
        pipe_array[id].where_writing = next_in_buffer(current);
        
        
        my_sem_post(pipe_array[id].mutex);

        my_sem_post(pipe_array[id].sem_read);

        }

        //Si el pipe se escribe mas rapido de lo que se lee=>se llena
        //=> Escritura se bloquea (wait ) hasta q se lea 
    return written ;
}


int64_t read_pipe(int64_t id, char* buffer, int num_bytes){
    if (invalid_ID_pipe(id) || pipe_array[id].pids[PIPE_READ] != get_running()->pid){
        return -1;
    }

    int64_t read = 0;

    while (read < num_bytes) {

        // Esperar a que haya algo para leer
        if (my_sem_wait(pipe_array[id].sem_read) == -1)
            return read;

        // Lock mutex
        my_sem_wait(pipe_array[id].mutex);

        // 💡 Verificar si no hay escritor y el buffer quedó vacío
        if (pipe_array[id].pids[PIPE_WRITE] == -1 &&
            pipe_array[id].where_reading == pipe_array[id].where_writing) {
            my_sem_post(pipe_array[id].mutex);
            return read;  // fin de datos
        }

        int64_t current = pipe_array[id].where_reading;
        buffer[read++] = pipe_array[id].buffer[current];
        pipe_array[id].where_reading = next_in_buffer(current);

        my_sem_post(pipe_array[id].mutex);
        my_sem_post(pipe_array[id].sem_write);
    }

    return read;
}



int8_t close_pipe_by_role(int64_t id, pid_t pid, int role  ){
    if (pipe_array[id].pids[role]==pid){
        pipe_array[id].pids[role]=-1;
      return 0;

    }
    return -1;

}


//VERIFICAR EL CLOSE(PUEDE SER QUE NO ESTE CERRANDO TODO CORRECTO ) 
int8_t close_pipe(int64_t id , pid_t current_pid){
    
    if (invalid_ID_pipe(id) ) {
        return -1; 
    }

    int close_read=close_pipe_by_role(id, current_pid, PIPE_READ)==0;
    int close_write=close_pipe_by_role(id, current_pid, PIPE_WRITE)==0;

    if (close_write && pipe_array[id].pids[PIPE_READ] != -1) {
        my_sem_post(pipe_array[id].sem_read);  // Desbloquea lector colgado
    }

   if(pipe_array[id].pids[PIPE_READ]==-1  && pipe_array[id].pids[PIPE_WRITE]==-1 ){
        pipe_array[id].where_reading = 0;
        pipe_array[id].where_writing = 0;

        my_sem_close(pipe_array[id].sem_read);
        my_sem_close(pipe_array[id].sem_write);
        my_sem_close(pipe_array[id].mutex);

        pipe_array[id].available=1;
   }
   return (close_read==0||close_write==0);
}














