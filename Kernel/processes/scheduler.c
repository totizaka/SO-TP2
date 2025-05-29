#include <scheduler.h>


list_adt readys;
list_adt blockeds;

//proceso actual
PCB *running = NULL;
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
        return current_rsp;
    }
    if(running != NULL){
        running->rsp = current_rsp;
    }
    if(is_empty(readys) && !is_idle){
        //Si no existen procesos para correr => corre el idle 
        idle=get_idle();
        is_idle++;
        running = idle;
        idle->time = UINT64_MAX; // El idle no tiene quantum, corre indefinidamente
        return idle->rsp;
    }
    if(running == NULL || running->state != RUNNING || running == idle){ 
        running = next(readys);
        running->time = QUANTUM * (running->priority);
        running->state = RUNNING; // Aseguramos que el nuevo proceso esté en estado RUNNING
        return running->rsp;
    }
    if (--running->time == 0) {
        // Se acabó su quantum, lo re-encolamos si no terminó
        if (running->state == RUNNING) {
            ready(running);  // volver a ponerlo en cola
        }
        running = next(readys);
        running->time = QUANTUM * (running->priority);
        running->state = RUNNING; // Aseguramos que el nuevo proceso esté en estado RUNNING
        return running->rsp;
    }
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
