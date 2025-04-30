#ifndef MM_DUMMY_H
#define MM_DUMMY_H

#include <stdlib.h>

typedef struct MemoryManagerCDT *MemoryManagerADT;

MemoryManagerADT createMemoryManager(void *const restrict memoryForMemoryManager,
    void *const restrict managedMemory,
    size_t managedMemorySize);

void *allocMemory(MemoryManagerADT const restrict memoryManager, const size_t memoryToAllocate);

size_t getFreeMemory(MemoryManagerADT const memoryManager);

#endif