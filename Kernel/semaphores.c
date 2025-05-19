#include <semaphores.h>

#define MAX_SEMS 50
typedef struct sem{
    uint64_t id;
    uint8_t block;
    int64_t value;

}sem_t;

sem_t sem_array[MAX_SEMS]={0};



uint64_t realese(sem_t * sem){
    return 1;
}
uint64_t acquire(sem_t * sem){
    return 1;
}

int64_t my_sem_open ( int64_t sem_id, int value, uint8_t is_kernel );

int64_t my_sem_post ( int64_t sem_id, uint8_t is_kernel ){
    if(sem_id>MAX_SEMS || sem_id<0){
        return -1;
    }
    acquire(&sem_array[sem_id]);
    sem_array[sem_id].value++;


    //falta parte de desbloquear procesos¿? dios proveera

    realese(&sem_array[sem_id]);
    return 1;//por ahi hacer void??

}
int64_t my_sem_wait ( int64_t sem_id, uint8_t is_kernel ){
    if(sem_id>MAX_SEMS || sem_id<0){
        return -1;
    }
    acquire(&sem_array[sem_id]);

    block_process(running->pid);//falta hacer tipo "cola" de procesos q se van bloqueando LOL ¿?

    realese(&sem_array[sem_id]);
    return 1;//hacer void??
}
int64_t my_sem_close ( int64_t sem_id, uint8_t is_kernel );
