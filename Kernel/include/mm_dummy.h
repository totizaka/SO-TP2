#ifndef MM_DUMMY_H
#define MM_DUMMY_H
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <shared_defs.h>

#define MEMORY_MANAGER_SIZE 0x100000
#define MEMORY_MANAGER_SIZE_STATE 1048576
#define BLOCK_SIZE 64

typedef struct memory_manager_cdt *memory_manager_adt;


memory_manager_adt create_memory_manager(void *memory);

void *my_malloc(memory_manager_adt manager, size_t size);

void my_free(memory_manager_adt manager, void *ptr);

size_t my_get_available_memory(memory_manager_adt manager);

memory_state* my_mem_state(memory_manager_adt manager);

void my_free_mem_state(memory_manager_adt manager, memory_state *state);
memory_manager_adt get_memory_manager();

#endif