
#include <stdio.h>
#include <stdint.h>
#include "sys_calls.h"

// extern uint64_t syscall_write();
extern uint64_t syscall_time();
extern uint64_t syscall_drawPixel();
extern uint64_t syscall_drawSquare();
extern uint64_t syscall_getWidth_vd();
extern uint64_t syscall_getHeight_vd();
extern uint64_t syscall_sleep();
extern uint64_t syscall_paintAll_vd();
extern uint64_t syscall_erraseChar();
extern uint64_t syscall_increaseFS();
extern uint64_t syscall_decreaseFS();
extern uint64_t syscall_setPixelSize();
extern uint64_t syscall_getCurrentPixelSize();
extern uint64_t syscall_erraseLine();
extern void syscall_beep();
extern uint64_t syscall_regs();


char getCharUser();
void erraseChar(uint32_t hexColor);



void print(const char* buf, uint64_t count) {
	syscall_write(1, buf, count);
}

void err_print(char* buff, int count){
    syscall_write(2, buff, count);
}

int strlen(char * str){
    int i=0;
    while(str[i]!=0){
        i++;
    }
    return i;
}

void getString(char* buff, int count){
    int i=0;
    char c=0;
    while(i < (count -1) && (c!='\n')){
         c = getCharUser();
        if(c=='\b'){
            if(i!=0){
             i--;
             erraseChar(0x000000);
            }
            buff[i]=0;
        }
        if(c!=0 && c!='=' && c!='\b'){   
        char auxPrint[2];
        auxPrint[0]=c;
        auxPrint[1]='\0';
        print(auxPrint, 1);
        
        if(c!=0 && c!='\n'){
            buff[i++]=c;
        }
    }
    buff[i]='\0';
}
}

int strcmp(char * s1, char * s2){
   int i = 0;
    while (s1[i] != 0 && s2[i] != 0){
        if (s1[i] - s2[i] != 0){
            return s1[i] - s2[i];
        }
        i++;
    }
    if (s1[i] != 0 || s2[i] != 0){
        return s1[i] - s2[i];
    }
    return 0;
}



uint64_t itoa(uint64_t number, char* s) {
    int digits = 0;

    // Contar los dígitos
    for (uint64_t n = number; n != 0; digits++, n /= 10);

    // Manejo del caso donde number es 0
    if (digits == 0) {
        s[0] = '0';
        s[1] = '\0';
        return 1;
    }

    // Terminar la cadena con un carácter nulo
    s[digits] = '\0';

    // Convertir los dígitos en orden inverso
    for (int i = digits - 1; i >= 0; i--) {
        s[i] = (number % 10) + '0';
        number /= 10;
    }

    return digits;
}

void getTime(char* buffer) {
    uint64_t time = syscall_time();
    int hours =  (time & 0xFF )-3; 
    if(hours<0){
        hours+=24;
    }
    
    // Convertir horas
    int digits = itoa(hours, buffer);
    buffer[digits] = ':'; // Separador de horas
    buffer += (digits + 1);

    // Convertir minutos
    digits = itoa((time >> 8) & 0xFF, buffer);
    buffer[digits] = ':'; // Separador de minutos
    buffer += (digits + 1);

    // Convertir segundos
    itoa((time >> 16) & 0xFF, buffer);
}

uint64_t get_seconds(){
    uint64_t time = syscall_time();
    return (time >> 16) & 0xFF;
}

void printTime() {
    char buffer[10]; // El buffer debe ser lo suficientemente grande
    getTime(buffer);
    print(buffer, 9);
}

char getCharUser(){
	char c;
	syscall_read(0,&c);
	return c;
}

uint64_t getWidth_vd(){
    int width = syscall_getWidth_vd();
    return width;
}

uint64_t getHeight_vd(){
    int height = syscall_getHeight_vd();
    return height;
}

void drawPixel(uint32_t color, uint64_t x, uint64_t y){
    syscall_drawPixel(color, x, y);
}

void drawSquare(uint32_t color, uint64_t x, uint64_t y, uint64_t thickness){
    syscall_drawSquare(color, x, y, thickness);
}

void nano_sleep(uint64_t secs){
    syscall_sleep(secs);
}

void paintAll_vd(uint32_t hexColor){
    syscall_paintAll_vd(hexColor);
}

void erraseChar(uint32_t hexColor){
    syscall_erraseChar(hexColor);
}

void increaseFontSize(){
    syscall_increaseFS();
}

void decreaseFontSize(){
    syscall_decreaseFS();
}

void setPixelSize(){
    syscall_setPixelSize();
}

uint8_t getCurrentPixelSize(){
    return syscall_getCurrentPixelSize();
}

void erraseLine(){
    syscall_erraseLine();

}
void makeBeep(int secs, int freq){
    syscall_beep(secs, freq);
}

uint64_t register_snapshot(uint64_t * regs){
    return syscall_regs(regs);
}

uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base){
	char *p = buffer;
	char *p1, *p2;
	uint32_t digits = 0;

	//Calculate characters for each digit
	do
	{
		uint32_t remainder = value % base;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		digits++;
	}
	while (value /= base);

	// Terminate string in buffer.
	*p = 0;

	//Reverse string in buffer.
	p1 = buffer;
	p2 = p - 1;
	while (p1 < p2)
	{
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}

	return digits;
}