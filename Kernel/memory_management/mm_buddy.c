/*#ifdef BUDDY
#include <mm_dummy.h> 

#define MEMORY_ALIGNMENT 8
#define BYTE_SIZE 8
#define BLOCK_SIZE 64
#define MAX_BLOCKS 1024
#define TOTAL_MEMORY (BLOCK_SIZE * MAX_BLOCKS)
#define TREE_SIZE (MAX_BLOCKS * 2 - 1)  // árbol binario completo
#define ROOT_INDEX 0

// Estados de nodos
#define FREE 0
#define SPLIT 1
#define FULL 2

#define LEVELS 11  // log2(1024) + 1

typedef struct memory_manager_cdt {
    uint8_t *baseAddress;
    char tree_bitmap[TREE_SIZE];
} memory_manager_cdt;


void *my_memset(void *ptr, int value, size_t num) {
    unsigned char *p = (unsigned char *)ptr;
    while (num--) {
        *p++ = (unsigned char)value;
    }
    return ptr;
}


static int get_level(size_t size) {
    size_t block = BLOCK_SIZE;
    int level = LEVELS - 1;
    while (size <= block && level > 0) {
        block >>= 1;
        level--;
    }
    return level;
}

static int index_for_level_offset(int level, int offset) {
    return (1 << level) - 1 + offset;
}

static void *alloc_block(memory_manager_adt m, int index, int level, int target_level) {
    if (m->tree_bitmap[index] == FULL)
        return NULL;

    if (level == target_level) {
        if (m->tree_bitmap[index] == FREE) {
            m->tree_bitmap[index] = FULL;
            int offset = index - ((1 << level) - 1);
            size_t block_size = TOTAL_MEMORY >> level;
            return m->baseAddress + offset * block_size;
        }
        return NULL;
    }

    if (m->tree_bitmap[index] == FREE)
        m->tree_bitmap[index] = SPLIT;

    int left = 2 * index + 1;
    void *result = alloc_block(m, left, level + 1, target_level);
    if (result != NULL) return result;

    int right = left + 1;
    return alloc_block(m, right, level + 1, target_level);
}

static int get_offset(void *addr, int level, memory_manager_adt manager) {
    size_t block_size = TOTAL_MEMORY >> level;
    return ((uintptr_t)addr - (uintptr_t)manager->baseAddress) / block_size;
}

static void update_free_status(memory_manager_adt m, int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        int left = 2 * parent + 1;
        int right = left + 1;
        if (m->tree_bitmap[left] == FREE && m->tree_bitmap[right] == FREE)
            m->tree_bitmap[parent] = FREE;
        else if (m->tree_bitmap[left] == FULL && m->tree_bitmap[right] == FULL)
            m->tree_bitmap[parent] = FULL;
        else
            m->tree_bitmap[parent] = SPLIT;
        index = parent;
    }
}

void *my_malloc(memory_manager_adt m, size_t size) {
    if (size == 0 || size > TOTAL_MEMORY)
        return NULL;
    int level = get_level(size);
    return alloc_block(m, ROOT_INDEX, 0, level);
}

void my_free(memory_manager_adt m, void *ptr) {
    if (!ptr) return;
    for (int level = LEVELS - 1; level >= 0; level--) {
        int offset = get_offset(ptr, level, m);
        int index = index_for_level_offset(level, offset);
        size_t block_size = TOTAL_MEMORY >> level;
        if (m->baseAddress + offset * block_size == ptr) {
            m->tree_bitmap[index] = FREE;
            update_free_status(m, index);
            return;
        }
    }
}

size_t my_get_available_memory(memory_manager_adt m) {
    size_t total = 0;
    for (int i = 0; i < TREE_SIZE; i++) {
        if (m->tree_bitmap[i] == FREE) {
            int level = 0;
            int idx = i;
            while (idx > 0) {
                idx = (idx - 1) / 2;
                level++;
            }
            total += TOTAL_MEMORY >> level;
        }
    }
    return total;
}

memory_manager_adt create_memory_manager(void *memory) {
    memory_manager_adt m = (memory_manager_adt)memory;
    uintptr_t raw_base = (uintptr_t)memory + sizeof(memory_manager_cdt);
    raw_base = (raw_base + MEMORY_ALIGNMENT - 1) & ~(MEMORY_ALIGNMENT - 1);
    m->baseAddress = (uint8_t *)raw_base;
    my_memset(m->tree_bitmap, 0, TREE_SIZE);
    return m;
}

#endif*/