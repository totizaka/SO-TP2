#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>
#include <defs.h>
#include <mm_dummy.h>
#include <pcb.h>
#include <kernel.h>
#include <interrupts.h>


// #define zombie 3
// #define blocked 2
// #define ready 1
// #define available 0

#define MAX_PID 50

#define LOW_PRIORITY 1
#define MEDIUM_PRIORITY 2
#define HIGH_PRIORITY 3


typedef int ( *main_function ) ( char ** argv, uint64_t argc );


int64_t find_free_pcb();
uint64_t new_process(uint64_t rip, uint8_t priority, char ** argv, uint64_t argc);
int64_t block_process(uint64_t pid);
int64_t ready_process(uint64_t pid);
int64_t kill_process(uint64_t pid);
int64_t get_pid();
int64_t nice(int64_t pid, uint8_t new_prio);
uint64_t load_stack(uint64_t rip, uint64_t rsp, uint64_t pid, char ** argv, uint64_t argc);
PCB* get_pcb(uint64_t pid);
PCB* list_processes();//quiza sea mejor directamente printear desde kernel¿?
void set_idle(uint64_t rip, uint8_t priority, char ** argv, uint64_t argc);

#endif