// #include <test_processes.h>

// enum State { RUNNINGTEST,
//              BLOCKEDTEST,
//              KILLEDTEST };

// typedef struct P_rq {
//   int32_t pid;
//   enum State state;
// } p_rq;

// void endless_loop_print() {
//   draw_word(0xFFFFFF, "Endless loop print\n");
//   int64_t pid = get_pid();
//   char str[4];
//     while (1) {
//       itoa(pid, str);
//       draw_word(0xFFFFFF ,str);
//       bussy_wait(10000000);
//     }
// }

// int64_t test_processes(uint64_t argc, char *argv[]) {
//   uint8_t rq;
//   uint8_t alive = 0;
//   uint8_t action;
//   uint64_t max_processes = 30;
//   char *argvAux[] = {0};

//   // if (argc != 1)
//   //   return -1;

//   // if ((max_processes = satoi(argv[0])) <= 0){
//   //   return -1;
//   // }
    

//   p_rq p_rqs[max_processes];

//   draw_word(0xFFFFF, "test_processes: Creating process %d\n");

//   while (1) {

//     // Create max_processes processesS
//     for (rq = 0; rq < max_processes; rq++) {
//       p_rqs[rq].pid = new_process((void(*))endless_loop_print, 1, NULL, 0);

//       if (p_rqs[rq].pid == -1) {
//         draw_word(0xFFFFF,"test_processes: ERROR creating process\n");
//         return -1;
//       } else {
//         p_rqs[rq].state = RUNNINGTEST;
//         alive++;
//       }
//     }

//     // Randomly kills, blocks or unblocks processes until every one has been killed
//     while (alive > 0) {

//       for (rq = 0; rq < max_processes; rq++) {
//         action = GetUniform(100) % 2;

//         switch (action) {
//           case 0:
//             if (p_rqs[rq].state == RUNNINGTEST || p_rqs[rq].state == BLOCKEDTEST) {
//               if (kill_process(p_rqs[rq].pid) == -1) {
//                 draw_word(0xFFFFFF ,"test_processes: ERROR killing process\n");
//                 return -1;
//               }
//               p_rqs[rq].state = KILLEDTEST;
//               alive--;
//             }
//             break;

//           case 1:
//             if (p_rqs[rq].state == RUNNINGTEST) {
//               if (block(get_pcb(p_rqs[rq].pid)) == -1) {
//                 draw_word(0xFFFFFF ,"test_processes: ERROR blocking process\n");
//                 return -1;
//               }
//               p_rqs[rq].state = BLOCKEDTEST;
//             }
//             break;
//         }
//       }

//       // Randomly unblocks processes
//       for (rq = 0; rq < max_processes; rq++)
//         if (p_rqs[rq].state == BLOCKEDTEST && GetUniform(100) % 2) {
//           if (ready(get_pcb(p_rqs[rq].pid)) == -1) {
//             draw_word(0xFFFFFF ,"test_processes: ERROR unblocking process\n");
//             return -1;
//           }
//           p_rqs[rq].state = RUNNINGTEST;
//         }
//     }
//   }
// }