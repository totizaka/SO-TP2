#ifndef _PIPE_H
#define _PIPE_H

#include <semaphores.h>

//ELEGIR BIEN LOS NUEMROS(PUSE CUALQUIERA)
#define MAX_PIPES 50



int pipe_write( int64_t id, char* buffer, uint64_t numBytes );
int pipe_read(int64_t id, char* buffer, int numBytes);



#endif