#include <process.h>
#define STACK_SIZE (4 * 1024)

PCB pcb_table[MAX_PID]={0};

uint64_t amount_of_processes = 0;
 
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
int64_t piping(pid_t pid, fd_t fds[]);

static uint64_t my_strlen ( const char *str ) 
{
	const char *s = str;
	while ( *s )
		++s;
	return s - str;
}

static char ** copy_argv(uint64_t pid, char ** argv, uint64_t argc){

    if ((argc == 1 && argv[1] != NULL) || (argc > 1 && argv[1] == NULL)){
        return NULL;
    }

   /// pcb_array[pid].argc = argc;
    
    if(argc == 1){
       // pcb_array[pid].argv == NULL;
        return NULL;
    }
    
    uint64_t real_argc = argc - 1;
    char ** ans = my_malloc(get_memory_manager(), sizeof(char *) * (real_argc + 1)); // +1 para NULL

    if(ans == NULL){
        return NULL;
    }

    for(uint64_t i=1; i<argc;i++){
        uint64_t len = my_strlen(argv[i])+1;
        char * p = my_malloc(get_memory_manager() ,len);
        if(p == NULL){ 
            for(uint64_t j=1; j<i;j++){
                my_free(get_memory_manager() ,ans[j]);
            }
            my_free(get_memory_manager() ,ans);
            return NULL;
        }
        memcpy(p, argv[i], len);
        ans[i-1] = p;
    }

    ans[real_argc] = NULL;

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

    // Cerrar pipes abiertos por el proceso antes de liberar recursos
    for (int i = 0; i < FD_MAX; i++) {
        if (process->fd[i] >= FD_MAX && process->fd[i] < FD_MAX + MAX_PIPES) {
            close_pipe(process->fd[i] - FD_MAX, process->pid);
        }
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

//primer arg va a ser el nombre por convencion argv[0] = nombre del proceso

uint64_t new_process(uint64_t rip, uint8_t priority, char ** argv, uint64_t argc, int8_t background, fd_t fds[FD_MAX]) {
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
    if(argc > 1 && args_cpy == NULL){
        my_free(get_memory_manager(),(void *)rsp_malloc); 
        pcb_table[pid].state = FREE;
        return -1;
    }

    PCB *current = &pcb_table[pid];
    current->pid = pid;
    current->ppid = get_pid(); // Set parent PID to the running process
    current->name = argv != NULL && argv[0] != NULL ? argv[0] : "Unnamed Process"; // Set name to first arg or default
    current->stack_base = rsp_malloc;
    current->priority = priority;
    current->rip = rip;
    current->background=background;
    // El stack crece hacia abajo, por eso rsp apunta al final del bloque reservado
    current->rsp = rsp_malloc + STACK_SIZE;
    if(current->rsp == 0){
        return -1;
    }
    current->rsp = load_stack(rip, current->rsp, args_cpy, argc-1, pid);//  inciializar el stack
    current->args = args_cpy;
    current->argc = argc - 1; // argc sin el nombre del proceso

    for (int i = 0; i < FD_MAX; i++) {
        if (fds[i] >= 0 && fds[i] < MAX_PIPES + FD_MAX) {
            current->fd[i] = fds[i];
        } else {
            current->fd[i] = -1;
        }
    }
    
    if (piping(current->pid, current->fd)==-1){
        my_free(get_memory_manager(), (void *)rsp_malloc);
        pcb_table[pid].state = FREE;
        return -1;
    }

    ready(current);
    amount_of_processes++;
    return pid;
}


int64_t piping(pid_t pid, fd_t fds[]) {
    for (int i = 0; i < FD_MAX; i++) {
        int role = -1;

        if (i == STDIN) {
            role = PIPE_READ;
        } else if (i == STDOUT) {
            role = PIPE_WRITE;
        }

        // Sólo proceder si se definió un rol y el fd apunta a un pipe
        if (role != -1 && fds[i] >= FD_MAX && fds[i] < FD_MAX + MAX_PIPES) {
            int pipe_id = fds[i] - FD_MAX;
            if (open_pipe(pipe_id, role, pid) == -1) {
                return -1;
            }
        }
    }
    return 0;
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
    if (pid <= 1 || pid >= MAX_PID || pcb_table[pid].state == FREE){
        //no puedo matar ni idle ni shell
        return -1;
    }
    remove_from_scheduler(&pcb_table[pid]);
    unblock_waiting_proc(&pcb_table[pid]);

    if (pcb_table[pid].waiting_for != NULL && pcb_table[pid].waiting_for->waiting_me != NULL){
        pcb_table[pid].waiting_for->waiting_me = NULL; // El proceso que estaba esperando a este se desbloquea
    }
    if(set_free_pcb(pid) != -1){
        amount_of_processes--;
    }
    if(&pcb_table[pid] == get_running()){
        // Si el proceso que se está matando es el que está corriendo, hacemos yield
        timer_tick();
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



void process_wrapper(main_function rip, char **argv, uint64_t argc, int64_t pid) {
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

    char* argv_idle[] = {"idle_process", NULL};

    PCB *current = &pcb_table[pid];
    //current->name="idle"; //despues cambiarlo a strcpy con malloc por ahi?
    current->stack_base = rsp_malloc;
    current->pid = pid;
    current->name = argv != NULL && argv[0] != NULL ? argv[0] : "Idle Process"; // Set name to first arg or default
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

char *my_strcpy(char *dest, const char *src) {
    char *original = dest;

    while ((*dest++ = *src++) != '\0');

    return original;
}

// Extrae la info relevante de un PCB a una estructura process_info para userland.
void fill_process_info(const PCB *pcb, process_info *pinfo) {
    pinfo->pid = pcb->pid;
    pinfo->name = pcb->name ? pcb->name : "NoName"; // Si no tiene nombre, asigna un valor por defecto
    pinfo->priority = pcb->priority;
    pinfo->stack_pointer = pcb->rsp;
    pinfo->stack_base = pcb->stack_base;
    pinfo->status = pcb->state;
    pinfo->background = pcb->background;

    const char *src_name = pcb->name;
    uint64_t len = my_strlen(src_name);

    char *copied_name = my_malloc(get_memory_manager(), len + 1);
    if (copied_name == NULL) {
        pinfo->name = NULL;
        return;
    }

    my_strcpy(copied_name, src_name);
    pinfo->name = copied_name;
}

uint64_t count_live_processes() {
    uint64_t count = 0;
    for (int i = 0; i < MAX_PID; i++) {
        if (pcb_table[i].state == RUNNING || pcb_table[i].state == READY || pcb_table[i].state == BLOCKED || pcb_table[i].state == ZOMBIE) {
            count++;
        }
    }
    return count;
}


// Recolecta todos los procesos activos y devuelve una lista con su info.
process_info_list * get_all_processes() {
    process_info_list *plist = my_malloc(get_memory_manager(), sizeof(process_info_list));
    if (plist == NULL) {
        return NULL;
    }

    uint64_t live_processes = count_live_processes();
    plist->amount_of_processes = live_processes;

    if (live_processes == 0) {
        plist->processes = NULL;
        return plist;
    }

    process_info * pinfos = my_malloc(get_memory_manager(), live_processes * sizeof(process_info));
    if (pinfos == NULL) {
        my_free(get_memory_manager(), plist);
        return NULL;
    }
    int found = 0;
    for (int i = 0; i < MAX_PID && found < live_processes; i++) {
        if (pcb_table[i].state == RUNNING || pcb_table[i].state == READY || pcb_table[i].state == BLOCKED || pcb_table[i].state == ZOMBIE) {
            fill_process_info(&pcb_table[i], &pinfos[found]);
            found++;
        }
    }
    plist->processes = pinfos;
    return plist;
}

// Libera la lista y todos los nombres dinámicos de procesos
void free_process_list(process_info_list *plist) {
    if (plist == NULL) {
        return;
    }
    if (plist->processes == NULL) {
        my_free(get_memory_manager(), plist);
        return;
    }
    for (uint64_t i = 0; i < plist->amount_of_processes; i++) {
        if (plist->processes[i].name) {
            my_free(get_memory_manager(), plist->processes[i].name);
        }
    }
    my_free(get_memory_manager(), plist->processes);
    my_free(get_memory_manager(), plist);
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
    if (set_free_pcb(pid) != -1){
        amount_of_processes--;
    }
    return pid;
}


void ctrl_c_handler(){
    pid_t current_pid = get_pid(); 
    if (current_pid != 0 && current_pid != 1) { // No permitir CTRL+C en el proceso kernel o init
        for (int i = 2; i < MAX_PID; i++) {
            if (pcb_table[i].state == RUNNING || pcb_table[i].state == READY) {
                // Si el proceso está en ejecución o listo, lo matamos
                kill_process(i);
            }
        }
        if (pcb_table[1].state != READY) {
            pcb_table[1].state = READY;
        }
        timer_tick(); // Forzar un cambio de contexto para que el scheduler pueda elegir un nuevo proceso
    }
}