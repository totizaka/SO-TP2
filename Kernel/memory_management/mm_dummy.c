#include "mm_dummy.h"
#include <stddef.h>
#include <stdint.h>
#include <string.h> 


#define MEMORY_ALIGNMENT 8
#define MAX_BLOCKS 1024
#define BYTE_SIZE 8
#define BITMAP_SIZE (MAX_BLOCKS / BYTE_SIZE)
#define HEADER_SIZE sizeof(block_header)

typedef struct {
    uint16_t blocks; // cantidad de bloques asignados
} block_header;

typedef struct MemoryManagerCDT {
    uint8_t *baseAddress;             // Puntero al bloque de memoria administrado
    uint8_t bitmap[BITMAP_SIZE];      // Un bit por cada bloque
    size_t totalBlocks;               // Total de bloques
} MemoryManagerCDT;


//Funciones de manipulación de bits

static void set_bit(uint8_t *bitmap, size_t index) {
    bitmap[index / BYTE_SIZE] |= (1 << (index % BYTE_SIZE));
}

static void clear_bit(uint8_t *bitmap, size_t index) {
    bitmap[index / BYTE_SIZE] &= ~(1 << (index % BYTE_SIZE));
}

static int is_bit_set(uint8_t *bitmap, size_t index) {
    return bitmap[index / BYTE_SIZE] & (1 << (index % BYTE_SIZE));
}


memory_manager_ADT createMemoryManager(void *memoryForMemoryManager) {
    memory_manager_ADT manager = (memory_manager_ADT) memoryForMemoryManager;

    manager->baseAddress = (uint8_t *) memoryForMemoryManager + sizeof(MemoryManagerCDT);
    manager->totalBlocks = MEMORY_MANAGER_SIZE / BLOCK_SIZE;

    memset(manager->bitmap, 0, BITMAP_SIZE);
    return manager;
}

void *my_malloc(memory_manager_ADT manager, size_t size) {
    if (size == 0 || size > manager->totalBlocks * BLOCK_SIZE - HEADER_SIZE){
        return NULL;
    }

    size_t blocksNeeded = (size + HEADER_SIZE + BLOCK_SIZE - 1) / BLOCK_SIZE;

    for (size_t i = 0; i <= manager->totalBlocks - blocksNeeded; i++) {
        int found = 1;
        for (size_t j = 0; j < blocksNeeded; j++) {
            if (is_bit_set(manager->bitmap, i + j)) {
                found = 0;
                i += j; // optimización: salta los bloques ocupados
                break;
            }
        }

        if (found) {
            for (size_t j = 0; j < blocksNeeded; j++)
                set_bit(manager->bitmap, i + j);

            uint8_t *blockStart = manager->baseAddress + i * BLOCK_SIZE;
            block_header *header = (block_header *) blockStart;
            header->blocks = blocksNeeded;
            return (void *)(blockStart + HEADER_SIZE);
        }
    }

    return NULL;
}

void my_free(memory_manager_ADT manager, void *ptr) {
    if (!ptr) return;

    uint8_t *blockStart = (uint8_t *)ptr - HEADER_SIZE;

    // Verifica que el puntero esté dentro del rango de memoria administrado
    if ((uintptr_t)blockStart < (uintptr_t)manager->baseAddress || 
        (uintptr_t)blockStart >= (uintptr_t)(manager->baseAddress + manager->totalBlocks * BLOCK_SIZE)){
        return;
    }

    size_t offset = blockStart - manager->baseAddress;
    if (offset % BLOCK_SIZE != 0){
        return;
    }

    size_t blockIndex = offset / BLOCK_SIZE;
    if (blockIndex >= manager->totalBlocks){
        return;
    }

    block_header *header = (block_header *) blockStart;
    size_t blocksToFree = header->blocks;

    if (blocksToFree == 0 || blockIndex + blocksToFree > manager->totalBlocks){
        return;
    }

    for (size_t i = 0; i < blocksToFree; i++){
        clear_bit(manager->bitmap, blockIndex + i);
    }
}

size_t my_get_available_memory(memory_manager_ADT manager) {
    size_t freeBlocks = 0;

    for (size_t i = 0; i < manager->totalBlocks; i++) {
        if (!is_bit_set(manager->bitmap, i))
            freeBlocks++;
    }

    return freeBlocks * BLOCK_SIZE;
}
