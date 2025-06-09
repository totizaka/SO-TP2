#include <test_sync.h>

#define SEM_ID 1
#define TOTAL_PAIR_PROCESSES 2

int64_t global; // shared memory

void slowInc(int64_t *p, int64_t inc) {
  int64_t aux = *p;
  my_yield(); // This makes the race condition highly probable
  aux += inc;
  *p = aux;
}


uint64_t my_process_inc( char *argv[], uint64_t argc) {
  print("test_sync: process-inc\n", 50);
  uint64_t n;
  int8_t inc;
  int8_t use_sem;

  if (argc != 3)
    return -1;

  if ((n = satoi(argv[0])) <= 0)
    return -1;
  if ((inc = satoi(argv[1])) == 0)
    return -1;
  if ((use_sem = satoi(argv[2])) < 0)
    return -1;

  if (use_sem)
    if (sem_open(SEM_ID, 1)==-1) {
      print("test_sync: ERROR opening semaphore\n", 50);
      return -1;
    }

  uint64_t i;
  for (i = 0; i < n; i++) {
    if (use_sem)
      sem_wait(SEM_ID);
    slowInc(&global, inc);
    if (use_sem)
      sem_post(SEM_ID);
  }

  if (use_sem)
    sem_close(SEM_ID);

  return 0;
}

uint64_t test_sync(char *argv[], uint64_t argc) { 
  uint64_t pids[2 * TOTAL_PAIR_PROCESSES];

  if (argc != 2)
    return -1;

  char *argvDec[] = {"process_inc" ,argv[0], "-1", argv[1], NULL};
  char *argvInc[] = {"process_inc" ,argv[0], "1", argv[1], NULL};

  global = 0;

  uint64_t i;
    fd_t fds[FD_MAX]={STDIN,STDOUT,STDERR};

  
  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
    pids[i] = my_create_process((void(*))my_process_inc, argvDec, 4, 0, fds);
    pids[i + TOTAL_PAIR_PROCESSES] = my_create_process((void(*))my_process_inc, argvInc, 4, 0, fds);
    print("test_sync: enloqueciendo\n", 50);
  }

  int64_t* res= my_malloc(4);
  int64_t*res2=  my_malloc(4);
  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
    my_wait(pids[i],res );
    my_wait(pids[i + TOTAL_PAIR_PROCESSES],res2 );
  }
  my_free(res);
  my_free(res2);

  print("Final value: ", 15);
  char s[4];
  itoa( global, s);
  print(s, 4);
  return 0;
}