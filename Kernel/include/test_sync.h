#ifndef TEST_SYNC_H
#define TEST_SYNC_H

#include <stdio.h>
#include <videoDriver.h>
#include <scheduler.h>
#include <process.h>
#include <test_util.h>
#include <semaphores.h>

uint64_t test_sync();
uint64_t my_process_inc1();
uint64_t my_process_inc2();

#endif
