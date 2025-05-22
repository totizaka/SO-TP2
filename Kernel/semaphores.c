#include <semaphores.h>

#define MAX_SEMS 50
typedef struct sem{
    uint64_t id;
    uint8_t lock;
    int64_t value;
    uint8_t ocupied;
    uint8_t is_kernel;
    PCB * blockeds;

}sem_t;

sem_t sem_array[MAX_SEMS]={0};

int comp(list_elem_t e1, list_elem_t e2){
    return ((PCB*)e1)->pid - ((PCB*)e2)->pid;
}


int64_t my_sem_open ( int64_t sem_id, int value, uint8_t is_kernel ){
    if(!sem_array[sem_id].ocupied || sem_id<0 || sem_id>MAX_SEMS){
        return -1;
    }
    acquire(&sem_array[sem_id].lock);
    sem_array[sem_id].ocupied=1;
    sem_array[sem_id].value=value;
    sem_array[sem_id].is_kernel=is_kernel;
    realese(&sem_array[sem_id].lock);

}

int64_t my_sem_post ( int64_t sem_id, uint8_t is_kernel ){
    if(sem_id>MAX_SEMS || sem_id<0){
        return -1;
    }
    acquire(&sem_array[sem_id].lock);
    if(sem_array[sem_id].blockeds==NULL||is_empty(sem_array[sem_id].blockeds)){
        sem_array[sem_id].value++;
    }else{
        list_elem_t unblock=remove_first(sem_array[sem_id].blockeds);
        ready(unblock);
    }
    

    realese(&sem_array[sem_id].lock);
    return 1;//por ahi hacer void??

}
int64_t my_sem_wait ( int64_t sem_id, uint8_t is_kernel ){
    if(sem_id>MAX_SEMS || sem_id<0){
        return -1;
    }
    acquire(&sem_array[sem_id].lock);
    if(sem_array[sem_id].value>0){
        sem_array[sem_id].value--;
        realese(&sem_array[sem_id].lock);
        return 1;
    }
    PCB* running= get_running();
    block(running);
    if(sem_array[sem_id].blockeds==NULL){
        sem_array[sem_id].blockeds=new_list(comp);
    }
    add_list(sem_array[sem_id].blockeds, running);
    realese(&sem_array[sem_id].lock);
    return 1;//hacer void??
}
int64_t my_sem_close ( int64_t sem_id, uint8_t is_kernel){
    if(sem_id<0 || sem_id>MAX_SEMS || !sem_array[sem_id].ocupied){
        return -1;
    }
    acquire(&sem_array[sem_id].lock);
    //me parece q esta mal esto
    if(sem_array[sem_id].blockeds!=NULL){
    list_elem_t PCB;
    while(!is_empty(sem_array[sem_id].blockeds)){
       PCB= remove_first(sem_array[sem_id].blockeds);
       ready(PCB);
    }
    free_list(sem_array[sem_id].blockeds);
    }
    
    realese(&sem_array[sem_id].lock);

    return 0;
}
