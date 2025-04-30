#include "mm_dummy.h"
#include <stddef.h>  // para size_t
#include <stdint.h>  // para uintptr_t

#define MEMORY_ALIGNMENT 8  // o 4, según la arquitectura
typedef struct MemoryManagerCDT {
	char *nextAddress;
} MemoryManagerCDT;

MemoryManagerADT createMemoryManager(void *const restrict memoryForMemoryManager, void *const restrict managedMemory) {
	MemoryManagerADT memoryManager = (MemoryManagerADT) memoryForMemoryManager;
	memoryManager->nextAddress = managedMemory;

	return memoryManager;
}

/*void *allocMemory(MemoryManagerADT const restrict memoryManager, const size_t memoryToAllocate) {
	char *allocation = memoryManager->nextAddress;

	memoryManager->nextAddress += memoryToAllocate;

	return (void *) allocation;
}*/


void *allocMemory(MemoryManagerADT const restrict memoryManager, const size_t memoryToAllocate) {
    // Convierte la dirección actual a entero para manipularla
    uintptr_t currentAddress = (uintptr_t) memoryManager->nextAddress;

    // Aplica alineamiento: redondea hacia arriba
    uintptr_t alignedAddress = (currentAddress + (MEMORY_ALIGNMENT - 1)) & ~(MEMORY_ALIGNMENT - 1);

    // Reserva memoria desde la dirección alineada
    memoryManager->nextAddress = (char)(alignedAddress + memoryToAllocate);

    return (void *) alignedAddress;
}