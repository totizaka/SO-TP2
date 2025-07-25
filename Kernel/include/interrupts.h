 /*
 *   interrupts.h
 *
 *  Created on: Apr 18, 2010
 *      Author: anizzomc
 */

#ifndef INTERRUPS_H_
#define INTERRUPS_H_
#include <stdint.h>

#include <idtLoader.h>

void _irq00Handler(void);
void _irq01Handler(void);
void _irq02Handler(void);
void _irq03Handler(void);
void _irq04Handler(void);
void _irq05Handler(void);

void _int80Handler(void);


void _exception0Handler(void);
void _exception06Handler(void);

void _intPureba(void);
void _cli(void);

void _sti(void);

void _hlt(void);

void pic_master_mask(uint8_t mask);

void pic_slave_mask(uint8_t mask);

//Termina la ejecución de la cpu.
void haltcpu(void);

void timer_tick(void);

#endif /* INTERRUPS_H_ */