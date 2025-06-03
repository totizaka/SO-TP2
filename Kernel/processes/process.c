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
 
int set_free_pcb(pid_t pid) {
    if (pid < 1 || pid >= MAX_PID) {
        return -1;
    }
    PCB *process = get_pcb(pid);
    if (process == NULL || process->state == FREE) {
        return -1; 
    }
    // Liberar args
    if (process->args != NULL) {
        for (uint64_t i = 0; process->args[i] != NULL; i++) {
            my_free(get_memory_manager(), process->args[i]);
        }
        my_free(get_memory_manager(), process->args);
        process->args = NULL;
    }
    // Liberar stack
    my_free(get_memory_manager(), (void *)process->stack_base);
    process->pid = 0;
    process->ppid = 0;
    process->rip = 0;
    process->rsp = 0;
    process->stack_base = 0;
    process->priority = 0;
    process->time = 0;
    process->ret = 0;
    process->waiting_me = NULL;
    process->waiting_for = NULL;
    process->state = FREE;
    return 0;
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
    current->ppid = get_pid(); // Set parent PID to the running process
    current->stack_base = rsp_malloc;
    current->priority = priority;
    current->rip = rip;
    current->time = QUANTUM * (10 - priority);
    // El stack crece hacia abajo, por eso rsp apunta al final del bloque reservado
    current->rsp = rsp_malloc + STACK_SIZE;
    if(current->rsp == 0){
        return -1;
    }
    current->rsp = load_stack(rip, current->rsp, args_cpy, argc, pid);//  inciializar el stack
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
    if(pcb_table[pid].state == READY){
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
    remove_from_scheduler(&pcb_table[pid]);
    unblock_waiting_proc(&pcb_table[pid]);

    if (pcb_table[pid].waiting_for != NULL && pcb_table[pid].waiting_for->waiting_me != NULL){
        pcb_table[pid].waiting_for->waiting_me = NULL; // El proceso que estaba esperando a este se desbloquea
    }
    set_free_pcb(pid);
    if(&pcb_table[pid] == get_running()){
        // Si el proceso que se está matando es el que está corriendo, hacemos yield
        yield();
    }
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
    if(pid < 0 || pid >= MAX_PID){
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
     if(argv!=NULL){
    draw_word(0xFFFFFF, argv[0]);
    }
    stack* to_ret=(stack*)(rsp - sizeof(stack));
    to_ret->stack_regs.rdi = (uint64_t) rip;
    to_ret->stack_regs.rsi = (uint64_t) argv;
    to_ret->stack_regs.rdx = argc;
    to_ret->stack_regs.rcx = pid;
    to_ret->rip = (uint64_t) process_wrapper;
    to_ret->cs = 0x8;
    to_ret->rflags = 0x202;
    to_ret->rsp = rsp;
    to_ret->ss = 0x0;

   return (uint64_t) to_ret;
}



void process_wrapper(main_function rip, char **argv, uint64_t argc, uint64_t pid) {
    int ret = rip(argv, argc);
    _cli();
    PCB * pcb= get_pcb(pid);
    if(pcb == NULL){
        return;
    }
    zombie(ret);
    timer_tick();
}


int zombie(int ret){
    PCB *process = get_running();
    if (process == NULL || process->state == FREE) {
        return -1; // No hay proceso corriendo o el proceso ya está libre
    }
    process->ret = ret;
    remove_from_scheduler(process); // Eliminar de la lista de procesos listos
    unblock_waiting_proc(process); // Desbloquear procesos que estaban esperando a este
    process->state = ZOMBIE;
    process->waiting_me = NULL; // El proceso no espera a nadie
    process->waiting_for = NULL; // El proceso no está esperando a nadie
    return 0;
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
    current->stack_base = rsp_malloc;
    current->pid = pid;
    current->priority = priority;
    current->rip = rip;
    current->state = READY;
    current->rsp = rsp_malloc + STACK_SIZE; 
    if(current->rsp == 0){
        return ;
    }
    current->rsp = load_stack(rip, current->rsp, args_cpy, argc, pid);//  inciializar el stack
    current->args = args_cpy; 
}

PCB * get_idle(){
    return &pcb_table[0];
}

void list_processes() {
    char s[64];  // Espacio suficiente para el PID + texto del estado + terminador

    for (int i = 1; i < MAX_PID; i++) {
        if (pcb_table[i].state != FREE) {
            int len = 0;

            // Convertir PID a string
            itoa(pcb_table[i].pid, s);

            // Buscar fin del string
            while (s[len] != '\0') len++;

            // Agregar '-'
            s[len++] = '-';

            // Agregar palabra según el estado
            switch (pcb_table[i].state) {
                case FREE:
                    s[len++] = 'F';
                    s[len++] = 'R';
                    s[len++] = 'E';
                    s[len++] = 'E';
                    break;
                case BLOCKED:
                    s[len++] = 'B';
                    s[len++] = 'L';
                    s[len++] = 'O';
                    s[len++] = 'C';
                    s[len++] = 'K';
                    s[len++] = 'E';
                    s[len++] = 'D';
                    break;
                case READY:
                    s[len++] = 'R';
                    s[len++] = 'E';
                    s[len++] = 'A';
                    s[len++] = 'D';
                    s[len++] = 'Y';
                    break;
                case ZOMBIE:
                    s[len++] = 'Z';
                    s[len++] = 'O';
                    s[len++] = 'M';
                    s[len++] = 'B';
                    s[len++] = 'I';
                    s[len++] = 'E';
                    break;
                case RUNNING:
                    s[len++] = 'R';
                    s[len++] = 'U';
                    s[len++] = 'N';
                    s[len++] = 'N';
                    s[len++] = 'I';
                    s[len++] = 'N';
                    s[len++] = 'G';
                    break;
            }

            // Agregar salto de línea y terminador nulo
            s[len++] = '\n';
            s[len] = '\0';

            // Dibujar la línea
            draw_word(s);
        }
    }
}

pid_t wait(pid_t pid, int64_t *ret) {
    PCB *pcb_to_wait = get_pcb(pid);
    PCB *running = get_running();

    if (pcb_to_wait == NULL || pcb_to_wait->state == FREE || pcb_to_wait->waiting_me != NULL || pid == get_pid()) {
        return -1;
    }
    // Si el hijo no terminó, bloqueamos al padre
    if (pcb_to_wait->state != ZOMBIE) {
        pcb_to_wait->waiting_me = running;
        running->waiting_for = pcb_to_wait;
        block(running);  // Bloquea al proceso running
        // running->waiting_for = NULL;
    }
    // Si el hijo fue eliminado antes de terminar
    if (pcb_to_wait->state != ZOMBIE) {
        return -1;
    }
    if (ret != NULL) {
        *ret = pcb_to_wait->ret;
    }
    set_free_pcb(pid);
    return pid;
}
