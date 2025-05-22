#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include <stdint.h>
#include <list.h>
#include <process.h>

#define  QUANTUM 5 //Nose cuanto hacer el quanttum cambiarllo dsp . Entre 20ms y 50ms es razonable 
//Maxima prioridad =0 

//Round Robin 

//Listas(habria que mandarlas a share memory)
//(Por ahi no es necesario) 
list_adt readys;
list_adt blockeds;


static PCB * idle_pcb;
static int initialized=0;

void ready(PCB *process);
void block(PCB* process);
PCB* get_running();

#endif