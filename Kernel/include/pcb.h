#ifndef PCB_H_
#define PCB_H_

#include <stdint.h>
#include <shared_defs.h>

typedef struct PCB{
    pid_t pid, ppid; //si se puede usar libreria sys/type ponemos pid_t directo
    uint64_t rsp; 
    uint64_t stack_base;
    uint64_t rip; //desp ver si va a estar en el stack - ver desp si es un void*
    process_state_t state; //ready, blocked o free
    uint8_t priority; //no c si esta ok el tipo de dato, q escala de prioridades vamos a tener????
    char* name; //nombre del proceso
    char** args;
    int argc; //cantidad de argumentos
    struct PCB* waiting_me; //Proceso que espera a este proceso
    struct PCB* waiting_for; //Proceso que este proceso espera
    int time;//Lo que queda para ejecutar
    int ret;
    fd_t fd[3];// file descriptors  fd[0]=entrada, fd[1]=salida, fd[2]=error
    int8_t background; // 1 si es un proceso en background, 0 si es foreground
}PCB;

#endif