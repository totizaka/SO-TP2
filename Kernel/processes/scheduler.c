#include <scheduler.h>

//proceso actual
PCB *running = NULL;
int64_t ran_time = 0;

int compare_elem(list_elem_t e1, list_elem_t e2){
    return ((PCB*)e1)->pid - ((PCB*)e2)->pid;
}


void initialize_scheduler(pid_t idle_pid){
    t_cmp cmp = compare_elem;
    readys = new_list(cmp);
    blockeds = new_list(cmp);

    if (readys == NULL || blockeds == NULL) {
        return;
    }

    idle_pcb = get_pcb(idle_pid);
    initialized = 1;
    to_begin(readys);
}

void ready(PCB* process){
    if (process == NULL || ready_process(process->pid) == -1){
        return;
    }   
    add_list(readys, (list_elem_t)process);
    remove_list(blockeds, (list_elem_t)process); //Checkear q pasa si no lo encuentra
}

void block(PCB* process){
    if (process == NULL || block_process(process->pid) == -1){
        return;
    }
    remove_list(readys, (list_elem_t)process); //Checkear q pasa si no lo encuentra
    add_list(blockeds, (list_elem_t)process);
}

PCB* get_running(){
    return running;
}

//IDEA: tener un quantum y multiplicarlo por la prioridad 
//LISTA DE PROCESOS READY

//En la clase dijieron: armar lista de los readys y lista de los blocked y pasar por los readys 

uint64_t scheduler(uint64_t current_rsp){
    if(!initialized){
        initialize_scheduler(idle_pcb->pid);
    }

    if(running != NULL){
        running->rsp = current_rsp;
        
        if(running->state != RUNNING){
            if(running->state == READY){
                add_list(readys, running);
            }
            running = NULL;
        }
    }
    // Si tiene quantum, sigue

    if(running != NULL && running->time > 0){
        running->time--;
        return running->rsp;
    }
    
    if(is_empty(readys)){
        running = idle_pcb;
        running->state = RUNNING;
        running->time = QUANTUM;
        return running->rsp;
    }
    
    if(running==NULL){
        if(!has_next(readys)){
            to_begin(readys);
        }
        running = next(readys);
        remove_list(readys, running);
        running->state=RUNNING;
        running->time = QUANTUM*(10-running->priority);
    }

    return running->rsp;
}

void yield(){
    timer_tick();
}

