#include <stdint.h>


#define blocked 2
#define ready 1
#define free 0
#define MAX_PID 50

typedef struct {
uint64_t pid, ppid; //si se puede usar libreria sys/type ponemos pid_t directo
void* rsp; 
uint64_t rip;//desp ver si va a estar en el stack
uint8_t state;//ready, blocked o free
uint8_t priority; //no c si esta ok el tipo de dato, q escala de prioridades vamos a tener????
}PCB;


PCB pcb_table[MAX_PID]={0};
int64_t find_free_pcb();

uint64_t new_process(uint64_t rip, uint8_t priority){
int64_t pid=find_free_pcb();
PCB current= pcb_table[pid];
current.priority=priority;
current.rip=rip;
}

void block_process(uint64_t pid){
    if(pid<0 || pid>MAX_PID){
         //ERROR, VER COMO MANEJAMOS ERRORES???
         return;
    }
    pcb_table[pid].state=blocked;
}
void ready_process(uint64_t pid){
    if(pid<0 || pid>MAX_PID){
         //ERROR, VER COMO MANEJAMOS ERRORES???
         return;
    }
    pcb_table[pid].state=ready;
}
void kill_process(uint64_t pid); //no c si lo vamos a dejar aca o pasarlo a syscal???

int64_t find_free_pcb(){
    int64_t toRet=0;
    while(pcb_table[toRet].state!=free){
        toRet++;
        if(toRet==MAX_PID){
        return -1;
     }
    }
    return toRet;
}

