#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include <stdint.h>

#include <interrupts.h>
#include <list.h>

#define SHELL_PID 1 //PID de la shell, no se puede cambiar
#define QUANTUM 1 //Nose cuanto hacer el quanttum cambiarllo dsp . Entre 20ms y 50ms es razonable 
//Maxima prioridad =0 

//Round Robin 

//Listas(habria que mandarlas a share memory)
//(Por ahi no es necesario) 




int compare_elem(list_elem_t e1, list_elem_t e2);
// void initialize_scheduler(pid_t idle_pid);
void initialize_scheduler(void * shell);
int64_t ready(PCB *process);
int64_t block(PCB* process);
PCB* get_running();
uint64_t scheduler(uint64_t current_rsp);
void yield();
void remove_from_scheduler(PCB* process);
uint64_t itoa(uint64_t number, char* s);
PCB *get_idle();
int64_t block_no_yield(PCB*process);
//PCB* set_idle();

#endif