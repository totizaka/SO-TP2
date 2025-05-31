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

int64_t ready(PCB* process){
    if (process == NULL || ready_process(process->pid) == -1){
        return -1;
    }   
    add_list(readys, (list_elem_t)process);
    remove_list(blockeds, (list_elem_t)process); 
    return 0;
}
int64_t block(PCB* process){
    if (process == NULL || block_process(process->pid) == -1){
        return -1;
    }
    remove_list(readys, (list_elem_t)process);
    add_list(blockeds, (list_elem_t)process);
    if (running == process) {
        yield();
    }
    return 0;
}

PCB* get_running(){
    return running;
}

uint64_t scheduler(uint64_t current_rsp){
    if (!initialized){
        return current_rsp;
    }
    if (running != NULL){
        running->rsp = current_rsp;
    }
    if (is_empty(readys)){
        //Si no existen procesos para correr => corre el idle 
        running = get_idle();
        idle = get_idle();
        //is_idle = 1; // Indicamos que estamos en el idle
        running->time = UINT64_MAX; // El idle no tiene quantum, corre indefinidamente
        current_rsp = running->rsp;
    }else{
    if (running == NULL || running->state != RUNNING || running == idle){ 
        running = next(readys);
        running->time = QUANTUM * (1 + running->priority);
        running->state = RUNNING; // Aseguramos que el nuevo proceso esté en estado RUNNING
        //is_idle = 0; // Si se selecciona un proceso diferente al idle, no estamos en idle
        return running->rsp;
    }

    if (--running->time == 0) {
        // Se acabó su quantum, lo re-encolamos si no terminó
        // if (running->state == RUNNING) {
        //     ready(running);  // volver a ponerlo en cola
        // }
        running = next(readys);
        running->time = QUANTUM * (1 + running->priority);
        running->state = RUNNING; // Aseguramos que el nuevo proceso esté en estado RUNNING
        //is_idle = 0; // Si se selecciona un proceso diferente al idle, no estamos en idle
        return running->rsp;
    }
}
    return current_rsp;
}

void yield(){
    timer_tick();
}

void remove_from_scheduler(PCB* process){
    draw_word(0xFFFFFF, "entre a remove\n");
    if (process == NULL){
        draw_word(0xFF0000, "Error: Attempted to remove a NULL process from scheduler\n");
        return;
    }

    //asi funciona

    remove_list(blockeds, (list_elem_t)process);
    remove_list(readys, (list_elem_t)process);

    //fijarse porque esto de abajo no funciona, debuggear, no se cargan bien los estados? anda mal block?

    if (process->state == READY){
        draw_word(0xFFFFFF, "Removing process from ready list\n");
        remove_list(readys, (list_elem_t)process);
    }
    else if (process->state == BLOCKED){
        draw_word(0xFFFFFF, "Removing process from blocked list\n");
        remove_list(blockeds, (list_elem_t)process);
    }
}
