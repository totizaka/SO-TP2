#ifndef SEMAPHORES_H_
#define SEMAPHORES_H_
#include <stdint.h>
#include <mm_dummy.h>
#include <scheduler.h>
#include <process.h>
#include <list.h>

void realese(uint8_t * lock);
void acquire(uint8_t * lock);
int64_t my_sem_open ( int64_t sem_id, int value, uint8_t is_kernel );
int64_t my_sem_post ( int64_t sem_id, uint8_t is_kernel );
int64_t my_sem_wait ( int64_t sem_id, uint8_t is_kernel );
int64_t my_sem_close ( int64_t sem_id, uint8_t is_kernel );

#endif