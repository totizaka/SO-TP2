#include "mm_dummy.h"
#include <stddef.h>
#include <stdint.h>
#include <string.h> 

#define MEMORY_ALIGNMENT 8

#define BLOCK_SIZE 64
#define MAX_BLOCKS 1024
#define BITMAP_SIZE (MAX_BLOCKS / 8)


typedef struct MemoryManagerCDT {
    uint8_t *baseAddress;             // Puntero al bloque de memoria administrado
    uint8_t bitmap[BITMAP_SIZE];      // Un bit por cada bloque
    size_t totalBlocks;               // Total de bloques
} MemoryManagerCDT;


MemoryManagerADT createMemoryManager(void *const restrict memoryForMemoryManager, void *const restrict managedMemory, size_t managedMemorySize) {
    MemoryManagerADT manager = (MemoryManagerADT) memoryForMemoryManager;

    manager->baseAddress = (uint8_t *) managedMemory;				// La direccion de la memoria que el MM va a administrar
    manager->totalBlocks = managedMemorySize / BLOCK_SIZE;			// La cantidad de bloques que el MM va a administrar

    memset(manager->bitmap, 0, BITMAP_SIZE);  						// Inicializa todos los bloques como libres (en 0)

    return manager;
}

//Por ahora esta hecho para que se pueda solicitar solo un bloque

void *my_malloc(MemoryManagerADT manager) {
    for (size_t i = 0; i < manager->totalBlocks; i++) {
        size_t byteIndex = i / 8;									// Indice del byte dentro del bitmap
        size_t bitIndex = i % 8;									// Indice del bit dentro del byte

        if (!(manager->bitmap[byteIndex] & (1 << bitIndex))) {		// Obtiene el valor del bit en la posición deseada
            // Bloque libre encontrado
            manager->bitmap[byteIndex] |= (1 << bitIndex);  		// Marcar el bloque como ocupado
            return manager->baseAddress + (i * BLOCK_SIZE);			// Devuelve la dirección del bloque de memoria asignado
        }
    }

    return NULL; 													// No hay bloques disponibles
}

void my_free(MemoryManagerADT manager, void *ptr) {
    if (!ptr) return;												// Si puntero es nulo, no hacer nada

    uintptr_t offset = (uint8_t *)ptr - manager->baseAddress;		// Cuánto avanzamos en la memoria desde baseAddress.
    if (offset % BLOCK_SIZE != 0) return; 							// Asegurar alineación

    size_t blockIndex = offset / BLOCK_SIZE;						// Calculamos el indice del bloque en el bitmap a liberar
    if (blockIndex >= manager->totalBlocks){						
		return; 													// Si esta fuera de rango, no hacer nada
	}

    size_t byteIndex = blockIndex / 8;
    size_t bitIndex = blockIndex % 8;

    manager->bitmap[byteIndex] &= ~(1 << bitIndex);  				// Marcar como libre (en 0)
}

size_t my_get_available_memory(MemoryManagerADT manager) {
    size_t freeBlocks = 0;

    for (size_t i = 0; i < manager->totalBlocks; i++) {
        size_t byteIndex = i / 8;
        size_t bitIndex = i % 8;

        if (!(manager->bitmap[byteIndex] & (1 << bitIndex))) {
            freeBlocks++;
        }
    }

    return freeBlocks * BLOCK_SIZE;
}