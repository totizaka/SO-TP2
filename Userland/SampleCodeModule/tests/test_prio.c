// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <test_prio.h>
#include <stdlib_user.h>

int64_t prio[TOTAL_PROCESSES] = {LOWEST, MEDIUM, HIGHEST};

void endless_loop_print(uint64_t wait);

void test_prio() {
  int64_t pids[TOTAL_PROCESSES];
  uint64_t i;
  fd_t fds[FD_MAX]={STDIN,STDOUT,STDERR};

  for (i = 0; i < TOTAL_PROCESSES; i++){
    char* argv[] = {"endless_loop_print", NULL};
    pids[i] = my_create_process((void(*))endless_loop_print, argv, 1, 0, fds);
  }

  bussy_wait(WAIT);
  print("\nCHANGING PRIORITIES...\n", MAXBUFF);

  for (i = 0; i < TOTAL_PROCESSES; i++)
    my_nice(pids[i], prio[i]);

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