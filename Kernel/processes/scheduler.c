#include <scheduler.h>


list_adt readys;
list_adt blockeds;

//proceso actual
PCB *running = NULL;
int64_t ran_time = 0;
static int initialized = 0;
PCB* idle=NULL;
uint8_t is_idle=0;

typedef int ( *main_function ) ( char ** argv, uint64_t argc );

int compare_elem(list_elem_t e1, list_elem_t e2){
    return e1 - e2;
}

//tiene q hacer las dos listas y dsp llamar a set_idle() => en el pcb table en la posicision 0 tenmos el idle 

void initialize_scheduler(){
    draw_word(0xFFFFFF, "Initializing scheduler\n");
    t_cmp cmp = compare_elem;
    readys = new_list(cmp);
    blockeds = new_list(cmp);

    if (readys == NULL || blockeds == NULL) {
        return;
    }
    initialized = 1;
}

void ready(PCB* process){
    if (process == NULL || ready_process(process->pid) == -1){
        return;
    }   
    add_list(readys, (list_elem_t)process);
    remove_list(blockeds, (list_elem_t)process); 
}

void block(PCB* process){
    if (process == NULL || block_process(process->pid) == -1){
        return;
    }
    remove_list(readys, (list_elem_t)process);
    add_list(blockeds, (list_elem_t)process);
}

PCB* get_running(){
    return running;
}

uint64_t scheduler(uint64_t current_rsp){
    if(!initialized){
        //Caso donde no se inicializo el scheduler
        return current_rsp;
    }
    if(running != NULL){
        running->rsp = current_rsp;
    }
    if(is_empty(readys)){
        //Si no existen procesos para correr => corre el idle 
        idle=get_idle();
        return idle->rsp;
    }
    if(running == NULL){ 
        running = next(readys);
        return running->rsp;
    }
    if(ran_time >= running->priority){
        PCB * next_pcb = next(readys);
        ran_time = 0;
        running = next_pcb;
        return next_pcb->rsp;
    }
    ran_time++;
    return current_rsp;
}

void yield(){
    timer_tick();
}

void remove_from_scheduler(PCB* process){
    if (process == NULL){
        return;
    }
    if (process->state == READY){
        remove_list(readys, (list_elem_t)process);
    }
    else if (process->state == BLOCKED){
        remove_list(blockeds, (list_elem_t)process);
    }
}
