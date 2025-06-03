#include "syscall.h"
#include <sys_calls.h>
#include "test_util.h"
#include <stdio.h>
// #include <stdlib.h>
#include <string.h>
#include <shell.h>

#define MAX_BLOCKS 128

typedef struct MM_rq {
  void *address;
  uint32_t size;
} mm_rq;

uint64_t test_mm(uint64_t argc, char *argv) {

  mm_rq mm_rqs[MAX_BLOCKS];
  uint8_t rq;
  uint32_t total;
  uint64_t max_memory;

  // print("entre", MAXBUFF);
  // if (argc != 1)
  //   return -1;

  // print("entre", MAXBUFF);

  // if ((max_memory = satoi(argv)) <= 0)
  //   return -1;

  max_memory = 1000000;

  print("test_mm pase \n", MAXBUFF);
  /*while (1) {*/
    rq = 0;
    total = 0;
    int count = 0;
    // Request as many blocks as we can
    while (rq < MAX_BLOCKS && total < max_memory) {
      count++;
      mm_rqs[rq].size = GetUniform(max_memory - total - 1) + 1;
      char buf[20];
      itoa(mm_rqs[rq].size, buf);
      print(buf, MAXBUFF);
      print("\n", MAXBUFF);
      mm_rqs[rq].address = my_malloc(mm_rqs[rq].size);

      if (mm_rqs[rq].address) {
        total += mm_rqs[rq].size;
        rq++;
      }
    }

    // Set
    uint32_t i;
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address)
        memset(mm_rqs[i].address, i, mm_rqs[i].size);

    // Check
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address)
        if (!memcheck(mm_rqs[i].address, i, mm_rqs[i].size)) {
          print("test_mm ERROR\n", MAXBUFF);
          return -1;
        }

    // Free
    for (i = 0; i < rq; i++)
      if (mm_rqs[i].address)
        my_free((uint64_t)mm_rqs[i].address);
  ////}
  char itoa_buffer[20];
  itoa(count, itoa_buffer);
  print(itoa_buffer, MAXBUFF);
  print("todo OK!!!\n", MAXBUFF);
}