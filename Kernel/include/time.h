#ifndef _TIME_H_
#define _TIME_H_

#include <stdint.h>

void timer_handler();
int ticks_elapsed();
int seconds_elapsed();
uint64_t get_current_time_binary();
void sleep(uint64_t secs);
extern uint8_t get_hs(void);
extern uint8_t get_mins(void);
extern uint8_t get_secs(void);

#endif