#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>


typedef struct {
uint64_t pid, ppid; //si se puede usar libreria sys/type ponemos pid_t directo
void* rsp; 
uint64_t rip;//desp ver si va a estar en el stack
uint8_t state;//ready, blocked o free
uint8_t priority; //no c si esta ok el tipo de dato, q escala de prioridades vamos a tener????
}PCB;

#endif