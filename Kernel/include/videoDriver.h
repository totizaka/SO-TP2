#ifndef LIB_H
#define LIB_H
#include <stdint.h>
#include <font.h>

void put_pixel(uint32_t hexColor, uint64_t x, uint64_t y);
void draw_char(uint32_t hexColor, char c, uint64_t x, uint64_t y);
void draw_word(uint32_t hexColor, char* word);
void errase_char(uint32_t hexColor);
void put_square_pixel (uint32_t hexColor, uint64_t x, uint64_t y, uint64_t thickness);
uint16_t get_width_vd();
uint16_t get_height_vd();
void new_line_vd();
void paint_all_vd(uint32_t hexColor);
void increase_pixel_size();
void decrease_pixel_size();
void set_pixel_size(uint8_t size);
uint8_t get_current_pixel_size();
void errase_line();
#endif