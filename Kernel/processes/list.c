#include <list.h>

typedef struct node {
    list_elem_t head;
    struct node *next;
} t_node;

typedef struct list_cdt {
    t_node *tail;      // Apunta al último nodo (el más nuevo)
    t_node *iter;      // Iterador para next()
    size_t size;
    t_cmp cmp;
} list_cdt;

// Crea una nueva lista (cola circular vacía)
list_adt new_list(t_cmp cmp) {
    list_adt aux = my_malloc(get_memory_manager(), sizeof(*aux));
    if (aux == NULL) {
        return NULL;
    }
    aux->size = 0;
    aux->tail = NULL;
    aux->iter = NULL;
    aux->cmp = cmp;
    return aux;
}

// Libera todos los nodos y la lista
int8_t free_list(list_adt list) {
    if (list == NULL || list->tail == NULL) {
        if (list != NULL)
            my_free(get_memory_manager(), list);
        return 0;
    }
    t_node *curr = list->tail->next;
    t_node *next;
    for (size_t i = 0; i < list->size; i++) {
        next = curr->next;
        my_free(get_memory_manager(), curr);
        curr = next;
    }
    my_free(get_memory_manager(), list);
    return 0;
}

// Agrega un elemento al final de la cola circular
int8_t add_list(list_adt list, list_elem_t elem) {
    if (list == NULL) {
        return -1;
    }
    t_node *new_node = my_malloc(get_memory_manager(), sizeof(*new_node));
    if (new_node == NULL) {
        return -1;
    }
    new_node->head = elem;
    if (list->tail == NULL) {
        new_node->next = new_node;
        list->tail = new_node;
        list->iter = new_node; // El iterador apunta al primer nodo agregado
    } else {
        new_node->next = list->tail->next;
        list->tail->next = new_node;
        list->tail = new_node;
    }
    list->size++;
    return 0;
}

// Elimina el primer nodo que coincida con elem
int8_t remove_list(list_adt list, list_elem_t elem) {
    if (is_empty(list)) {
        return -1;
    }
    t_node *curr = list->tail->next;
    t_node *prev = list->tail;
    for (size_t i = 0; i < list->size; i++) {
        if (list->cmp(elem, curr->head) == 0) {
            if (curr == prev) { // Solo un nodo
                list->tail = NULL;
                list->iter = NULL;
            } else {
                prev->next = curr->next;
                if (curr == list->tail)
                    list->tail = prev;
                if (curr == list->iter)
                    list->iter = curr->next;
            }
            my_free(get_memory_manager(), curr);
            list->size--;
            return 0;
        }
        prev = curr;
        curr = curr->next;
    }
    return -1;
}

int8_t list_size(list_adt list) {
    if (list == NULL) {
        return -1;
    }
    return list->size;
}

int8_t is_empty(list_adt list) {
    return list == NULL || list->tail == NULL;
}

// Inicializa el iterador al primer elemento (llamar antes de iterar si se quiere empezar de nuevo)
void reset_iter(list_adt list) {
    if (is_empty(list)) {
        list->iter = NULL;
    } else {
        list->iter = list->tail->next;
    }
}

// Devuelve el elemento apuntado por el iterador y lo avanza (rotación de la cola)
list_elem_t next(list_adt list) {
    if (is_empty(list)) {
        return NULL;
    }
    if (list->iter == NULL) {
        list->iter = list->tail->next;
    } else {
        list->iter = list->iter->next;
    }
    return list->iter->head;
}