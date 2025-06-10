#include <shell_apps.h>

int8_t vowel(char c) {
    return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' ||
           c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U';
}

//funciones auxiliares

int read_line_with_backspace(char *buffer, int max_len) {
    int i = 0;
    char c;
    char s[2] = {0};

    while (i < max_len - 1) {
        c = get_char_user();

        if (c == MYEOF) {
            buffer[0] = '\0';
            return 1; // Ctrl+D → EOF
        }

        if (c == '\b') {
            if (i > 0) {
                i--;
                buffer[i] = '\0';
                errase_char(0x000000);
            }
        } else if ((c >= 32 && c < 127) || c == '\n') { // imprimible o enter
            buffer[i++] = c;
            s[0] = c;
            print(s, 1);
        }
    }

    buffer[i] = '\0';
    return 0;
}

char get_char_handled_input() {
    char c;

    while (1) {
        c = get_char_user();

        if (c == '\b') {
            errase_char(0x000000);
            continue; // No devolvemos backspace
        }

        return c;
    }
}

// shell_apps SO commands

void shell_loop(char** argv, uint64_t argc){
   if(argc!=1){
      err_print("ERROR: argc", 30);
      return;
   }
   int segs;
   if ((segs = satoi(argv[0])) <= 0){
      err_print("ERROR: seconds required", 30);
         return;
   }
   int pid;
        char buff[30]="Hola PID: ";
    while(1){
       pid = my_getpid();
       itoa(pid, buff+10);
       print(buff, 30);
       print ("\n",1);
       nano_sleep(segs);
    }
}

void shell_cat() {
    char buffer[MAX_LINE];
    while (1) {
        if (read_line_with_backspace(buffer, MAX_LINE)) {
            break; // Ctrl+D
        }
    }
}

void shell_wc() {
    char c;
    char s[2] = {0};
    int64_t l = 1;

    while ((c = get_char_handled_input()) > 0) {
        if (c == '\n') {
            l++;
        }

        s[0] = c;
        print(s, 1);
    }

    print("\nLines: ", 7);
    char count_buf[20];
    itoa(l, count_buf);
    print(count_buf, 1);
}


void shell_filter() {
    char c;
    char s[2] = {0};

    while ((c = get_char_handled_input()) > 0) {
        if (!vowel(c)) {
            s[0] = c;
            print(s, 1);
        }
    }

    print("\n", 1);
}

void my_ps(){
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

    print("  PID   PRIORITY   STATE     NAME             STACK_BASE   STACK_PTR   BACKGROUND\n", MAXBUFF);
    print("---------------------------------------------------------------------------------\n", MAXBUFF);

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
        for (int j = n; j < 16; j++) buffer[pos++] = ' ';

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


// shell apps Arqui commands

void clear_screen(){
    paint_all_vd(BLACK);
    return;
}

void invalid_comand(){
    paint_all_vd(BLACK);
    err_print("Invalid Command!! \n",18);
}

void opcode_exc(){
    paint_all_vd(BLACK);
    if(get_current_pixel_size() > 2){
        print("FontSize too big\nDecrease fontSize to see al Registers", MAXBUFF);
    }
    else{
        opcode();
    }
}

void div0_exc(){
    paint_all_vd(BLACK);
    if(get_current_pixel_size() > 2){
        print("Fontsize too big\nDecrease fontSize to see al Registers", MAXBUFF);
    }
    else{
        div0();
    }
}

void show_time(){
    paint_all_vd(BLACK);
    print_time();
}

void snake(){
    paint_all_vd(BLACK);

    char buff[MAXBUFF];

    int aux1 = get_current_pixel_size();
    set_pixel_size(3);
    print("WELCOME TO THE SNAKE-GAME\n", MAXBUFF);
    set_pixel_size(2);
    print("Key Control:\nPlayer1: 'w', 'd', 'a', 's'\nPlayer2: 'i', 'l', 'j', 'k'\n", MAXBUFF);
    int correctAmount=0;
    while(!correctAmount){

        print("\nWrite letter \"q\" to exit\n\nIntroduce Players to start game (1 or 2): ", MAXBUFF);

        my_get_string(buff, MAXBUFF);

        if(buff[0] =='q'&& buff[1] == '\0'){
            set_pixel_size(aux1);
            paint_all_vd(BLACK);
            return;
        }
        else if(buff[0] =='1'&& buff[1] == '\0'){
            correctAmount=1;
            uint8_t aux2 = get_current_pixel_size();
            game_loop_1(); //while (gameLoop=='r') y en gameloop pongo que get char hasta q sea r o q(si es q sale ) 
            set_pixel_size(aux2);
        }
        else if (buff[0] =='2' && buff[1] == '\0'){
            correctAmount=1;
            uint8_t aux2 = get_current_pixel_size();
            game_loop_2();
            set_pixel_size(aux2);
        }
        else{
            paint_all_vd(0x000000);
            err_print("Invalid amount!! \n",18); 
        }
    }
    set_pixel_size(aux1);
    paint_all_vd(BLACK);
    return;
}

void font_size(){
    
    paint_all_vd(BLACK);

    while(1){
        char buff[MAXBUFF];

        print("Write letter \"q\" to exit\nPress \"i\" to increase the font\nPress \"d\" to decrease the font\n", MAXBUFF);

        my_get_string(buff, MAXBUFF);
        
        if(buff[0] =='q' && buff[1] == '\0'){
            paint_all_vd(BLACK);
            return;
        }
        else if(buff[0] == 'i' && buff[1] == '\0'){
            increaseFontSize();
            paint_all_vd(BLACK);
            //return;
        }
        else if(buff[0] == 'd' && buff[1] == '\0'){
            decreaseFontSize();
            paint_all_vd(BLACK);
            //return;
        }
        else{
            paint_all_vd(BLACK);
            err_print("Invalid amount!! \n",18); 
        }
    }
    paint_all_vd(0x000000);
}

uint64_t regs[18];
static char * regstxt[18]={"RAX:", "RBX:", "RCX:", "RDX:", "RDI:", "RSI:", "RBP:", "RSP:", "R8:", "R9:", "R10:", "R11:", "R12:", "R13:", "R14:", "R15:", "RIP:", "RFLAGS:" };

void show_regs(){
    paint_all_vd(BLACK);
    if(get_current_pixel_size() > 2){
        print("FontSize too big\nDecrease fontSize to see al Registers", MAXBUFF);
    }
    else{
        paint_all_vd(BLACK);
        int b= register_snapshot(regs);
        char buffer[17];
        if(b==1){
            for(int i=0; i<18; i++){
            print(regstxt[i], 4);
            uint_to_base(regs[i], buffer, 16);
            buffer[16]=0;
            print(buffer, 16);
            print("\n", 2);
            }
        }else{
            paint_all_vd(BLACK);
            print("No hay registro para imprimir \n", 25);
        }
    }
    return;
}
