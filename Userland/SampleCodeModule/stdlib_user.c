#include <stdio.h>
#include <stdint.h>
#include <stdlib_user.h>

void print(const char* buf, uint64_t count) {
	syscall_write(1, buf, count);
}

void err_print(char* buff, int count){
    syscall_write(2, buff, count);
}

int strlen_user(char * str){
    int i=0;
    while(str[i]!=0){
        i++;
    }
    return i;
}

void get_string(char* buff, int count){
    int i=0;
    char c=0;
    while(i < (count -1) && (c!='\n')){
         c = get_char_user();
        if(c=='\b'){
            if(i!=0){
             i--;
             errase_char(0x000000);
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

int strcmp_user(char * s1, char * s2){
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

void get_time(char* buffer) {
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

void print_time() {
    char buffer[10]; // El buffer debe ser lo suficientemente grande
    get_time(buffer);
    print(buffer, 9);
}

char get_char_user(){
	char c;
	syscall_read(0,&c);
	return c;
}

uint64_t get_width_vd(){
    int width = syscall_get_width_vd();
    return width;
}

uint64_t get_height_vd(){
    int height = syscall_get_height_vd();
    return height;
}

void draw_pixel(uint32_t color, uint64_t x, uint64_t y){
    syscall_draw_pixel(color, x, y);
}

void draw_square(uint32_t color, uint64_t x, uint64_t y, uint64_t thickness){
    syscall_draw_square(color, x, y, thickness);
}

void nano_sleep(uint64_t secs){
    syscall_sleep(secs);
}

void paint_all_vd(uint32_t hexColor){
    syscall_paint_all_vd(hexColor);
}

void errase_char(uint32_t hexColor){
    syscall_errase_char(hexColor);
}

void increaseFontSize(){
    syscall_increase_FS();
}

void decreaseFontSize(){
    syscall_decrease_FS();
}

void set_pixel_size(){
    syscall_set_pixel_size();
}

uint8_t get_current_pixel_size(){
    return syscall_get_current_pixel_size();
}

void errase_line(){
    syscall_errase_line();

}
void make_beep(int secs, int freq){
    syscall_beep(secs, freq);
}

uint64_t register_snapshot(uint64_t * regs){
    return syscall_regs(regs);
}

uint32_t uint_to_base(uint64_t value, char * buffer, uint32_t base){
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

void* my_malloc(uint64_t size){
    return (void*)syscall_my_malloc(size);
}

void my_free(uint64_t ptr){
    syscall_my_free(ptr);
}

int64_t my_getpid(){
    return syscall_my_getpid();
}

int64_t my_create_process(uint64_t rip, uint8_t priority, char ** argv, uint64_t argc){
    return syscall_my_create_process(rip, priority, argv, argc);
}

int64_t my_nice(uint64_t pid, uint64_t new_prio){
    return syscall_my_nice(pid, new_prio);
}

int64_t my_kill(uint64_t pid){
    return syscall_my_kill(pid);
}

int64_t my_block(uint64_t pid){
    return syscall_my_block(pid);
}

int64_t my_unblock(uint64_t pid){
    return syscall_my_unblock(pid);
}

int64_t exit(uint64_t res){//para q usaria la res en kernel??
    //buscar la forma de volver al proceso en el que estaba antes despues con el scheduler (al padre??)
    return my_kill(my_getpid());
}