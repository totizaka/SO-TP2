#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

void keyboard_handler();
void printCharPressed();
char get_char_pressed();
int get_regs();
int read_stdin(char * buff, int count);
int buf_has_next();
#endif