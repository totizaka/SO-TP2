#ifndef SHARED_DEFS_H
#define SHARED_DEFS_H

#include <stdint.h>
#define STDIN 0
#define STDOUT 1
#define STDERR 2 

#define FD_MAX 3 				// CANT DE FILE DESCRIPTORS
typedef int64_t target_t;
typedef int64_t fd_t;

#define LOW_PRIORITY 1
#define MEDIUM_PRIORITY 3
#define HIGH_PRIORITY 5

typedef enum {
    FREE = 0,
    READY,
    BLOCKED,
    RUNNING,
    ZOMBIE
} process_state_t;

typedef int64_t pid_t;

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