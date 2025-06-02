#include <process.h>
#define STACK_SIZE (4 * 1024)

 PCB pcb_table[MAX_PID]={0};

 
 typedef struct {
    uint64_t r15, r14, r13, r12, r11, r10, r9, r8, rsi, rdi, rbp, rdx, rcx, rbx, rax;
 }stack_regs;


 typedef struct{
    stack_regs stack_regs;
	uint64_t rip;
	uint64_t cs;
	uint64_t rflags;
	uint64_t rsp;
	uint64_t ss;
 }stack;
 
void process_wrapper ( main_function rip, char ** argv, uint64_t argc, pid_t pid );

static uint64_t my_strlen ( const char *str ) 
{
	const char *s = str;
	while ( *s )
		++s;
	return s - str;
}

static char ** copy_argv(uint64_t pid, char ** argv, uint64_t argc){
   
    if( (argc == 0 && argv != NULL) || (argc>0 && argv==NULL)){ 
        return NULL;
    }

   /// pcb_array[pid].argc = argc;
    
    if(argc == 0){
       // pcb_array[pid].argv == NULL;
        return NULL;
    }
    
    char ** ans = my_malloc(get_memory_manager() ,sizeof(char *) * (argc+1));

    if(ans == NULL){
        return NULL;
    }

    for(uint64_t i=0; i<argc;i++){
        uint64_t len = my_strlen(argv[i])+1;
        char * p = my_malloc(get_memory_manager() ,len);
        if(p == NULL){ 
            for(uint64_t j=0; j<i;j++){
                my_free(get_memory_manager() ,ans[j]);
            }
            my_free(get_memory_manager() ,ans);
            return NULL;
        }
        memcpy(p, argv[i], len);
        ans[i] = p;
    }

    ans[argc] = NULL; // Terminar el array de args con NULL

    return ans;
}
 
void set_free_pcb(pid_t pid){
    pcb_table[pid].state=FREE;
}

uint64_t new_process(uint64_t rip, uint8_t priority, char ** argv, uint64_t argc){
    int64_t pid = find_free_pcb();
    if (pid == -1){
        return -1; 
    }

    uint64_t rsp_malloc = (uint64_t) my_malloc(get_memory_manager(), STACK_SIZE);

    if (rsp_malloc == 0) {
        pcb_table[pid].state = FREE;
        return -1;
    }

    char ** args_cpy = copy_argv(pid, argv, argc);
    if(argc > 0 && args_cpy == NULL){
        my_free(get_memory_manager(),(void *)rsp_malloc); 
        pcb_table[pid].state = FREE;
        return -1;
    }

    PCB *current = &pcb_table[pid];
    current->pid = pid;
    current->stack_base = rsp_malloc;
    current->priority = priority;
    current->rip = rip;
    current->time = QUANTUM * (10 - priority);
    // El stack crece hacia abajo, por eso rsp apunta al final del bloque reservado
    current->rsp = rsp_malloc + STACK_SIZE;
    if(current->rsp == 0){
        return -1;
    }
    current->rsp = load_stack(rip, current->rsp, argv, argc, pid);//  inciializar el stack
    current->args = args_cpy;
    ready(current);
    return pid;
}


//Implementado como si las listas NO estuvieran en shm
int64_t block_process(uint64_t pid){
    if(pid < 1 || pid >= MAX_PID){
         return -1;
    }
    if(pcb_table[pid].state==BLOCKED){
        return 0;
    }
    pcb_table[pid].state = BLOCKED;
    return 1; 
}

//devuelve -1 si pid no esta en rango, 0 si "no hizo nada", 1 si cambio
int64_t ready_process(uint64_t pid){
    if(pid < 1 || pid >= MAX_PID){
        return -1;
    }
    if(pcb_table[pid].state==READY){
        return 0;
    }
    pcb_table[pid].state = READY;
    return 1;
    
    //falta cambiarlo de cola cuando ya las tengamos implementadas en el scheduler
}

//falta implementar
int64_t kill_process(uint64_t pid){
    if (pid < 1 || pid >= MAX_PID || pcb_table[pid].state == FREE){
        return -1;
    }
    //draw_word(0xFFFFFF, "vpy a matar\n");
    remove_from_scheduler(&pcb_table[pid]);
    set_free_pcb(pid);

    // Liberar args
    if(pcb_table[pid].args != NULL){
        for(uint64_t i=0; pcb_table[pid].args[i] != NULL; i++){
            my_free(get_memory_manager(), pcb_table[pid].args[i]);
        }
        my_free(get_memory_manager(), pcb_table[pid].args);
        pcb_table[pid].args = NULL;
    }

    my_free(get_memory_manager(), (void *)pcb_table[pid].stack_base);
    return 0;
}


int64_t find_free_pcb(){
    int64_t to_ret=1;
    while(pcb_table[to_ret].state!=FREE && to_ret < MAX_PID){
        to_ret++;
    }
    if(to_ret == MAX_PID){
        return -1;
    }
    return to_ret;
}

int64_t get_pid(){
    PCB* running = get_running();
    return running->pid;
}


PCB* get_pcb(uint64_t pid){
    if(pid < 0 || pid > MAX_PID){
        return NULL;
    }
    return &pcb_table[pid];
}


int64_t nice(int64_t pid, uint8_t new_prio){
    if(pid<1 || pid>MAX_PID || pcb_table[pid].state == FREE){
        return -1;
    }
    // Prioridad inválida
    if (new_prio < LOW_PRIORITY || new_prio > HIGH_PRIORITY) {
        return -1;
    }
    pcb_table[pid].priority = new_prio;
    return 1;
}



uint64_t load_stack(uint64_t rip, uint64_t rsp, char ** argv, uint64_t argc, uint64_t pid){

    stack* to_ret=(stack*)(rsp - sizeof(stack));
    to_ret->stack_regs.rdi = rip;
    to_ret->stack_regs.rsi = argv;
    to_ret->stack_regs.rdx = argc;
    to_ret->stack_regs.rcx = pid;
    to_ret->rip = rip;
    to_ret->cs = 0x8;
    to_ret->rflags = 0x202;
    to_ret->rsp = (uint64_t) to_ret;
    to_ret->ss = 0x0;

   return (uint64_t) to_ret;
}


void process_wrapper(main_function rip, char **argv, uint64_t argc, uint64_t pid) {
     int ret = rip(argv, argc);
    PCB * pcb= get_pcb(pid);
    if(pcb == NULL){
        return;
    }
    pcb->ret = ret;  //@TODO esto podria estar en kill_process
    kill_process(pid);
}


void set_idle(uint64_t rip, uint8_t priority, char ** argv, uint64_t argc){
    pid_t pid=0;
    uint64_t rsp_malloc = (uint64_t) my_malloc(get_memory_manager(), STACK_SIZE);
    char ** args_cpy = copy_argv(pid, argv, argc);

    if(argc > 0 && args_cpy == NULL){
        my_free(get_memory_manager(),(void *)rsp_malloc); 
        pcb_table[pid].state = FREE;
        return ;
    }

    PCB *current = &pcb_table[pid];
    current->pid = pid;
    current->priority = priority;
    current->rip = rip;
    current->state = READY;
    current->rsp = rsp_malloc + STACK_SIZE; 
    if(current->rsp == 0){
        return ;
    }
    current->rsp = load_stack(rip, current->rsp, argv, argc, pid);//  inciializar el stack
    current->args = args_cpy; 
}

PCB * get_idle(){
    return &pcb_table[0];
}

    
