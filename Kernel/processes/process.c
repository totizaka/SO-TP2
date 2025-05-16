#include <process.h>
#include <mm_dummy.h>


PCB * running;

 PCB pcb_table[MAX_PID]={0};

 typedef struct {
    uint64_t r15, r14, r13, r12, r11, r10, r9, r8, rsi, rdi, rbp, rdx, rcx, rbx, rax;
 }stack_regs;

 typedef struct{
    uint64_t align;
    uint64_t ss;
    uint64_t cs;
    uint64_t rsp;
    uint64_t rip;
    uint64_t rflags;
    stack_regs stack_regs;
 }stack;
 
 void*  load_stack(uint64_t rip, uint64_t rsp, uint64_t pid, char ** argv, uint64_t argc);

uint64_t new_process(uint64_t rip, uint8_t priority, char ** argv, uint64_t argc){
    int64_t pid = find_free_pcb();
    if (pid == -1){
        return -1; 
    }
    PCB *current = &pcb_table[pid];
    current->priority = priority;
    current->rip = rip;
    current->state = ready;
    memory_manager_ADT mm;
   current->rsp= my_malloc(mm, sizeof(stack) ); //HAY QUE PASARLE EL MEMORY MANAGER Q USAMOS EN KERNEL.C???? <-- a resolver
    if(current->rsp==NULL){
        return -1;
    }
    current->rsp = load_stack(current->rip,current->rsp, pid, argv, argc);//  inciializar el stack
    current->args = argv;
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
    while(pcb_table[to_ret].state!=available){
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

void*  load_stack(uint64_t rip, uint64_t rsp, uint64_t pid, char ** argv, uint64_t argc){

    stack*to_ret=rsp;
    to_ret->cs=0x8;
    to_ret->rflags=0x202;
    to_ret->ss=0x0;
    to_ret->rsp=rsp;
    to_ret->stack_regs.rdi=rip;
    to_ret->stack_regs.rcx=pid;
    to_ret->stack_regs.rsi=(uint64_t)argv;
    to_ret->stack_regs.rdx=argc;
    //falta la parte de la wrapper??

   return (void *) to_ret;
}