// #ifdef BUDDY

// #include <mm_dummy.h> 

// #include <stdint.h>
// #include <stddef.h>

// #define MIN_ORDER 4  // 2^4 = 16 bytes
// #define MAX_ORDER 20 // 2^20 = 1048576 bytes
// #define ORDER_COUNT (MAX_ORDER - MIN_ORDER + 1)

// typedef struct block {
//   struct block *next, *prev;
//   uint8_t order;
//   uint8_t free;
// } block_t;

// typedef struct memory_manager_cdt {
//   void *base;
//   block_t *orders[ORDER_COUNT];
//   size_t total_size;
// } memory_manager_cdt;

// static int get_order(size_t size) {
//     size_t total = size + sizeof(block_t);
//     int order = MIN_ORDER;
//     while ((1UL << order) < total && order <= MAX_ORDER)
//         order++;
//     return order > MAX_ORDER ? -1 : order;
// }

// static block_t *buddy(block_t *node, void *base, uint8_t order) {
//     uintptr_t offset = (uintptr_t)node - (uintptr_t)base;
//     uintptr_t buddy_offset = offset ^ (1UL << order);
//     return (block_t *)((uintptr_t)base + buddy_offset);
// }

// static void remove_block(block_t **list, block_t *blk) {
//     if (!blk)
//         return;

//     if (blk->prev)
//         blk->prev->next = blk->next;
//     if (blk->next)
//         blk->next->prev = blk->prev;
//     if (*list == blk)
//         *list = blk->next;

//     blk->next = blk->prev = NULL;
// }

// static void insert_block(block_t **list, block_t *blk) {
//     if (!blk)
//         return;

//     blk->next = *list;
//     blk->prev = NULL;
//     if (*list)
//         (*list)->prev = blk;
//     *list = blk;
//     blk->free = 1;
// }

// static int check_list_cycles(block_t *head) {
//     block_t *slow = head, *fast = head;
//     while (fast && fast->next) {
//         slow = slow->next;
//         fast = fast->next->next;
//         if (slow == fast) {
//             // Ciclo detectado
//             return 1;
//         }
//     }
//     return 0;
// }

// static void debug_print_lists(memory_manager_adt manager) {
//     for (int i = 0; i < ORDER_COUNT; i++) {
//         block_t *curr = manager->orders[i];
//         if (check_list_cycles(curr)) {
//             draw_word(0xFFFFFF ,"Loop detected in free list of order \n");
//         }
//     }
// }

// memory_manager_adt create_memory_manager(void *memory) {
//     memory_manager_adt manager = (memory_manager_adt)memory;

//     uintptr_t raw_base = (uintptr_t)memory + sizeof(memory_manager_cdt);
//     uintptr_t aligned_base = (raw_base + ((1UL << MIN_ORDER) - 1)) & ~((uintptr_t)((1UL << MIN_ORDER) - 1));

//     manager->base = (void *)aligned_base;
//     manager->total_size = MEMORY_MANAGER_SIZE - (aligned_base - (uintptr_t)memory);

//     for (int i = 0; i < ORDER_COUNT; i++)
//         manager->orders[i] = NULL;

//     // Calcular el orden máximo que entra en el espacio disponible
//     int max_possible_order = MAX_ORDER;
//     while ((1UL << max_possible_order) > manager->total_size && max_possible_order > MIN_ORDER)
//         max_possible_order--;

//     block_t *initial = (block_t *)manager->base;
//     initial->order = max_possible_order - MIN_ORDER;
//     initial->free = 1;
//     initial->next = initial->prev = NULL;

//     manager->orders[initial->order] = initial;

//     return manager;
// }

// void *my_malloc(memory_manager_adt manager, size_t size) {
//     int target_order = get_order(size);
//     if (target_order < 0)
//         return NULL;

//     int o;
//     for (o = target_order - MIN_ORDER; o < ORDER_COUNT; o++) {
//         block_t *blk = manager->orders[o];
//         if (!blk)
//             continue;

//         remove_block(&manager->orders[o], blk);

//         // Ir dividiendo hasta llegar al tamaño deseado
//         while (o > target_order - MIN_ORDER) {
//             o--;

//             size_t split_size = 1UL << (o + MIN_ORDER);
//             block_t *buddy_blk = (block_t *)((uint8_t *)blk + split_size);

//             buddy_blk->order = o;
//             buddy_blk->free = 1;
//             buddy_blk->next = buddy_blk->prev = NULL;

//             insert_block(&manager->orders[o], buddy_blk);

//             blk->order = o;
//         }

//         blk->free = 0;
//         blk->next = blk->prev = NULL;

//         return (void *)(blk + 1); // Retornar después del header
//     }

//     return NULL;
// }

// void my_free(memory_manager_adt manager, void *ptr) {
//     if (!ptr)
//         return;

//     block_t *blk = (block_t *)ptr - 1;
//     blk->free = 1;

//     while (blk->order + 1 < ORDER_COUNT) {
//         block_t *b = buddy(blk, manager->base, blk->order + MIN_ORDER);

//         // Verificar que el buddy esté dentro de los límites del heap
//         if (!b)
//             break;

//         uintptr_t base = (uintptr_t)manager->base;
//         uintptr_t buddy_addr = (uintptr_t)b;
//         size_t block_size = 1UL << (blk->order + MIN_ORDER);
//         if (buddy_addr < base || buddy_addr + block_size > base + manager->total_size)
//             break;

//         if (!b->free || b->order != blk->order)
//             break;

//         remove_block(&manager->orders[b->order], b);
//         if (b < blk)
//             blk = b;

//         blk->order++;
//     }

//     insert_block(&manager->orders[blk->order], blk);
// }

// size_t my_get_available_memory(memory_manager_adt manager) {
//     if (!manager) return 0;

//     size_t total_free = 0;

//     for (int i = 0; i < ORDER_COUNT; i++) {
//         block_t *curr = manager->orders[i];
//         size_t block_size = 1UL << (i + MIN_ORDER);

//         while (curr) {
//             if (curr->free) {
//                 // Resta el tamaño del header del bloque total
//                 size_t usable = block_size > sizeof(block_t) ? block_size - sizeof(block_t) : 0;
//                 total_free += usable;
//             }
//             curr = curr->next;
//         }
//     }

//     return total_free;
// }

// memory_state* my_mem_state(memory_manager_adt manager) {
//     if (!manager) return NULL;

//     memory_state *state = my_malloc(manager ,sizeof(memory_state));
//     state->total_size = MEMORY_MANAGER_SIZE_STATE;
//     state->free = my_get_available_memory(manager);
//     state->occupied = state->total_size - state->free;
//     return state;
// }

// void my_free_mem_state(memory_manager_adt manager, memory_state *state) {
//     if (state) {
//         my_free(manager, state);
//     }
// }

// #endif