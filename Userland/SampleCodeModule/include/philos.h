#ifndef _PHILOS_H
#define _PHILOS_H

#include <stdlib_user.h>

#define ADD 'a'
#define REMOVE 'r'
#define STATE 's'

#define MAX_PHILOS 10
#define MIN_PHILOS 5
#define TICKS_PER_SECOND 18
#define EATING_TIME (2 * TICKS_PER_SECOND)
#define THINKING_TIME (3 * TICKS_PER_SECOND)

typedef enum {
    THINKING,
    EATING,
}philo_state;

typedef struct philo_t{
    int64_t pid; //proceso
    philo_state state;
    int64_t left_fork;
    int64_t right_fork;

}philo_t;

typedef struct table_t
{
    philo_t philos_array[MAX_PHILOS];
    uint8_t amount;
    int64_t mutex;
    int64_t border_mutex;
    fd_t fds[FD_MAX];

}table_t;


int64_t philos(char **argv, uint64_t argc);
#endif

