#include <stdint.h>
#include "list.h"
#include <process.h>
#define  QUANTUM 5 //Nose cuanto hacer el quanttum cambiarllo dsp . Entre 20ms y 50ms es razonable 

//Maxima prioridad =0 

//Round Robin 
//


//Listas(habria que mandarlas a share memory) 
list_adt readys;
list_adt blockeds;

static int initialized=0;

int compare_elem(list_elem_t e1, list_elem_t e2){
    return ((PCB*)e1)->pid - ((PCB*)e2)->pid;
}


void iniatialize_lists(){
    t_cmp cmp= compare_elem;

    readys=new_list(cmp);
    blockeds=new_list(cmp);
    initialized=1;
    to_begin(readys);
}


//IDEA: tener un quantum y multiplicarlo por la prioridad 
//LISTA DE PROCESOS READY

//En la clase dijieron: armar lista de los readys y lista de los blocked y pasar por los readys 

uint64_t scheduler(uint64_t current_rsp){

    static PCB *running = NULL;

    if(!initialized){
        iniatialize_lists();
    }

    
    //Guardamos el contexto

    if(running != NULL){
        running->rsp = current_rsp;
        if(running->state == ready){
            add_list(readys, running);
        }
    }

    //Vemos si no hay procesos listos

    if(is_empty(readys)){
        //proceso q hace hlt falta hacerlo
        return current_rsp; 
    }


    if(!has_next(readys)){
        to_begin(readys); //lista circular
    }

    running = (PCB*) next(readys);

    return running->rsp;
}