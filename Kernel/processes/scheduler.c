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

void initialize_scheduler(void * shell){
    t_cmp cmp = compare_elem;
    readys = new_list(cmp);
    blockeds = new_list(cmp);

    if (readys == NULL || blockeds == NULL) {
        return;
    }

    char* argv_shell[] = {"shell", NULL};

    fd_t fd[FD_MAX]={STDIN,STDOUT,STDERR};
    new_process((void(*))shell, SHELL_PID, argv_shell, 1, 0, fd);
    //habria q ver si podemos decir q ya sabemos q la shell no va a tener args y ademas q elegimos nosotras la prioridad¿?
    initialized = 1;
}

int64_t ready(PCB* process){
    int res;
    if (process != NULL){
        res=ready_process(process->pid);
        if(res==-1){
            return -1;
        }else if(res==0){
            return 0;
        }
    } else{
        return -1;
    }  
    add_list(readys, (list_elem_t)process);
    remove_list(blockeds, (list_elem_t)process); 
    return 0;
}

int64_t block(PCB* process){
    if(block_no_yield(process)==-1){
        return -1;
    }
    if (running == process) {
        yield();
    }
    return 0;
}

int64_t block_no_yield(PCB*process){
    int res;
    if (process != NULL){
        res=block_process(process->pid);
        if(res==-1){
            return -1;
        }else if(res==0){
            return 0;
        }
    } else{
        return -1;
    } 
    remove_list(readys, (list_elem_t)process);
    add_list(blockeds, (list_elem_t)process);
    return 0;
}

void unblock_waiting_proc(PCB *child) {
    if (child == NULL || child->waiting_me == NULL)
        return;

    PCB *parent = child->waiting_me;

    // Asegurarse de que el padre está esperando al hijo correcto
    if ( parent->state == BLOCKED) {
        parent->waiting_for = NULL;
        ready(parent); // Tu función para meterlo al scheduler
    }
    child->waiting_me = NULL; // Limpieza del hijo
}

PCB* get_running(){
    return running;
}

uint64_t scheduler(uint64_t current_rsp){
    if (!initialized){
        return current_rsp;
    }
   
    if (running != NULL && running->state!=FREE){
        running->rsp = current_rsp;
    }else {
        running==NULL;
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
        running->state = READY;
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
    if (process == NULL){
        return;
    }
    if (process->state == READY || process->state==RUNNING){
        remove_list(readys, (list_elem_t)process);
    }
    else if (process->state == BLOCKED){
        remove_list(blockeds, (list_elem_t)process);
    }
}
