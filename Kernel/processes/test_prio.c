// #include <test_prio.h>

// int64_t prio[TOTAL_PROCESSES] = {LOWEST, MEDIUM, HIGHEST};


// void endless_loop_print(uint64_t wait) {
//   draw_word(0xFFFFFF, "Endless loop print\n");
//   int64_t pid = get_pid();
//   char str[4];
//     while (1) {
//       itoa(pid, str);
//       draw_word(0xFFFFFF ,str);
//       bussy_wait(wait);
//     }
// }

// void test_prio() {
//   int64_t pids[TOTAL_PROCESSES];
//   char *argv[] = {0};
//   uint64_t i;

//   for (i = 0; i < TOTAL_PROCESSES; i++)
//     pids[i] = new_process((void(*))endless_loop_print, 1, argv, 1);

//   bussy_wait(WAIT);
//   draw_word(0xFFFFFF, "\nCHANGING PRIORITIES...\n");

//   for (i = 0; i < TOTAL_PROCESSES; i++)
//     nice(pids[i], prio[i]*2);

//   bussy_wait(WAIT);
//   draw_word(0xFFFFFF, "\nBLOCKING...\n");

//   for (i = 0; i < TOTAL_PROCESSES; i++)
//     block(pids[i]);

//   draw_word(0xFFFFFF, "CHANGING PRIORITIES WHILE BLOCKED...\n");

//   for (i = 0; i < TOTAL_PROCESSES; i++)
//     nice(pids[i], MEDIUM);

//   draw_word(0xFFFFFF, "UNBLOCKING...\n");

//   for (i = 0; i < TOTAL_PROCESSES; i++)
//     ready(pids[i]);

//   bussy_wait(WAIT);
//   draw_word(0xFFFFFF, "\nKILLING...\n");

//   for (i = 0; i < TOTAL_PROCESSES; i++)
//     kill_process(pids[i]);
// }