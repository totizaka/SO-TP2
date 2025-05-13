#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>

#define blocked 2
#define ready 1
#define free 0
#define MAX_PID 50


typedef struct {
    uint64_t pid, ppid; //si se puede usar libreria sys/type ponemos pid_t directo
    void* rsp; 
    uint64_t rip; //desp ver si va a estar en el stack - ver desp si es un void*
    uint8_t state; //ready, blocked o free
    uint8_t priority; //no c si esta ok el tipo de dato, q escala de prioridades vamos a tener????
    char name[32]; //nombre del proceso
}PCB;


int64_t find_free_pcb();
uint64_t new_process(uint64_t rip, uint8_t priority);
int64_t block_process(uint64_t pid);
int64_t ready_process(uint64_t pid);
int64_t kill_process(uint64_t pid);
int64_t get_pid();
int64_t nice(int64_t pid, uint8_t new_prio);

#endif