// /*#ifdef BUDDY
#include <mm_dummy.h> 

#include <stdint.h>
#include <stddef.h>

#define MEMORY_ALIGNMENT 8
#define BYTE_SIZE 8
#define BLOCK_SIZE 64
#define MAX_BLOCKS 1024
#define TOTAL_MEMORY (BLOCK_SIZE * MAX_BLOCKS)
#define TREE_SIZE (MAX_BLOCKS * 2 - 1)  // árbol binario completo
#define ROOT_INDEX 0

#define LEVELS 11  // log2(1024) + 1

#define MEMORY_MANAGER_SIZE (1 << 20) // 1 MB

#define MAX_LEVELS 14 // log2(MEMORY_MANAGER_SIZE / BLOCK_SIZE) + 1
#define BITMAP_SIZE ((1 << MAX_LEVELS) - 1)

typedef enum {
    FREE = 0,
    SPLIT,
    FULL
} block_state;

typedef struct memory_manager_cdt {
    uint8_t *base_address;
    block_state bitmap[BITMAP_SIZE];
    size_t total_size;
    size_t min_block_size;
    size_t free_bytes;
} memory_manager_cdt;

typedef struct memory_manager_cdt *memory_manager_adt;

// Función para crear el manejador de memoria
memory_manager_adt create_memory_manager(void *memory) {
    if (!memory) return NULL;

    memory_manager_cdt *manager = (memory_manager_cdt *)memory;
    manager->base_address = (uint8_t *)memory + sizeof(memory_manager_cdt);
    manager->total_size = MEMORY_MANAGER_SIZE - sizeof(memory_manager_cdt);
    manager->min_block_size = BLOCK_SIZE;
    manager->free_bytes = manager->total_size;

    memset(manager->bitmap, FREE, sizeof(manager->bitmap));

    return manager;
}

// Función para obtener el nivel correspondiente al tamaño solicitado
static int get_level(memory_manager_adt manager, size_t size) {
    size_t total_blocks = manager->total_size / manager->min_block_size;
    size_t blocks_needed = (size + manager->min_block_size - 1) / manager->min_block_size;
    int level = 0;
    size_t blocks_in_level = total_blocks;

    while (blocks_in_level > 1 && blocks_in_level > blocks_needed) {
        blocks_in_level /= 2;
        level++;
    }

    return level;
}

// Función para calcular el índice en el bitmap
static int get_index(int level, int offset) {
    return (1 << level) - 1 + offset;
}

// Función recursiva para asignar memoria
static void *allocate(memory_manager_adt manager, int level, int offset, int target_level) {
    int index = get_index(level, offset);

    if (manager->bitmap[index] == FULL) {
        return NULL;
    }

    if (level == target_level) {
        if (manager->bitmap[index] == FREE) {
            manager->bitmap[index] = FULL;
            size_t block_size = manager->total_size >> level;
            size_t block_offset = offset * block_size;
            manager->free_bytes -= block_size;
            return manager->base_address + block_offset;
        } else {
            return NULL;
        }
    }

    if (manager->bitmap[index] == FREE) {
        manager->bitmap[index] = SPLIT;
    }

    void *left = allocate(manager, level + 1, offset * 2, target_level);
    if (left) return left;

    void *right = allocate(manager, level + 1, offset * 2 + 1, target_level);
    if (right) return right;

    // Si no se pudo asignar en ninguno de los hijos, revertir el estado
    if (manager->bitmap[get_index(level + 1, offset * 2)] == FREE &&
        manager->bitmap[get_index(level + 1, offset * 2 + 1)] == FREE) {
        manager->bitmap[index] = FREE;
    }

    return NULL;
}

// Función para asignar memoria
void *my_malloc(memory_manager_adt manager, size_t size) {
    if (!manager || size == 0 || size > manager->free_bytes) return NULL;

    int level = get_level(manager, size);
    return allocate(manager, 0, 0, level);
}

// Función recursiva para liberar memoria
static void deallocate(memory_manager_adt manager, int level, int offset) {
    int index = get_index(level, offset);

    if (manager->bitmap[index] == FULL) {
        manager->bitmap[index] = FREE;
        size_t block_size = manager->total_size >> level;
        manager->free_bytes += block_size;
    } else if (manager->bitmap[index] == SPLIT) {
        deallocate(manager, level + 1, offset * 2);
        deallocate(manager, level + 1, offset * 2 + 1);

        int left = get_index(level + 1, offset * 2);
        int right = get_index(level + 1, offset * 2 + 1);

    if (manager->bitmap[left] == FREE && manager->bitmap[right] == FREE) {
        manager->bitmap[index] = FREE;
        size_t block_size = manager->total_size >> level;
        manager->free_bytes += block_size;
    }
    }
}

// Función para liberar memoria
void my_free(memory_manager_adt manager, void *ptr) {
    if (!manager || !ptr) return;

    size_t offset = (uint8_t *)ptr - manager->base_address;
    if (offset >= manager->total_size) return;

    int level = 0;
    size_t block_size = manager->total_size;

    while (offset % block_size != 0 && block_size >= manager->min_block_size) {
        block_size /= 2;
        level++;
    }

int node_index = get_index(level, offset / block_size);
deallocate(manager, level, offset / block_size);
}

// Función para obtener la memoria disponible
size_t my_get_available_memory(memory_manager_adt manager) {
    if (!manager) return 0;
    return manager->free_bytes;
}

// #endif*/