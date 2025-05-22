#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include <stdint.h>
#include <list.h>
#include <process.h>
#include <interrupts.h>

#define  QUANTUM 5 //Nose cuanto hacer el quanttum cambiarllo dsp . Entre 20ms y 50ms es razonable 
//Maxima prioridad =0 

//Round Robin 

//Listas(habria que mandarlas a share memory)
//(Por ahi no es necesario) 
list_adt readys;
list_adt blockeds;


static PCB * idle_pcb;
static int initialized=0;
typedef uint64_t pid_t;


int compare_elem(list_elem_t e1, list_elem_t e2);
void initialize_scheduler(pid_t idle_pid);
void ready(PCB *process);
void block(PCB* process);
PCB* get_running();
uint64_t scheduler(uint64_t current_rsp);
void yield();

#endif