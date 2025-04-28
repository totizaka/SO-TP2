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

uint64_t getCurrentTimeBinary() {

	uint8_t bcdHs = getHs();      // Obtener horas en BCD
    uint8_t bcdMins = getMins();  // Obtener minutos en BCD
    uint8_t bcdSecs = getSecs();  // Obtener segundos en BCD
    
	uint8_t binHs = ((bcdHs >> 4) * 10) + (bcdHs & 0xF);	//Pasar horas a binario
	uint8_t binMins = ((bcdMins >> 4) * 10) + (bcdMins & 0xF);	//Pasar mins a binario
	uint8_t binSecs = ((bcdSecs >> 4) * 10) + (bcdSecs & 0xF);	//Pasar segs a binario

    return binHs | ((uint64_t)binMins << 8) | ((uint64_t)binSecs << 16);
}

void sleep(uint64_t secs){
	uint64_t start = ticks;
	while(ticks_elapsed()-start<secs){
		_sti();
		_hlt();
	}
	return;
}