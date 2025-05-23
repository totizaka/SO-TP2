#include <list.h>

typedef struct list_cdt {
    struct node * first;
    uint32_t size;
    struct node* it;
    t_cmp cmp;
}list_cdt;

typedef struct node {
    list_elem_t elem; 
    struct node * tail;
}node;

list_adt new_list(t_cmp cmp){
    list_adt to_ret = my_malloc(get_memory_manager(), sizeof(list_cdt));
    if(to_ret == NULL){
        return NULL;
    }
    to_ret->size=0;
    to_ret->first=NULL;
    to_ret->it=NULL;
    to_ret->cmp = cmp;
    return to_ret;
}
list_elem_t remove_first(list_adt list){
    if(list == NULL || list->first == NULL){
        return NULL;
    }
    node * aux_node = list->first;
    list_elem_t aux = aux_node->elem;
    list->first = aux_node->tail;
    my_free(get_memory_manager(), aux_node);
    list->size--;
    return aux;
}
int8_t free_list(list_adt list){
    if(list == NULL){ 
        return -1;
    }
    node *current = list->first;
    while(current != NULL){
        node * aux = current->tail;
        my_free(get_memory_manager(), current);
        current = aux;
    }
    my_free(get_memory_manager(), list);
    return 0;
}

int8_t add_list(list_adt list, list_elem_t elem){
    if ( list == NULL ) {
		return -1;
	}
    node* aux = my_malloc(get_memory_manager(), sizeof(node));
    if(aux == NULL){
        return -1;
    }
    aux->elem = elem;
    aux->tail = NULL;
    
    if(list->first == NULL){
        list->first = aux;
    }
    else{
        node * current= list->first;
        while(current->tail != NULL){
            current = current->tail;
        }
        current->tail = aux;
    }
    list->size++;
    return 0;
}

int8_t remove_list(list_adt list, list_elem_t elem){
    if(list == NULL || list->first == NULL){
        return -1;
    }
    node * current = list->first;
    node * previous = NULL;
    while(current != NULL){
        if(list->cmp(current->elem, elem)==0){
            if (previous == NULL){
                list->first = current->tail;
            }
            else{
                previous->tail = current->tail;
            }
            my_free(get_memory_manager(),current);
            list->size--;
            return 1;
        }
        previous = current;
        current = current->tail;
    }
    return 0;
}

int8_t list_size(list_adt list){
    if ( list == NULL ) {
		return -1;
	}
	return list->size;
}

int8_t is_empty(list_adt list){
    return list == NULL || list->first == NULL;
}

void to_begin(list_adt list){
    if(list==NULL){
        return;
    }
    list->it=list->first;
}
list_elem_t next(list_adt list){
    if(list == NULL || list->it == NULL){
        return NULL;
    }
    list_elem_t to_ret = list->it->elem;
    list->it = list->it->tail;
    return to_ret;
}

int8_t has_next(list_adt list){
    return (list != NULL && list->it != NULL);
}