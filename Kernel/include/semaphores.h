#ifndef SEMAPHORES_H_
#define SEMAPHORES_H_
#include <stdint.h>
#include <memory_manager.h>
#include <scheduler.h>
#include <process.h>
#include <list.h>

#define MAX_SEM 50 

void release(uint8_t * lock);
void acquire(uint8_t * lock);

int invalid_ID(uint64_t sem_id, int max);
int64_t my_sem_open ( int64_t sem_id, int value, uint8_t is_kernel , int16_t id_by_hand);
int64_t my_sem_post ( int64_t sem_id );
int64_t my_sem_wait ( int64_t sem_id);
int64_t my_sem_close ( int64_t sem_id);

int64_t my_sem_open_get_id ( int value, uint8_t is_kernel );

#endif