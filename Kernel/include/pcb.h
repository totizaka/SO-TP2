#ifndef PCB_H_
#define PCB_H_

#include <stdint.h>

typedef enum{
    FREE,
    READY,
    BLOCKED,
    RUNNING,
    ZOMBIE
}process_state_t;

typedef struct PCB{
    uint64_t pid, ppid; //si se puede usar libreria sys/type ponemos pid_t directo
    uint64_t rsp; 
    uint64_t rip; //desp ver si va a estar en el stack - ver desp si es un void*
    process_state_t state; //ready, blocked o free
    uint8_t priority; //no c si esta ok el tipo de dato, q escala de prioridades vamos a tener????
    char name[32]; //nombre del proceso
    char** args;
    int time;//Lo que queda para ejecutar
    int ret;
}PCB;

#endif