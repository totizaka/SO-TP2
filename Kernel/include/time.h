#ifndef _TIME_H_
#define _TIME_H_

#include <stdint.h>

void timer_handler();
int ticks_elapsed();
int seconds_elapsed();
uint64_t getCurrentTimeBinary();
void sleep(uint64_t secs);
extern uint8_t getHs(void);
extern uint8_t getMins(void);
extern uint8_t getSecs(void);

#endif