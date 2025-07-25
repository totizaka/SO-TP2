// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com



//Codigo recuperado de  https://wiki.osdev.org/PC_Speaker

#include <speaker.h>
#include "time.h"
#include <stdint.h>
#include <lib.h>


 //Play sound using built-in speaker
 static void play_sound(uint32_t nFrequence) {
 	uint32_t Div;
 	uint8_t tmp;
 
        //Set the PIT to the desired frequency
 	Div = 1193180 / nFrequence;
 	outb(0x43, 0xb6);
 	outb(0x42, (uint8_t) (Div) );
 	outb(0x42, (uint8_t) (Div >> 8));
 
        //And play the sound using the PC speaker
 	tmp = inb(0x61);
  	if (tmp != (tmp | 3)) {
 		outb(0x61, tmp | 3);
 	}
 }
 
 //make it shut up
 static void no_sound() {
 	uint8_t tmp = inb(0x61) & 0xFC;
     
 	outb(0x61, tmp);
 }
 
 //Make a beep
 void beep(int secs, int freq) {
 	 play_sound(freq);
 	 sleep(secs);
 	 no_sound();
          //set_PIT_2(old_frequency);
 }