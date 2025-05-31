#ifndef TEST_PRIO_H
#define TEST_PRIO_H

#include <stdint.h>
#include <process.h>
#include <videoDriver.h>
#include <scheduler.h>
#include <test_util.h>

#define MINOR_WAIT 1000000 // TODO: Change this value to prevent a process from flooding the screen
#define WAIT 1000      // TODO: Change this value to make the wait long enough to see theese processes beeing run at least twice

#define TOTAL_PROCESSES 3
#define LOWEST 1  // TODO: Change as required
#define MEDIUM 4  // TODO: Change as required
#define HIGHEST 7 // TODO: Change as required


void test_prio() ;


#endif