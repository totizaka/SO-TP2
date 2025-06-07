#include <test_processes.h>

enum State { RUNNING1,
             BLOCKED1,
             KILLED1 };

typedef struct P_rq {
  int32_t pid;
  enum State state;
} p_rq;

int64_t test_processes(char *argv[], uint64_t argc) {
  uint8_t rq;
  uint8_t alive = 0;
  uint8_t action;
  uint64_t max_processes;
  char *argvAux[] = {0};

   if (argc != 1)
     return -1;

   if ((max_processes = satoi(argv[0])) <= 0)
     return -1;

  p_rq p_rqs[max_processes];
    fd_t fds[FD_MAX]={STDIN,STDOUT,STDERR};


  while (1) {

    // Create max_processes processes
    for (rq = 0; rq < max_processes; rq++) {
      p_rqs[rq].pid = my_create_process((void(*))endless_loop_print, NULL, 0, 0,fds);

      if (p_rqs[rq].pid == -1) {
        print("test_processes: ERROR creating process\n", MAXBUFF);
        return -1;
      } else {
        p_rqs[rq].state = RUNNING1;
        alive++;
      }
    }

    // Randomly kills, blocks or unblocks processes until every one has been killed
    while (alive > 0) {

      for (rq = 0; rq < max_processes; rq++) {
        action = GetUniform(100) % 2;

        switch (action) {
          case 0:
            if (p_rqs[rq].state == RUNNING1 || p_rqs[rq].state == BLOCKED1) {
              if (my_kill(p_rqs[rq].pid) == -1) {
                print("test_processes: ERROR killing process\n", MAXBUFF);
                return -1;
              }
              p_rqs[rq].state = KILLED1;
              alive--;
            }
            break;

          case 1:
            if (p_rqs[rq].state == RUNNING1) {
              if (my_block(p_rqs[rq].pid) == -1) {
                print("test_processes: ERROR blocking process\n", MAXBUFF);
                return -1;
              }
              p_rqs[rq].state = BLOCKED1;
            }
            break;
        }
      }

      // Randomly unblocks processes
      for (rq = 0; rq < max_processes; rq++)
        if (p_rqs[rq].state == BLOCKED1 && GetUniform(100) % 2) {
          if (my_unblock(p_rqs[rq].pid) == -1) {
            print("test_processes: ERROR unblocking process\n", MAXBUFF);
            return -1;
          }
          p_rqs[rq].state = RUNNING1;
        }
    }
  }
}