// #include <stdint.h>
// #include "mm_dummy.h"

// #define MEMORY_ALIGNMENT 8//Creo que no es necesario

// #define BYTE_SIZE 8
// #define BLOCK_SIZE 64
// #define MAX_BLOCKS 1024
// #define TREE_SIZE MAX_BLOCKS*2

// typedef struct MemoryManagerCDT {
//     uint8_t *baseAddress;             // Puntero al bloque de memoria administrado
//     char tree_bitmap[TREE_SIZE];
// } MemoryManagerCDT;

// memory_manager_ADT createMemoryManager(void *const restrict memoryForMemoryManager,
//     void *const restrict managedMemory,
//     size_t managedMemorySize){
//         MemoryManagerCDT mm;
//         mm.baseAddress=memoryForMemoryManager;
//         //falta terminar
//     }

// void *my_malloc(memory_manager_ADT manager);

// void my_free(memory_manager_ADT manager, void *ptr);

// size_t my_get_available_memory(memory_manager_ADT manager);