#include <semaphores.h>

#define MAX_SEMS 50
typedef struct sem{

    uint64_t id;
    uint8_t lock;
    int64_t value;  
    uint8_t ocupied;

    uint8_t is_kernel;
    list_adt my_blockeds;

}sem_t;

sem_t sem_array[MAX_SEMS]={0};

int comp(list_elem_t e1, list_elem_t e2){
    // return ((PCB*)e1)->pid - ((PCB*)e2)->pid;
    return 0; //Falta implementacion
}


int invalid_ID(int64_t sem_id){
    return (sem_id < 0 || sem_id >= MAX_SEMS);
}


int64_t my_sem_open ( int64_t sem_id, int value, uint8_t is_kernel ){
     if (invalid_ID(sem_id)) {//ID invalido
        return -1;
    }

    acquire(&sem_array[sem_id].lock);//lockea el semaforo

    if (sem_array[sem_id].ocupied) {//si el semaforo ya esta ocupado
        release(&sem_array[sem_id].lock);
        return -1;
    }

    sem_array[sem_id].id = sem_id;
    sem_array[sem_id].value = value;
    sem_array[sem_id].is_kernel = is_kernel;

    sem_array[sem_id].ocupied = 1;

    sem_array[sem_id].my_blockeds = new_list(comp);


    release(&sem_array[sem_id].lock);

    return 1;
}


// int64_t my_sem_open ( int64_t sem_id, int value, uint8_t is_kernel ){
//     // if(!sem_array[sem_id].ocupied || sem_id<0 || sem_id>MAX_SEMS){
//     //     return -1;
//     // }
//     // acquire(&sem_array[sem_id].lock);
//     // sem_array[sem_id].ocupied=1;
//     // sem_array[sem_id].value=value;
//     // sem_array[sem_id].is_kernel=is_kernel;
//     // release(&sem_array[sem_id].lock);

// }


int64_t my_sem_post ( int64_t sem_id){
if (invalid_ID(sem_id))
             return -1;

    acquire(&sem_array[sem_id].lock);

    if( !sem_array[sem_id].ocupied){
        release(&sem_array[sem_id].lock);
        return -1;
    }

    if (sem_array[sem_id].my_blockeds==NULL) {
        sem_array[sem_id].value++;
    }
     else {
        list_elem_t to_unblock = next(sem_array[sem_id].my_blockeds);
        remove_list(sem_array[sem_id].my_blockeds,to_unblock);
        ready(to_unblock);

    }

    release(&sem_array[sem_id].lock);
    return 1;

}


//int64_t my_sem_post ( int64_t sem_id, uint8_t is_kernel ){
    // if(sem_id>MAX_SEMS || sem_id<0){
    //     return -1;
    // }
    // acquire(&sem_array[sem_id].lock);
    // if(sem_array[sem_id].my_blockeds==NULL||is_empty(sem_array[sem_id].my_blockeds)){
    //     sem_array[sem_id].value++;
    // }else{
    //     list_elem_t unblock=remove_first(sem_array[sem_id].my_blockeds);
    //     ready(unblock);
    // }
    

    // release(&sem_array[sem_id].lock);
    // return 1;//por ahi hacer void??

// }


int64_t my_sem_wait ( int64_t sem_id){
    if (invalid_ID(sem_id)) {//ID invalido
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
        return 1;
    }

    //Caso2: no tengo para disminuir, me bloqueo
    PCB* running = get_running();
    block(running);//bloquea el proceso que esta corriendo

    if (sem_array[sem_id].my_blockeds == NULL) {//Por si no tengo lista de bloqueados
        sem_array[sem_id].my_blockeds = new_list(comp);
    }

    if(add_list(sem_array[sem_id].my_blockeds, running)==-1){//agrega el proceso a la lista de bloqueados (Si no se pudo agregar, devuelve -1)
        release(&sem_array[sem_id].lock);
        return -1;
    }
    release(&sem_array[sem_id].lock);//libera el semaforo
    return 1; //
}


// int64_t my_sem_wait ( int64_t sem_id, uint8_t is_kernel ){
//     // if(sem_id>MAX_SEMS || sem_id<0){
//     //     return -1;
//     // }
//     // acquire(&sem_array[sem_id].lock);
//     // if(sem_array[sem_id].value>0){
//     //     sem_array[sem_id].value--;
//     //     release(&sem_array[sem_id].lock);
//     //     return 1;
//     // }
//     // PCB* running= get_running();
//     // block(running);
//     // if(sem_array[sem_id].my_blockeds==NULL){
//     //     sem_array[sem_id].my_blockeds=new_list(comp);
//     // }
//     // add_list(sem_array[sem_id].my_blockeds, running);
//     // release(&sem_array[sem_id].lock);
//     // return 1;//hacer void??
// }

int64_t my_sem_close ( int64_t sem_id){
    if (invalid_ID(sem_id)) {//ID invalido
        return -1;
    }

    acquire(&sem_array[sem_id].lock);

    if (sem_array[sem_id].my_blockeds != NULL) {
        while (!is_empty(sem_array[sem_id].my_blockeds)) {
            list_elem_t pcb = next(sem_array[sem_id].my_blockeds);
            remove_list(sem_array[sem_id].my_blockeds, pcb);
            ready(pcb);
        }

        
        free_list(sem_array[sem_id].my_blockeds);
        sem_array[sem_id].my_blockeds = NULL;
    }
    sem_array[sem_id].ocupied = 0;
    sem_array[sem_id].value = 0;
    release(&sem_array[sem_id].lock);
    return 0;
}





//int64_t my_sem_close ( int64_t sem_id, uint8_t is_kernel){
    // if(sem_id<0 || sem_id>MAX_SEMS || !sem_array[sem_id].ocupied){
    //     return -1;
    // }
    // acquire(&sem_array[sem_id].lock);
    // //me parece q esta mal esto
    // if(sem_array[sem_id].my_blockeds!=NULL){
    // list_elem_t PCB;
    // while(!is_empty(sem_array[sem_id].my_blockeds)){
    //    PCB= remove_first(sem_array[sem_id].my_blockeds);
    //    ready(PCB);
    // }
    // free_list(sem_array[sem_id].my_blockeds);
    // }
    
    // release(&sem_array[sem_id].lock);

    // return 0;
//}
