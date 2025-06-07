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


comands_pipe get_comands_pipe(char* input){
   comands_pipe comands;
    int i=0;
    int c1=0;
    int c2=0;
    comands.pipe=0;

    while (input[i]!='\0'){
        if(input[i]=='|' && comands.pipe==0){
            comands.pipe=1;
        }
        else if (!comands.pipe)
        {
            comands.cm1[c1++]=input[i];
        }
        else {
            comands.cm2[c2++]=input[i];
        }
        i++;
    }
        comands.cm1[c1]='\0';
   
        comands.cm2[c2]='\0';
        return comands;
    
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

int64_t my_create_process(uint64_t rip, char ** argv, uint64_t argc, int8_t background, fd_t fds[FD_MAX]){
    return syscall_my_create_process(rip, argv, argc, background, fds);
}

int64_t my_create_process_shell(uint64_t rip, char ** argv, uint64_t argc, int8_t background, fd_t fds[FD_MAX]){
    uint64_t pid = my_create_process(rip, argv, argc, background, fds);
    if (!background){
        int64_t ret;
        my_wait(pid, &ret);
    }
    return pid;
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

int64_t my_wait(uint64_t pid, int64_t *ret){
    return syscall_my_wait(pid, ret);
}

int64_t my_unblock(uint64_t pid){
    return syscall_my_unblock(pid);
}
int64_t my_yield(){
    return  syscall_my_yield();
}

int64_t exit_proc(uint64_t res, uint64_t pid){//para q usaria la res en kernel??
    //buscar la forma de volver al proceso en el que estaba antes despues con el scheduler (al padre??)
    return my_kill(pid);
}

int utoa_hex(uint64_t value, char *str) {
    const char *hex = "0123456789ABCDEF";
    char tmp[17]; // 16 dígitos + \0
    int i = 0;

    if (value == 0) {
        str[0] = '0';
        return 1;
    }

    while (value > 0) {
        tmp[i++] = hex[value % 16];
        value /= 16;
    }

    // invertimos
    for (int j = 0; j < i; j++) {
        str[j] = tmp[i - j - 1];
    }

    return i; // cantidad de caracteres escritos
}

void my_ps(){
    print("Iniciando my_ps\n", MAXBUFF);

    process_info_list *plist = syscall_my_get_processes();
    if (plist == NULL) {
        print("Error: no se pudo obtener la lista de procesos\n", MAXBUFF);
        return;
    }

    char tmp[64];
    int len = itoa(plist->amount_of_processes, tmp);
    tmp[len] = '\0';

    print("Procesos encontrados: ", MAXBUFF);
    print(tmp, len);
    print("\n", 1);

    print("  PID   PRIORITY   STATE     NAME           STACK_BASE   STACK_PTR   BACKGROUND\n", MAXBUFF);
    print("-------------------------------------------------------------------------------\n", MAXBUFF);

    char buffer[128];  // ampliamos para más información
    char numbuf[21];

    for (uint64_t i = 0; i < plist->amount_of_processes; i++) {
        process_info *p = &plist->processes[i];

        int pos = 0;

        // PID
        len = itoa(p->pid, numbuf);
        for (int j = 0; j < 4 - len; j++) buffer[pos++] = ' ';
        for (int j = 0; j < len; j++) buffer[pos++] = numbuf[j];
        buffer[pos++] = ' ';

        // PRIORITY
        len = itoa(p->priority, numbuf);
        for (int j = 0; j < 7 - len; j++) buffer[pos++] = ' ';
        for (int j = 0; j < len; j++) buffer[pos++] = numbuf[j];
        buffer[pos++] = ' ';

        for (int j = 0; j < 6; j++) buffer[pos++] = ' '; // espacio para el estado

        // STATE
        const char *state_str;
        switch (p->status) {
            case READY: state_str = "READY\0"; break;
            case BLOCKED: state_str = "BLOCKE\0"; break;
            case FREE: state_str = "FREE\0"; break;
            case RUNNING: state_str = "RUNNING\0"; break;
            case ZOMBIE: state_str = "ZOMBIE\0"; break;
            default: state_str = "UNKNOWN\0"; break;
        }

        int st_len= my_strcpy(buffer+pos, state_str);
        pos+=st_len;
        for (int j = st_len; j < 9; j++) buffer[pos++] = ' ';
        buffer[pos++] = ' ';

        // NAME
        const char *name = p->name ? p->name : "NoName";
        int n = 0;
        while (name[n] != '\0' && pos < (int)sizeof(buffer) - 1) {
            buffer[pos++] = name[n++];
        }
        for (int j = n; j < 14; j++) buffer[pos++] = ' ';

        // STACK_BASE (hex)
        char * prefix=" 0x\0";
        pos+=my_strcpy(buffer+pos, prefix);
        pos += utoa_hex(p->stack_base, buffer + pos);

        for (int i = 0; i < 4; i++){
            buffer[pos++] = ' ';
        }

        // STACK_POINTER (hex)
        pos+=my_strcpy(buffer+pos, prefix);
        pos += utoa_hex(p->stack_pointer, buffer + pos);

        for (int i = 0; i < 5; i++){
            buffer[pos++] = ' ';
        }

        // BACKGROUND 
        pos+=my_strcpy(buffer+pos, p->background ? "YES\0":"NO\0");

        print(buffer, pos);
        print("\n", 1);
        print("-------------------------------------------------------------------------------\n", MAXBUFF);
    }

    my_free_ps(plist);
    return;
}

void my_free_ps(process_info_list *plist) {
    syscall_my_free_processes(plist);
}

void my_mem_state() {
    memory_state* state = syscall_my_mem_state();

    if (state == NULL) {
        print("Error al obtener el estado de la memoria\n", MAXBUFF);
        return;
    }
    char buffer[128];
    int len = 0;
    len += my_strcpy(buffer + len, "Estado de la memoria:\n");
    len += my_strcpy(buffer + len, "Total Size: ");
    len += itoa(state->total_size, buffer + len);
    len += my_strcpy(buffer + len, " bytes\n");
    len += my_strcpy(buffer + len, "Free Memory: ");
    len += itoa(state->free, buffer + len);
    len += my_strcpy(buffer + len, " bytes\n");
    len += my_strcpy(buffer + len, "Occupied Memory: ");
    len += itoa(state->occupied, buffer + len);
    len += my_strcpy(buffer + len, " bytes\n");
    buffer[len] = '\0';
    print(buffer, len);
    print("\n", 1);

    my_free_mem_state(state);
    return;
}

void my_free_mem_state(memory_state *state) {
    syscall_my_free_mem_state(state);
}

int my_strcpy(char *dest, const char *src) {
    char *original = dest;
    int i=0;
    while ((*original++ = *src++) != '\0') i++;

    return i;
}

int64_t sem_open ( int64_t sem_id, int value){
    return syscall_my_sem_open(sem_id, value);
}
int64_t sem_open_get_id(int value){
    return syscall_my_sem_open_get_id(value);
}
int64_t sem_post ( int64_t sem_id ){
    return syscall_my_sem_post(sem_id);
}
int64_t sem_wait ( int64_t sem_id){
    return syscall_my_sem_wait(sem_id);
}
int64_t sem_close ( int64_t sem_id){
    return syscall_my_sem_close(sem_id);
}

void my_create_pipe(int64_t id){
    return syscall_my_create_pipe(id);
}

int8_t my_open_pipe(int64_t target, int role){
    return syscall_my_open_pipe( target,  role);
}

int8_t my_close_pipe(int64_t target){
    return syscall_my_close_pipe(target);
}

int64_t my_get_available_pipe_id(){
    return syscall_my_get_available_pipe_id();
}

int64_t my_write_pipe(int64_t target, int * buffer,int num_bytes){
    return syscall_my_write_pipe(target, buffer, num_bytes);
}

int64_t my_read_pipe(int64_t target, int * buffer,  int num_bytes){
    return syscall_my_read_pipe(target, buffer, num_bytes);
}

void my_strcat(char *dest, const char *src) {
    while (*dest) {
        dest++; // Move to the end of the destination string
    }
    while (*src) {
        *dest++ = *src++; // Copy the source string to the destination
    }
    *dest = '\0'; // Null-terminate the resulting string
}