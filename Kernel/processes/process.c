#include <process.h>

PCB * running;

 PCB pcb_table[MAX_PID]={0};

uint64_t new_process(uint64_t rip, uint8_t priority){
    int64_t pid = find_free_pcb();
    if (pid == -1){
        return -1; 
    }
    PCB *current = &pcb_table[pid];
    current->priority = priority;
    current->rip = rip;
    current->state = ready;
    // current->rsp  inciializar el stack
    return pid;
}

int64_t block_process(uint64_t pid){
    if(pid < 0 || pid >= MAX_PID){
         //ERROR, VER COMO MANEJAMOS ERRORES???
         return -1;
    }
    pcb_table[pid].state=blocked;
    return 1;
}

int64_t ready_process(uint64_t pid){
    if(pid < 0 || pid >= MAX_PID){
        return -1;
    }
    pcb_table[pid].state=ready;
    return 1;
}
//falta implementar
int64_t kill_process(uint64_t pid){
    return 0;
}

int64_t find_free_pcb(){
    int64_t to_ret=0;
    while(pcb_table[to_ret].state!=free){
        to_ret++;
        if(to_ret == MAX_PID){
        return -1;
     }
    }
    return to_ret;
}


int64_t get_pid(){
    return running->pid;
}

int64_t nice(int64_t pid, uint8_t new_prio){
    if(pid<0 || pid>MAX_PID){
        return -1;
    }
    pcb_table[pid].priority=new_prio;
    return 1;
}