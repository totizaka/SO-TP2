#ifndef _PHILOS_H
#define _PHILOS_H

#include <stdlib_user.h>

#define ADD 'a'
#define REMOVE 'r'

#define MAX_PHILOS 10
#define MIN_PHILOS 5

#define EATING_TIME 10
#define THINKING_TIME 20

typedef enum {
    THINKING,
    EATING,
}philo_state;

typedef struct philo_t{
    int64_t pid;
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
int new_philo(int i);
void print_instructions();
int open_mutexes();
void close_mutexes();
void clean_resources();
void keyboard_handler();
void think();
void take_forks(int i);
void eat(int i);
void put_forks(uint64_t i);
int add_philo();
int remove_philo();
void print_state();

#endif

