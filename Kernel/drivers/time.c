#include <time.h>
#include <naiveConsole.h>
#include <interrupts.h>


static unsigned long ticks = 0;

void timer_handler() {
	ticks++;
}

int ticks_elapsed() {
	return ticks;
}

int seconds_elapsed() {
	return ticks / 18;
}

uint64_t get_current_time_binary() {

	uint8_t bcd_hs = get_hs();      // Obtener horas en BCD
    uint8_t bcd_mins = get_mins();  // Obtener minutos en BCD
    uint8_t bcd_secs = get_secs();  // Obtener segundos en BCD
    
	uint8_t bin_hs = ((bcd_hs >> 4) * 10) + (bcd_hs & 0xF);	//Pasar horas a binario
	uint8_t bin_mins = ((bcd_mins >> 4) * 10) + (bcd_mins & 0xF);	//Pasar mins a binario
	uint8_t bin_secs = ((bcd_secs >> 4) * 10) + (bcd_secs & 0xF);	//Pasar segs a binario

    return bin_hs | ((uint64_t)bin_mins << 8) | ((uint64_t)bin_secs << 16);
}

void sleep(uint64_t secs){
	uint64_t start = ticks;
	while(ticks_elapsed()-start<secs){
		_sti();
		_hlt();
	}
	return;
}