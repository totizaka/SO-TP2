// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <clock.h>


//numeros de flags que te da el link en la P5
#define SECONDS 0x00
#define MINUTES 0x02
#define HOURS 0x04
#define YEAR 0x09
#define MONTH 0x08
#define DAY 0x07 //asumi q es dia del mes, para dia de la semana es otro flag

unsigned int RTC(uint8_t value);
int bcd_decimal(uint8_t hex);


int seconds(){
return bcd_decimal(RTC(SECONDS));
}
int minutes(){
    return bcd_decimal(RTC(MINUTES));
}
int hours(){
    return bcd_decimal(RTC(HOURS));
}
int day(){
    return bcd_decimal(RTC(DAY));
}
int year(){
    return bcd_decimal(RTC(YEAR));
}
int month(){
    return bcd_decimal(RTC(MONTH));
}
//funcion -> https://stackoverflow.com/questions/28133020/how-to-convert-bcd-to-decimal
int bcd_decimal(uint8_t hex){
    return ((hex & 0xF0) >> 4) * 10 + (hex & 0x0F);
}