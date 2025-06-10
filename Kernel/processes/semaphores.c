#include <semaphores.h>

typedef struct sem{

    uint64_t id;
    uint8_t lock;
    int64_t value;  
    
    uint8_t ocupied;
    uint8_t is_kernel;
    list_adt processes;

}sem_t;

sem_t sem_array[MAX_SEM]={0};

int comp(list_elem_t e1, list_elem_t e2){
    return (e1 - e2); 
}

int invalid_ID(uint64_t num, int max){
    return (num < 0 || num >= max);
}

int invalid_ID_sem(int64_t sem_id){
    return invalid_ID(sem_id, MAX_SEM);
}


//CHEQUEARLES A ESTOS DOS LA SYNCRO 
int64_t find_free_sem(){ 
    
    for(int i=0; i<MAX_SEM; i++){
        acquire(&sem_array[i].lock);
        if(sem_array[i].ocupied==0){
            return i;
        }

        release(&sem_array[i].lock); 

    }
    return -1;
}

int64_t my_sem_open ( int64_t sem_id, int value, uint8_t is_kernel , int16_t id_by_hand){
     if (invalid_ID_sem(sem_id)) {//ID invalido
        return -1;
    }

    if(id_by_hand){ //Singnifica que el id NO fue encontrado entre los libres 
            acquire(&sem_array[sem_id].lock); //lockea el semaforo

    }

    if (sem_array[sem_id].ocupied) { //si el semaforo ya esta ocupado
        sem_array[sem_id].ocupied++;
        release(&sem_array[sem_id].lock);
        return 1;
    }

    sem_array[sem_id].ocupied=1;
    sem_array[sem_id].id = sem_id;
    sem_array[sem_id].value = value;
    sem_array[sem_id].is_kernel = is_kernel;

   sem_array[sem_id].processes = new_list(comp);
    if(sem_array[sem_id].processes==NULL){
        release(&sem_array[sem_id].lock);
        return -1;
    }

    release(&sem_array[sem_id].lock);

    return 1;
}



int64_t my_sem_open_get_id ( int value, uint8_t is_kernel ){
    int64_t sem_id =  find_free_sem();
    if( my_sem_open(sem_id, value, is_kernel, 0)==1){
        return sem_id;
    }else{
        return -1;
    }

}



int64_t my_sem_post ( int64_t sem_id){
if (invalid_ID_sem(sem_id))
             return -1;

    acquire(&sem_array[sem_id].lock);

    if( !sem_array[sem_id].ocupied){
        release(&sem_array[sem_id].lock);
        return -1;
    }

    if (is_empty(sem_array[sem_id].processes)) {
        sem_array[sem_id].value++;
    } else {
        list_elem_t to_unblock = next(sem_array[sem_id].processes);
        remove_list(sem_array[sem_id].processes,to_unblock);
        ready(to_unblock);

    }

    release(&sem_array[sem_id].lock);
    return 0;

}

int64_t my_sem_wait ( int64_t sem_id){
    if (invalid_ID_sem(sem_id)) {//ID invalido
        return -1;
    }
    acquire(&sem_array[sem_id].lock);

    if (!sem_array[sem_id].ocupied) {//si el semaforo no esta ocupado
        release(&sem_array[sem_id].lock);
        return -1;
    }

    //Caso1: tengo para disminuir
    if (sem_array[sem_id].value > 0) {
        sem_array[sem_id].value--;//decrementa el valor del semaforo
        release(&sem_array[sem_id].lock);
        return 0;
    }

    //Caso2: no tengo para disminuir, me bloqueo
    PCB* running = get_running();
 

 
    if(add_list(sem_array[sem_id].processes, running)==-1){//agrega el proceso a la lista de bloqueados (Si no se pudo agregar, devuelve -1)
        release(&sem_array[sem_id].lock);
        return -1;
    }
    block_no_yield(running);//bloquea el proceso que esta corriendo
    release(&sem_array[sem_id].lock);//libera el semaforo
    yield();
    return 0;
}

int64_t my_sem_close ( int64_t sem_id){
    if (invalid_ID_sem(sem_id)) {//ID invalido
        return -1;
    }

    acquire(&sem_array[sem_id].lock);
    if(sem_array[sem_id].ocupied==0){
        release(&sem_array[sem_id].lock);
        return 0;
    }
    --sem_array[sem_id].ocupied;
    if (sem_array[sem_id].processes != NULL && sem_array[sem_id].ocupied==0) {
        while (!is_empty(sem_array[sem_id].processes)) {
            list_elem_t pcb = next(sem_array[sem_id].processes);
            remove_list(sem_array[sem_id].processes, pcb);
            ready(pcb);
        }

        free_list(sem_array[sem_id].processes);
        sem_array[sem_id].processes = NULL;
    }
    sem_array[sem_id].value = 0;
    release(&sem_array[sem_id].lock);
    return 0;
}

