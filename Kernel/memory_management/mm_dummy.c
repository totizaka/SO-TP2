#include "mm_dummy.h"
#include <stddef.h>
#include <stdint.h>
#include <string.h> 


#define BLOCK_COUNT ((HEAP_SIZE) / BLOCK_SIZE)


typedef struct {
	void *start;
	int64_t current;
	void *free_ptrs[BLOCK_COUNT];
} memory_manager_cdt;




memory_manager_adt my_mm_init ( void *p )
{
	memory_manager_cdt * aux = ( memory_manager_cdt * ) p;

	for ( int i = 0; i < BLOCK_COUNT; i++ ) {
		aux->free_ptrs[i] = ( void * ) ( ( char * ) p + i * BLOCK_SIZE );
	}

	aux->start = ( void * ) ( ( char * ) p );
	aux->current = 0;
	for ( int i = 0; i <= ( sizeof ( memory_manager_cdt ) / BLOCK_SIZE ); i++ ) {
		my_malloc ( ( memory_manager_adt ) aux, 1 );														//reserves the space for the CDT
	}

	return ( memory_manager_adt ) aux;
}

void * my_malloc (memory_manager_adt mem, uint64_t size )
{
	memory_manager_cdt * aux = ( memory_manager_cdt * ) mem;
	if ( aux == NULL || size > BLOCK_SIZE || aux->current >= BLOCK_COUNT ) {
		return NULL;
	}

	return aux->free_ptrs[aux->current++];
}



void my_free ( void * p, memory_manager_adt mem )
{
	memory_manager_cdt * aux = ( memory_manager_cdt * ) mem;
	if ( aux == NULL ) {
		return;
	}
	aux->current--;
	aux->free_ptrs[aux->current] = p;
}

/*int64_t my_mem_info ( memory_info * info, memory_manager_adt mem )
{
	memory_manager_cdt * aux = ( memory_manager_cdt * ) mem;
	if ( aux == NULL || info == NULL ) {
		return -1;
	}
	info->total_size = HEAP_SIZE;
	info->free = ( ( BLOCK_COUNT - aux->current ) * BLOCK_SIZE );
	return 0;
}*/
