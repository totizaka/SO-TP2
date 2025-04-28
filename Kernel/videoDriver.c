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

const uint16_t charWidht = 9;
const uint16_t charHeight = 16;
uint8_t pixelSize = 1;

void increasePixelSize() {
    if (pixelSize < 5) {
        pixelSize++;
    }
}

void decreasePixelSize() {
    if (pixelSize > 1) {
        pixelSize--;
    }
}

void setPixelSize(uint8_t size){
	if(size >=1 && size <= 5){
		pixelSize = size;
	}
}

uint8_t getCurrentPixelSize(){
	return pixelSize;
}

uint16_t getCharWidth() {
    return charWidht * pixelSize;
}

uint16_t getCharHeight() {
    return charHeight * pixelSize;
}


void putPixel(uint32_t hexColor, uint64_t x, uint64_t y) {
    uint8_t * framebuffer = (uint8_t *) (uint64_t)VBE_mode_info->framebuffer;
    uint64_t offset = (x * ((VBE_mode_info->bpp)/8)) + (y * VBE_mode_info->pitch);
    framebuffer[offset]     =  (hexColor) & 0xFF;
    framebuffer[offset+1]   =  (hexColor >> 8) & 0xFF; 
    framebuffer[offset+2]   =  (hexColor >> 16) & 0xFF;
}

void drawChar(uint32_t hexColor, char c, uint64_t x, uint64_t y){
	if(c == 10){
		newLine_vd();
		global_x = 0;
		return;
	}
    if(c<32 && c>126){
        return;
    }
	
    int pos=((int)c-32)*16;

	if (global_x + getCharWidth() >= VBE_mode_info->width) {
        global_x = 0;
        if (global_y + getCharHeight() > VBE_mode_info->height) {
            paintAll_vd(BLACK);
        } else {
            global_y += getCharHeight();
        }
    }

    for (int i = 0; i < 16; i++){
        for (int j = 0; j < 8; j++){
			for (int t = 0; t < pixelSize; t++){
                for (int h = 0; h < pixelSize; h++){
					if (font_bitmap[pos+i] >> (7-j) & 0x1)
					{
						putPixel(hexColor, x+(j*pixelSize)+t, y+(i*pixelSize)+h);
					}
				}
			}
        }
    }
}

void drawWord(uint32_t hexColor, char* word){
	for (int i = 0; word[i]!=0; i++)
	{	
		if (global_x >= VBE_mode_info->width) {
        	global_x = 0;
        	if (global_y + getCharHeight() > VBE_mode_info->height) {
            	paintAll_vd(BLACK);
        	} else {
            	global_y += getCharHeight();
        	}
    	}
		drawChar(hexColor, word[i], (global_x += getCharWidth()), global_y);
	}
}

void erraseChar(uint32_t hexColor){
	
	for (int i = 0; i < 16; i++){
        for (int j = 0; j < 8; j++){
			for (int t = 0; t < pixelSize; t++){
                for (int h = 0; h < pixelSize; h++){
					putPixel(hexColor, global_x+(j*pixelSize)+t, global_y+(i*pixelSize)+h);
				}
        	}
    	}
	}
	if(global_x!=0){
		global_x -= getCharWidth();
	}
	
}

void newLine_vd(){
	if (global_y + getCharHeight() > VBE_mode_info->height) {
        paintAll_vd(BLACK);
    }else {
        global_y += getCharHeight();
    }
	global_x= 0;
}



void putSquarePixel(uint32_t hexColor, uint64_t x, uint64_t y, uint64_t thickness){
	for (uint64_t i = 0; i < thickness; i++){
		for (uint64_t j = 0; j < thickness; j++){
			putPixel(hexColor, x+i, y+j);
		}
	}
}

uint16_t getWidth_vd(){
	return VBE_mode_info->width;			
}

uint16_t getHeight_vd(){
	return VBE_mode_info->height;			
}

void paintAll_vd(uint32_t hexColor){
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
void erraseLine(){
    if (global_y > 0)
    {
        global_x = 0;
        for (int i = 0; i < 16; i++){
            for (int j = 0; j < getWidth_vd(); j++){
                for (int t = 0; t < pixelSize; t++){
                    for (int h = 0; h < pixelSize; h++){
                    putPixel(BLACK, global_x+(j*pixelSize)+t, global_y+(i*pixelSize)+h);
                    }
                }
            }
        }
    }
	if(global_y > 0){
    	global_y -= getCharHeight();
	}
}