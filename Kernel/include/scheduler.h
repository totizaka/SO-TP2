#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include <stdint.h>

#include <interrupts.h>
#include <list.h>
#include <process.h>

#define SHELL_PID 1     // PID de la shell, no se puede cambiar
#define QUANTUM 2       // Tiempo de quantum para el Round Robin

//Round Robin 

int compare_elem(list_elem_t e1, list_elem_t e2);
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
void unblock_waiting_proc(PCB *child);

#endif