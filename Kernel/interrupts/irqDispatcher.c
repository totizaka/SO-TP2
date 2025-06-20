// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <time.h>
#include <keyboard.h>
#include <stdint.h>

static void int_20();
static void int_21();

void (*interrupt_arr[])(void) = {int_20, int_21};

void irq_dispatcher(uint64_t irq) {
	interrupt_arr[irq]();
	return;
}

void int_20() {
	timer_handler();
}

void int_21(){
	keyboard_handler();
}
