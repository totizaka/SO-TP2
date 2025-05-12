#ifndef MM_DUMMY_H
#define MM_DUMMY_H

#include <stdlib.h>

typedef struct MemoryManagerCDT *MemoryManagerADT;

MemoryManagerADT createMemoryManager(void *const restrict memoryForMemoryManager,
    void *const restrict managedMemory,
    size_t managedMemorySize);

void *my_malloc(MemoryManagerADT manager);

void my_free(MemoryManagerADT manager, void *ptr);

size_t my_get_available_memory(MemoryManagerADT manager);

#endif