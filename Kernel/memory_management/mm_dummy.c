#include "mm_dummy.h"
#include <stddef.h>
#include <stdint.h>

#define MEMORY_ALIGNMENT 8

typedef struct MemoryManagerCDT {
	char *nextAddress;
	char *baseAddress;
	size_t totalSize;
} MemoryManagerCDT;


MemoryManagerADT createMemoryManager(void *const restrict memoryForMemoryManager, void *const restrict managedMemory, size_t managedMemorySize) {
    MemoryManagerADT memoryManager = (MemoryManagerADT) memoryForMemoryManager;

    memoryManager->baseAddress = (char *) managedMemory;
    memoryManager->nextAddress = memoryManager->baseAddress;
    memoryManager->totalSize = managedMemorySize;

    return memoryManager;
}


void *allocMemory(MemoryManagerADT const restrict memoryManager, const size_t memoryToAllocate) {
	uintptr_t currentAddress = (uintptr_t) memoryManager->nextAddress;

	uintptr_t alignedAddress = (currentAddress + (MEMORY_ALIGNMENT - 1)) & ~(MEMORY_ALIGNMENT - 1);
	uintptr_t newNextAddress = alignedAddress + memoryToAllocate;

	// Verificar que no se exceda el bloque
	if ((newNextAddress - (uintptr_t) memoryManager->baseAddress) > memoryManager->totalSize) {
		return NULL;  // No hay suficiente memoria
	}

	memoryManager->nextAddress = (char *) newNextAddress;

	return (void *) alignedAddress;
}

size_t getFreeMemory(MemoryManagerADT const memoryManager) {
	return memoryManager->totalSize - (memoryManager->nextAddress - memoryManager->baseAddress);
}