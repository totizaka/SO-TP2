#ifndef KERNEL_H
#define KERNEL_H
#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <videoDriver.h>
#include <idtLoader.h>
#include <clock.h>
#include <time.h>
#include <mm_dummy.h>  // Para el administrador de memoria
#include <process.h>
#include <scheduler.h>
#include <interrupts.h>
#include <test_prio.h>
#include <test_processes.h>


memory_manager_ADT get_memory_manager();



#endif