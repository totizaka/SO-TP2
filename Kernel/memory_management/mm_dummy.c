// #ifdef DUMMY

#include <mm_dummy.h>


#define MAX_BLOCKS 1024                      // Máximo número de bloques
#define MEMORY_ALIGNMENT 8
#define HEADER_MAGIC 0xBEEF
#define HEADER_SIZE ((sizeof(block_header) + MEMORY_ALIGNMENT - 1) & ~(MEMORY_ALIGNMENT - 1))
#define BYTE_SIZE 8
#define BITMAP_SIZE (MAX_BLOCKS / BYTE_SIZE)

typedef struct {
    uint16_t blocks;
    uint16_t magic;
} block_header;

typedef struct memory_manager_cdt {
    uint8_t *baseAddress;
    size_t totalBlocks;
    uint8_t bitmap[BITMAP_SIZE];
} memory_manager_cdt;

void *my_memset(void *ptr, int value, size_t num) {
    unsigned char *p = (unsigned char *)ptr;
    while (num--) {
        *p++ = (unsigned char)value;
    }
    return ptr;
}

static void set_bit(uint8_t *bitmap, size_t i) {
    bitmap[i / BYTE_SIZE] |= (1 << (i % BYTE_SIZE));
}

static void clear_bit(uint8_t *bitmap, size_t i) {
    bitmap[i / BYTE_SIZE] &= ~(1 << (i % BYTE_SIZE));
}

static int is_bit_set(uint8_t *bitmap, size_t i) {
    return bitmap[i / BYTE_SIZE] & (1 << (i % BYTE_SIZE));
}

memory_manager_adt create_memory_manager(void *memory) {
    memory_manager_adt manager = (memory_manager_adt) memory;

    uintptr_t raw_base = (uintptr_t)memory + sizeof(memory_manager_cdt);
    raw_base = (raw_base + MEMORY_ALIGNMENT - 1) & ~(MEMORY_ALIGNMENT - 1);

    manager->baseAddress = (uint8_t *)raw_base;

    size_t usable_bytes = MEMORY_MANAGER_SIZE - (manager->baseAddress - (uint8_t *)memory);
    manager->totalBlocks = usable_bytes / BLOCK_SIZE;

    my_memset(manager->bitmap, 0, BITMAP_SIZE);
    return manager;
}

void *my_malloc(memory_manager_adt manager, size_t size) {
    if (size == 0 || size > manager->totalBlocks * BLOCK_SIZE - HEADER_SIZE) {
        return NULL;
    }

    size_t blocksNeeded = (size + HEADER_SIZE + BLOCK_SIZE - 1) / BLOCK_SIZE;

    for (size_t i = 0; i <= manager->totalBlocks - blocksNeeded; i++) {
        int available = 1;

        for (size_t j = 0; j < blocksNeeded; j++) {
            if (is_bit_set(manager->bitmap, i + j)) {
                available = 0;
                i += j;
                break;
            }
        }

        if (available) {
            for (size_t j = 0; j < blocksNeeded; j++) {
                set_bit(manager->bitmap, i + j);
            }

            uint8_t *blockStart = manager->baseAddress + i * BLOCK_SIZE;
            block_header *header = (block_header *)blockStart;
            header->blocks = blocksNeeded;
            header->magic = HEADER_MAGIC;

            return (void *)(blockStart + HEADER_SIZE);
        }
    }

    return NULL;
}

void my_free(memory_manager_adt manager, void *ptr) {
    if (!ptr || !manager) return;

    uint8_t *start = (uint8_t *)ptr - HEADER_SIZE;

    if ((uintptr_t)start < (uintptr_t)manager->baseAddress ||
        (uintptr_t)start >= (uintptr_t)(manager->baseAddress + manager->totalBlocks * BLOCK_SIZE)) {
        return;
    }

    size_t offset = start - manager->baseAddress;
    if (offset % BLOCK_SIZE != 0) return;

    size_t index = offset / BLOCK_SIZE;
    block_header *header = (block_header *)start;

    if (header->magic != HEADER_MAGIC || header->blocks == 0 || index + header->blocks > manager->totalBlocks)
        return;

    for (size_t i = 0; i < header->blocks; i++) {
        clear_bit(manager->bitmap, index + i);
    }
}

size_t my_get_available_memory(memory_manager_adt manager) {
    if (!manager) return 0;

    size_t freeBlocks = 0;
    for (size_t i = 0; i < manager->totalBlocks; i++) {
        if (!is_bit_set(manager->bitmap, i))
            freeBlocks++;
    }
    return freeBlocks * BLOCK_SIZE;
}

memory_state* my_mem_state(memory_manager_adt manager) {
    if (!manager) return NULL;

    memory_state *state = my_malloc(manager ,sizeof(memory_state));
    state->total_size = MEMORY_MANAGER_SIZE_STATE;
    state->free = my_get_available_memory(manager);
    state->occupied = state->total_size - state->free;
    return state;
}

void my_free_mem_state(memory_manager_adt manager, memory_state *state) {
    if (state) {
        my_free(manager, state);
    }
}

// #endif 
