#include <test_sync.h>

#define SEM_ID 1
#define TOTAL_PAIR_PROCESSES 2

int64_t global; // shared memory

void slowInc(int64_t *p, int64_t inc) {
  uint64_t aux = *p;
  yield(); // This makes the race condition highly probable
  aux += inc;
  *p = aux;
}

uint64_t my_process_inc1() {

  draw_word(0xFFFFFF, "my_process_inc1: Starting process\n");

  uint64_t n;
  int8_t inc;
  int8_t use_sem;

  // if (argc != 3)
  //   return -1;

  // if ((n = satoi(argv[0])) <= 0)
  //   return -1;
  // if ((inc = satoi(argv[1])) == 0)
  //   return -1;
  // if ((use_sem = satoi(argv[2])) < 0)
  //   return -1;
  
  n = 10;
  inc = 1;
  use_sem = 1;

  if (use_sem)
    if (!my_sem_open(SEM_ID, 1, 1,0)) {
      draw_word(0xFFFFFF ,"test_sync: ERROR opening semaphore\n");
      return -1;
    }

  uint64_t i;
  for (i = 0; i < n; i++) {
    if (use_sem)
      my_sem_wait(SEM_ID);
    slowInc(&global, inc);
    if (use_sem)
      my_sem_post(SEM_ID);
  }

  if (use_sem)
    my_sem_close(SEM_ID);

        draw_word(0xFFFFFF ,"termino1\n");
  return 0;
}

uint64_t my_process_inc2() {

  draw_word(0xFFFFFF, "my_process_inc2\n");

  uint64_t n;
  int8_t inc;
  int8_t use_sem;

  // if (argc != 3)
  //   return -1;

  // if ((n = satoi(argv[0])) <= 0)
  //   return -1;
  // if ((inc = satoi(argv[1])) == 0)
  //   return -1;
  // if ((use_sem = satoi(argv[2])) < 0)
  //   return -1;
  
  n = 10;
  inc = -1;
  use_sem = 1;

  if (use_sem)
    if (my_sem_open(SEM_ID, 1, 1, 0)==-1) {
      draw_word(0xFFFFFF ,"test_sync: ERROR opening semaphore\n");
      return -1;
    }

  uint64_t i;
  for (i = 0; i < n; i++) {
    if (use_sem)
      my_sem_wait(SEM_ID);
    slowInc(&global, inc);
    if (use_sem)
      my_sem_post(SEM_ID);
  }

  if (use_sem)
    my_sem_close(SEM_ID);

    draw_word(0xFFFFFF ,"termino2\n");
  return 0;
}

uint64_t test_sync() { //{n, use_sem, 0}
  uint64_t pids[2 * TOTAL_PAIR_PROCESSES];

  // if (argc != 2)
  //   return -1;

  // char *argvDec[] = {100, "-1", 0, NULL};
  // char *argvInc[] = {100, "1", 0, NULL};

  global = 0;

  uint64_t i;
  int64_t fd[3] = {0, 1, 2};

  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
    pids[i] = new_process((void(*))my_process_inc1, 1, NULL, 0,fd,0);
    pids[i + TOTAL_PAIR_PROCESSES] = new_process((void(*))my_process_inc2, 1, NULL, 0,fd,0);
  }

  for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
    int ret;
    int64_t sem = wait(pids[i], &ret);
    wait(pids[i + TOTAL_PAIR_PROCESSES], &ret);
  }

  char str[20];
  itoa(global, str);
  for(int i=0; i<1000000;i++){
    i+1;
    i-2;
    int x=i;
  }

  draw_word(0xFFFFFF ,"Final value:");
  draw_word(0xFFFFFF ,str);

  return 0;
}