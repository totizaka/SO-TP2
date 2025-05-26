#include <scheduler.h>

list_adt readys;
list_adt blockeds;

//proceso actual
PCB *running = NULL;
int64_t ran_time = 0;
static int initialized = 0;
static PCB * idle_pcb;

typedef int ( *main_function ) ( char ** argv, uint64_t argc );

int compare_elem(list_elem_t e1, list_elem_t e2){
    return e1 - e2;
}



// void initialize_scheduler(pid_t idle_pid){
//     draw_word(0xFFFFFF, "Initializing scheduler\n");
//     t_cmp cmp = compare_elem;
//     readys = new_list(cmp);
//     blockeds = new_list(cmp);

//     if (readys == NULL || blockeds == NULL) {
//         return;
//     }

//     idle_pcb = get_pcb(idle_pid);
//     initialized = 1;
//     to_begin(readys);
// }

void initialize_scheduler(){
    draw_word(0xFFFFFF, "Initializing scheduler\n");
    t_cmp cmp = compare_elem;
    readys = new_list(cmp);
    blockeds = new_list(cmp);

    if (readys == NULL || blockeds == NULL) {
        return;
    }
    initialized = 1;
    to_begin(readys);
}

void ready(PCB* process){
    if (process == NULL || ready_process(process->pid) == -1){
        return;
    }   
    process->state=READY;
    add_list(readys, (list_elem_t)process);
    remove_list(blockeds, (list_elem_t)process); //Checkear q pasa si no lo encuentra
}

void block(PCB* process){
    if (process == NULL || block_process(process->pid) == -1){
        return;
    }
    process->state=BLOCKED;
    remove_list(readys, (list_elem_t)process); //Checkear q pasa si no lo encuentra
    add_list(blockeds, (list_elem_t)process);
}

PCB* get_running(){
    return running;
}

//IDEA: tener un quantum y multiplicarlo por la prioridad 
//LISTA DE PROCESOS READY

//En la clase dijieron: armar lista de los readys y lista de los blocked y pasar por los readys 


uint64_t itoa(uint64_t number, char* s) {
    int digits = 0;

    // Contar los dígitos
    for (uint64_t n = number; n != 0; digits++, n /= 10);

    // Manejo del caso donde number es 0
    if (digits == 0) {
        s[0] = '0';
        s[1] = '\0';
        return 1;
    }

    // Terminar la cadena con un carácter nulo
    s[digits] = '\0';

    // Convertir los dígitos en orden inverso
    for (int i = digits - 1; i >= 0; i--) {
        s[i] = (number % 10) + '0';
        number /= 10;
    }

    return digits;
}

uint64_t scheduler(uint64_t current_rsp){
     draw_word(0xFFFFFF, "adentro!!\n");
     return current_rsp;
}

/*uint64_t scheduler(uint64_t current_rsp){
    if(!initialized){
        return current_rsp;
    }
    draw_word(0xFFFFFF, "adentro!!\n");

    
    if(running != NULL){
        running->rsp = current_rsp;
        
        /*if(running->state != RUNNING){
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
    
    char jiji[20];
    uint8_t aux = is_empty(readys);
    itoa(aux, jiji);
    draw_word(0xFFFFFF, jiji);

    /*if(is_empty(readys)){
        running = idle_pcb;
        running->state = RUNNING;
        running->time = QUANTUM;
        char str[20];
        itoa(running->rsp, str);
        draw_word(0xFFFFFF, "holisss\n");
        draw_word(0xFFFFFF, str);
        
        return running->rsp;
    }
    
    if(running==NULL){
        draw_word(0xFFFFFF, "running es null\n");
        if(!has_next(readys)){
            to_begin(readys);
        }
        running = next(readys);
        remove_list(readys, running);

        char jijo[20];
        uint64_t aux2 = running->rip;
        itoa(aux2, jijo);
        draw_word(0xFFFFFF, jijo);

        running->state = RUNNING;
        running->time = QUANTUM*(10-running->priority);
    }

    if(running->time==0){
        if(!has_next(readys)){
            to_begin(readys);
        }
        running = next(readys);
        remove_list(readys, running);
        running->state=RUNNING;
        running->time = QUANTUM*(10-running->priority);
    }
    return current_rsp;
}*/



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
