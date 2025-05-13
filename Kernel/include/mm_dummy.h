#ifndef MM_DUMMY_H
#define MM_DUMMY_H

#include <stdlib.h>

typedef struct MemoryManagerCDT *memory_manager_ADT;

memory_manager_ADT createMemoryManager(void *const restrict memoryForMemoryManager,
    void *const restrict managedMemory,
    size_t managedMemorySize);

void *my_malloc(memory_manager_ADT manager);

void my_free(memory_manager_ADT manager, void *ptr);

size_t my_get_available_memory(memory_manager_ADT manager);

#endif