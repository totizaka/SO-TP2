#ifndef _PHILOS_H
#define _PHILOS_H

#include <stdlib_user.h>

#define ADD 'a'
#define REMOVE 'r'
#define MAX_PHILOS 10
#define MIN_PHILOS 5
#define THINKING_TIME 1000
#define EATING_TIME 100

typedef enum {
    FREE=0,
    THINKING,
    EATING,
    HUNGRY
}philo_state;

#define SLEEP_CONSTANT 3
#define THINK_CONSTANT 1
#define GET_UNIFORM_CONSTANT 3
int64_t philos ( char ** argv, int argc );

#endif