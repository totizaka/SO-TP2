#include <test_prio.h>
#include <stdlib_user.h>

int64_t prio[TOTAL_PROCESSES] = {LOWEST, MEDIUM, HIGHEST};


void test_prio() {
  int64_t pids[TOTAL_PROCESSES];
  char *argv[] = {0};
  uint64_t i;

  for (i = 0; i < TOTAL_PROCESSES; i++)
    pids[i] = my_create_process((void(*))endless_loop_print, 1, argv, 1);

  bussy_wait(WAIT);
  print("\nCHANGING PRIORITIES...\n", MAXBUFF);

  for (i = 0; i < TOTAL_PROCESSES; i++)
    my_nice(pids[i], prio[i]*3);

  bussy_wait(WAIT);
  print("\nBLOCKING...\n", MAXBUFF);

  for (i = 0; i < TOTAL_PROCESSES; i++)
    my_block(pids[i]);

  print("CHANGING PRIORITIES WHILE BLOCKED...\n", MAXBUFF);

  for (i = 0; i < TOTAL_PROCESSES; i++)
    my_nice(pids[i], MEDIUM);

  print("UNBLOCKING...\n", MAXBUFF);

  for (i = 0; i < TOTAL_PROCESSES; i++)
    my_unblock(pids[i]);

  bussy_wait(WAIT);
  print("\nKILLING...\n", MAXBUFF);

  for (i = 0; i < TOTAL_PROCESSES; i++)
    my_kill(pids[i]);
}