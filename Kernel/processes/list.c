#include <list.h>

typedef struct node {
    list_elem_t head; 
    struct node * tail;
}t_node;

typedef struct list_cdt {
    t_node * pre_next;
    size_t size;
    t_cmp cmp;
}list_cdt;

list_adt new_list(t_cmp cmp){
    list_adt aux = my_malloc(get_memory_manager() ,sizeof(*aux));
    if(aux == NULL){
        return NULL;
    }
    aux->size = 0;
    aux->pre_next = NULL;
    aux->cmp = cmp;
    return aux;
}

int8_t free_list(list_adt list){
    if(list == NULL){
        return 0;
    }
    for(int i = 0; i < list->size; i++){
        t_node * aux = list->pre_next;
        list->pre_next = list->pre_next->tail;
        my_free(get_memory_manager(), aux);
    }
    my_free(get_memory_manager(), list);
    return 0;
}

int8_t add_list(list_adt list, list_elem_t elem){
    if (list == NULL) {
        return -1;
    }

    t_node * new_node = my_malloc(get_memory_manager() ,sizeof(*new_node));
    if (new_node == NULL) {
        return -1; 
    }

    new_node->head = elem;
   
    if (list->pre_next == NULL) {
       list->pre_next = new_node;
       list->pre_next->tail = new_node; 
    } else {
       new_node->tail = list->pre_next->tail;
       list->pre_next->tail = new_node;
       list->pre_next = new_node;
    } 

    list->size++;
    return 0;
}

int8_t remove_list(list_adt list, list_elem_t elem){
    if (is_empty(list))
    {
        return -1;
    }
    if (list->size == 1)
    {
        if (list->cmp(elem, list->pre_next->head) != 0)
        {
            return -1;
        }
        my_free(get_memory_manager(), list->pre_next);
        list->pre_next = NULL;
        list->size = 0;
        return 0;
    }

    t_node * ant = list->pre_next;
    t_node * current = list->pre_next->tail;
    for(int i=0; i<list->size ; i++){
        if(list->cmp(elem, current->head) == 0){
            ant->tail = current->tail;
            if(list->pre_next == current){
                list->pre_next = ant;
            }
            my_free(get_memory_manager(), current);
            list->size -- ;
            return 0;
        }
        ant = ant->tail;
        current = current->tail;
    }
    return -1;
}

int8_t list_size(list_adt list){
    if(list == NULL){
        return -1;
    }
    return list->size;
}

int8_t is_empty(list_adt list){
    return list == NULL || list->pre_next == NULL;
}

list_elem_t next(list_adt list){
    if (is_empty(list)) {
        return NULL;
    }

    list_elem_t aux = list->pre_next->tail->head;
    list->pre_next = list->pre_next->tail;
    return aux;
}