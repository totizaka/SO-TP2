#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>
#include <defs.h>
#include <mm_dummy.h>
#include "list.h"
#include <scheduler.h>


// #define zombie 3
// #define blocked 2
// #define ready 1
// #define available 0

#define MAX_PID 50

#define LOW_PRIORITY 0
#define MEDIUM_PRIORITY 1
#define HIGH_PRIORITY 2


typedef enum{
    FREE,
    READY,
    BLOCKED,
    RUNNING,
    ZOMBIE
}process_state_t;

typedef struct {
    uint64_t pid, ppid; //si se puede usar libreria sys/type ponemos pid_t directo
    void* rsp; 
    uint64_t rip; //desp ver si va a estar en el stack - ver desp si es un void*
    process_state_t state; //ready, blocked o free
    uint8_t priority; //no c si esta ok el tipo de dato, q escala de prioridades vamos a tener????
    char name[32]; //nombre del proceso
    char** args;
    int time;//Lo que queda para ejecutar
}PCB;


int64_t find_free_pcb();
uint64_t new_process(uint64_t rip, uint8_t priority, char ** argv, uint64_t argc);
int64_t block_process(uint64_t pid);
int64_t ready_process(uint64_t pid);
int64_t kill_process(uint64_t pid);//FALTA IMPLEMENTAR
int64_t get_pid();
int64_t nice(int64_t pid, uint8_t new_prio);
void*  load_stack(uint64_t rip, uint64_t rsp, uint64_t pid, char ** argv, uint64_t argc);
PCB* get_pcb(uint64_t pid);
PCB* list_processes();//quiza sea mejor directamente printear desde kernel¿?

#endif