#include <videoDriver.h>
#include <stdint.h>

#define BLACK 0x000000

int global_x=0;
int global_y=0;

extern const unsigned char font_bitmap[];

struct vbe_mode_info_structure {
	uint16_t attributes;		// deprecated, only bit 7 should be of interest to you, and it indicates the mode supports a linear frame buffer.
	uint8_t window_a;			// deprecated
	uint8_t window_b;			// deprecated
	uint16_t granularity;		// deprecated; used while calculating bank numbers
	uint16_t window_size;
	uint16_t segment_a;
	uint16_t segment_b;
	uint32_t win_func_ptr;		// deprecated; used to switch banks from protected mode without returning to real mode
	uint16_t pitch;			// number of bytes per horizontal line
	uint16_t width;			// width in pixels
	uint16_t height;			// height in pixels
	uint8_t w_char;			// unused...
	uint8_t y_char;			// ...
	uint8_t planes;
	uint8_t bpp;			// bits per pixel in this mode
	uint8_t banks;			// deprecated; total number of banks in this mode
	uint8_t memory_model;
	uint8_t bank_size;		// deprecated; size of a bank, almost always 64 KB but may be 16 KB...
	uint8_t image_pages;
	uint8_t reserved0;
 
	uint8_t red_mask;
	uint8_t red_position;
	uint8_t green_mask;
	uint8_t green_position;
	uint8_t blue_mask;
	uint8_t blue_position;
	uint8_t reserved_mask;
	uint8_t reserved_position;
	uint8_t direct_color_attributes;
 
	uint32_t framebuffer;		// physical address of the linear frame buffer; write here to draw to the screen
	uint32_t off_screen_mem_off;
	uint16_t off_screen_mem_size;	// size of memory in the framebuffer but not being displayed on the screen
	uint8_t reserved1[206];
} __attribute__ ((packed));

typedef struct vbe_mode_info_structure * VBEInfoPtr;

VBEInfoPtr VBE_mode_info = (VBEInfoPtr) 0x0000000000005C00;

const uint16_t char_widht = 9;
const uint16_t char_height = 16;
uint8_t pixel_size = 1;

void increase_pixel_size() {
    if (pixel_size < 5) {
        pixel_size++;
    }
}

void decrease_pixel_size() {
    if (pixel_size > 1) {
        pixel_size--;
    }
}

void set_pixel_size(uint8_t size){
	if(size >=1 && size <= 5){
		pixel_size = size;
	}
}

uint8_t get_current_pixel_size(){
	return pixel_size;
}

uint16_t get_char_width() {
    return char_widht * pixel_size;
}

uint16_t get_char_height() {
    return char_height * pixel_size;
}


void put_pixel(uint32_t hexColor, uint64_t x, uint64_t y) {
    uint8_t * framebuffer = (uint8_t *) (uint64_t)VBE_mode_info->framebuffer;
    uint64_t offset = (x * ((VBE_mode_info->bpp)/8)) + (y * VBE_mode_info->pitch);
    framebuffer[offset]     =  (hexColor) & 0xFF;
    framebuffer[offset+1]   =  (hexColor >> 8) & 0xFF; 
    framebuffer[offset+2]   =  (hexColor >> 16) & 0xFF;
}

void draw_char(uint32_t hexColor, char c, uint64_t x, uint64_t y){
	if(c == 10){
		new_line_vd();
		global_x = 0;
		return;
	}
    if(c<32 && c>126){
        return;
    }
	
    int pos=((int)c-32)*16;

	if (global_x + get_char_width() >= VBE_mode_info->width) {
        global_x = 0;
        if (global_y + get_char_height() > VBE_mode_info->height) {
            paint_all_vd(BLACK);
        } else {
            global_y += get_char_height();
        }
    }

    for (int i = 0; i < 16; i++){
        for (int j = 0; j < 8; j++){
			for (int t = 0; t < pixel_size; t++){
                for (int h = 0; h < pixel_size; h++){
					if (font_bitmap[pos+i] >> (7-j) & 0x1)
					{
						put_pixel(hexColor, x+(j*pixel_size)+t, y+(i*pixel_size)+h);
					}
				}
			}
        }
    }
}

void draw_word(uint32_t hexColor, char* word){
	for (int i = 0; word[i]!=0; i++)
	{	
		if (global_x >= VBE_mode_info->width) {
        	global_x = 0;
        	if (global_y + get_char_height() > VBE_mode_info->height) {
            	paint_all_vd(BLACK);
        	} else {
            	global_y += get_char_height();
        	}
    	}
		draw_char(hexColor, word[i], (global_x += get_char_width()), global_y);
	}
}

void errase_char(uint32_t hexColor){
	
	for (int i = 0; i < 16; i++){
        for (int j = 0; j < 8; j++){
			for (int t = 0; t < pixel_size; t++){
                for (int h = 0; h < pixel_size; h++){
					put_pixel(hexColor, global_x+(j*pixel_size)+t, global_y+(i*pixel_size)+h);
				}
        	}
    	}
	}
	if(global_x!=0){
		global_x -= get_char_width();
	}
	
}

void new_line_vd(){
	if (global_y + get_char_height() > VBE_mode_info->height) {
        paint_all_vd(BLACK);
    }else {
        global_y += get_char_height();
    }
	global_x= 0;
}



void put_square_pixel(uint32_t hexColor, uint64_t x, uint64_t y, uint64_t thickness){
	for (uint64_t i = 0; i < thickness; i++){
		for (uint64_t j = 0; j < thickness; j++){
			put_pixel(hexColor, x+i, y+j);
		}
	}
}

uint16_t get_width_vd(){
	return VBE_mode_info->width;			
}

uint16_t get_height_vd(){
	return VBE_mode_info->height;			
}

void paint_all_vd(uint32_t hexColor){
    uint8_t * framebuffer = (uint8_t *)(uint64_t) VBE_mode_info->framebuffer;
    uint32_t color = hexColor;
    uint64_t totalPixels = VBE_mode_info->width * VBE_mode_info->height;
    uint8_t red = (color >> 16) & 0xFF;
    uint8_t green = (color >> 8) & 0xFF;
    uint8_t blue = color & 0xFF;

    for (uint64_t i = 0; i < totalPixels; i++) {
        framebuffer[i * 3]     = blue;  // Componente azul
        framebuffer[i * 3 + 1] = green; // Componente verde
        framebuffer[i * 3 + 2] = red;   // Componente roja
    }
	global_y=0;
	global_x=0;
}

void errase_line(){
    if (global_y > 0)
    {
        global_x = 0;
        for (int i = 0; i < 16; i++){
            for (int j = 0; j < get_width_vd(); j++){
                for (int t = 0; t < pixel_size; t++){
                    for (int h = 0; h < pixel_size; h++){
                    put_pixel(BLACK, global_x+(j*pixel_size)+t, global_y+(i*pixel_size)+h);
                    }
                }
            }
        }
    }
	if(global_y > 0){
    	global_y -= get_char_height();
	}
}