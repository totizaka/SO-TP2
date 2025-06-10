#ifndef PCB_H_
#define PCB_H_

#include <stdint.h>
#include <shared_defs.h>

typedef struct PCB{
    pid_t pid, ppid;
    uint64_t rsp; 
    uint64_t stack_base;
    void* rip;       
    process_state_t state;      // Running, Ready, Blocked, Zombie
    uint8_t priority;           // Higher number means higher priority
    char* name;
    char** args;
    int argc; 
    struct PCB* waiting_me;     // Process that is waiting for this process to finish
    struct PCB* waiting_for;    // Process that this process is waiting for
    int time;                   // Time slice for the process
    int ret;
    fd_t fd[3];                 // File descriptors 
    int8_t background;          // 1 si es un proceso en background, 0 si es foreground
    int16_t group_id;           // ID del grupo de procesos al que pertenece
}PCB;

#endif