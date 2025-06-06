#ifndef SHARED_DEFS_H
#define SHARED_DEFS_H

#include <stdint.h>

typedef enum {
    FREE = 0,
    READY,
    BLOCKED,
    RUNNING,
    ZOMBIE
} process_state_t;

typedef uint64_t pid_t;

typedef struct process_info {
	pid_t pid;
	uint8_t priority;
	uint64_t stack_base;
	uint64_t stack_pointer;
	process_state_t status;
	char * name;
	uint8_t background;
} process_info;

typedef struct process_info_list {
	uint64_t amount_of_processes;
	process_info * processes;
} process_info_list;

typedef struct memory_state {
	uint64_t total_size;
	uint64_t free;
	uint64_t occupied;
} memory_state;

#endif