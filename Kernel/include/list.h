#ifndef LIST_H_
#define LIST_H_

#include <stdint.h>
#include <defs.h>
#include <stddef.h>
#include <memory_manager.h>
#include <pcb.h>


typedef struct list_cdt * list_adt;
typedef struct PCB* list_elem_t; 

typedef int (*t_cmp)(list_elem_t e1, list_elem_t e2);

list_adt new_list(t_cmp cmp);
int8_t free_list(list_adt list);
int8_t add_list(list_adt list, list_elem_t elem);
int8_t remove_list(list_adt list, list_elem_t elem);
int8_t list_size(list_adt list);
int8_t is_empty(list_adt list);
void to_begin(list_adt list);
list_elem_t next(list_adt list);
int8_t has_next(list_adt list);
list_elem_t remove_first(list_adt list);
#endif