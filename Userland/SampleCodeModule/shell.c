#include <shell.h>
#include <test_mm.h>
#include <test_prio.h>
#include <test_processes.h>
#include <test_sync.h>

char **prepare_args_generic(parsed_command *cmd, const char *cmd_name, const char *default_arg) {
    static char *argv[10];
    int argc = 0;

    argv[argc++] = (char *)cmd_name;

    if (cmd->argc > 0) {
        for (int i = 0; i < cmd->argc && i < 8; i++) {
            argv[argc++] = cmd->args[i];
        }
    } else if (default_arg != NULL) {
        argv[argc++] = (char *)default_arg;
    }

    argv[argc] = NULL;
    cmd->argc = argc;
    return argv;
}

char **prepare_args_loop(parsed_command *cmd) {
    return prepare_args_generic(cmd, "loop", "30");
}

char **prepare_args_test_processes(parsed_command *cmd) {
    return prepare_args_generic(cmd, "test_processes", "30");
}

char **prepare_args_test_syncro(parsed_command *cmd) {
    static char *argv[4];
    int argc = 0;

    argv[argc++] = "test_sync";

    if (cmd->argc > 0) {
        for (int i = 0; i < cmd->argc && i < 2; i++) {
            argv[argc++] = cmd->args[i];
        }
    } else {
        argv[argc++] = "5";
        argv[argc++] = "1";
    }
    argv[argc] = NULL;
    cmd->argc = argc;
    return argv;
}

char **prepare_args_test_syncro_no_sem(parsed_command *cmd) {
    static char *argv[4];
    int argc = 0;

    argv[argc++] = "test_sync";

    if (cmd->argc > 0) {
        for (int i = 0; i < cmd->argc && i < 2; i++) {
            argv[argc++] = cmd->args[i];
        }
    } else {
        argv[argc++] = "5";
        argv[argc++] = "0";
    }
    argv[argc] = NULL;
    cmd->argc = argc;
    return argv;
}

char **prepare_args_test_a(parsed_command *cmd) {
    return prepare_args_generic(cmd, "test_a", NULL);
}

char **prepare_args_test_prio(parsed_command *cmd) {
    return prepare_args_generic(cmd, "test_prio", NULL);
}

char **prepare_args_default(parsed_command *cmd) {
    // Simplemente usa los argumentos tal cual están
    return cmd->args;
}

char **prepare_args_cat(parsed_command *cmd) {
    return prepare_args_generic(cmd, "cat", NULL);
}

char **prepare_args_filter(parsed_command *cmd) {
    return prepare_args_generic(cmd, "filter", NULL);
}

char **prepare_args_wc(parsed_command *cmd) {
    return prepare_args_generic(cmd, "wc", NULL);
}

char **prepare_args_ps(parsed_command *cmd) {
    return prepare_args_generic(cmd, "ps", NULL);
}

char **prepare_args_mem_state(parsed_command *cmd) {
    return prepare_args_generic(cmd, "memstate", NULL);
}

char **prepare_args_writer(parsed_command *cmd) {
    return prepare_args_generic(cmd, "writer", NULL);
}

char **prepare_args_reader(parsed_command *cmd) {
    return prepare_args_generic(cmd, "reader", NULL);
}

char **prepare_args_philos(parsed_command *cmd) {
    return prepare_args_generic(cmd, "philos", NULL);
}

module menu[] = {
    {"help", help, prepare_args_default, BUILTIN}, 
    {"clear", clear_screen, NULL, BUILTIN},
    {"snake", snake, prepare_args_default, BUILTIN}, 
    {"regvalues", show_regs, prepare_args_default, BUILTIN},
    {"fontsize", font_size, prepare_args_default, BUILTIN},
    {"time", show_time, prepare_args_default, BUILTIN},
    {"div0", div0_exc, prepare_args_default, BUILTIN}, 
    {"opcode", opcode_exc, prepare_args_default, BUILTIN}, 
    {"mmtest", test_mm, prepare_args_default, NOT_BUILTIN}, 
    {"testprio", test_prio, prepare_args_test_prio, NOT_BUILTIN}, 
    {"testprocesses", test_processes, prepare_args_test_processes, NOT_BUILTIN}, 
    {"testsyncro-sem", test_sync, prepare_args_test_syncro, NOT_BUILTIN}, 
    {"testsyncro-no-sem", test_sync, prepare_args_test_syncro_no_sem, NOT_BUILTIN},
    {"ps", ps, prepare_args_ps, NOT_BUILTIN},
    {"mem", show_mem_state, prepare_args_mem_state, NOT_BUILTIN}, 
    {"testa", t_a, prepare_args_test_a, NOT_BUILTIN},
    {"writer", write_process_test, prepare_args_default, NOT_BUILTIN},
    {"reader", read_process_test, prepare_args_default, NOT_BUILTIN},
    {"loop", shell_loop, prepare_args_loop, NOT_BUILTIN},
    {"cat", shell_cat,prepare_args_cat, NOT_BUILTIN},
    {"filter", shell_filter, prepare_args_filter, NOT_BUILTIN},
    {"philos", philos_shell,prepare_args_philos, NOT_BUILTIN},
    {"wc", shell_wc,prepare_args_wc, NOT_BUILTIN},

};

uint64_t regs[18];
static char * regstxt[18]={"RAX:", "RBX:", "RCX:", "RDX:", "RDI:", "RSI:", "RBP:", "RSP:", "R8:", "R9:", "R10:", "R11:", "R12:", "R13:", "R14:", "R15:", "RIP:", "RFLAGS:" };

// para funciones con arguemntos
typedef int (*cmd_func_t)(char **args, int argc);

typedef struct {
    char *name;
    cmd_func_t func;
} special_command_t;

// array de funciones con argumentos
special_command_t special_commands[] = {
    {"kill", shell_kill},
    {"nice", shell_nice},
    {"block", shell_block},
    {"unblock", shell_unblock},
    {NULL, NULL}
};

char **get_args_preparer(parsed_command * cmd) {
    for (int i = 0; i < sizeof(menu) / sizeof(menu[0]); i++) {
        if (my_strcmp(menu[i].name, cmd->name) == 0) {
            return menu[i].arg_preparer(cmd);
        }
    }
    return cmd->args; 
}

void help(){
    paint_all_vd(BLACK);
    print("Enter: help >> To print the different functions of the shell\n", MAXBUFF);
    print("Enter: clear >> To clear the screen\n", MAXBUFF);
    print("Enter: snake >> To play the Snake Game\n", MAXBUFF);
    print("Enter: regvalues >> To show snapshot of the Register values\n", MAXBUFF);
    print("Enter: fontsize >> To change the font size\n", MAXBUFF);
    print("Enter: time >> To show the current time\n", MAXBUFF);
    print("Enter: div0 >> To try Divide by Zero Exception\n", MAXBUFF);
    print("Enter: opcode >> To try Invalid Opcode Exception\n", MAXBUFF);
    print("Enter: ps >> To see the current processes\n", MAXBUFF);
    print("Enter: mem >> To see the current memory state\n", MAXBUFF);
    print("Enter: kill [pid] >> To kill a process with the given PID\n", MAXBUFF);
    print("Enter: nice [pid,new_priority] >> To change the priority of a process with the given PID, new_priority must be a        number between 1(LOWEST) and 7(HIGHEST)\n", MAXBUFF);
    print("Enter: block [pid] >> To block a process with the given PID\n", MAXBUFF);
    print("Enter: unblock [pid] >> To unblock a process with the given PID\n", MAXBUFF);
    print("\n\nApartado de tests de la catedra:\n", MAXBUFF);
    print("Enter: mmtest >> To test the Memory Manager\n", MAXBUFF);
    print("Enter: testprio >> To test the Process Manager\n", MAXBUFF);
    print("Enter: testprocesses >> To test the Priority Manager\n", MAXBUFF);
    print("Enter: testsyncro-sem >> To test Synchronization\n", MAXBUFF);
    print("Enter: testsyncro-no-sem >> To test Synchronization without semaphores\n", MAXBUFF);
    return;
}

void ps(){
    paint_all_vd(BLACK);
    my_ps();
    return;
}

void show_mem_state(){
    paint_all_vd(BLACK);
    my_mem_state();
    return;
}

void shell_kill(char ** argv, uint64_t argc){
     paint_all_vd(BLACK);
    if(argc!=1){
        return;
    }
    int64_t pid;
    if ((pid = satoi(argv[0])) <= 0){
        print("ERROR: error getting PID", 30);
      return;
    }
    my_kill(pid);
    return;
}

void shell_nice(char** argv, uint64_t argc){
    paint_all_vd(BLACK);
    if(argc!=2){
        return;
    }
    int64_t pid;
    int64_t new_prio;
    if ((pid = satoi(argv[0])) <= 0){
        print("ERROR: error getting PID", 30);
      return;
    }
    new_prio = satoi(argv[1]);
    if (new_prio <= 0 || new_prio > 7){
        print("ERROR: error getting new priority", 30);
      return;
    }  
    my_nice(pid, new_prio);
    return;
}

void shell_block(char ** argv, uint64_t argc){
    paint_all_vd(BLACK);
    if(argc!=1){
        return;
    }
    int64_t pid;
    if ((pid = satoi(argv[0])) <= 0){
        print("ERROR: error getting PID", 30);
      return;
    }
    my_block(pid);
    return;
}

void shell_unblock(char ** argv, uint64_t argc){
     paint_all_vd(BLACK);
    if(argc!=1){
        return;
    }
    int64_t pid;
    if ((pid = satoi(argv[0])) <= 0){
        print("ERROR: error getting PID", 30);
      return;
    }
    my_unblock(pid);
    return;  
}

//VER CON ARGUMENTOS, NO HARDCODEAR LOS TESTS, DEJARLOS COMO ESTAN EN EL REPO DE LA CATEDRA

void write_process_test(){
    char *to_print = "holis!! ESTOY PIPEANDOOOO";
    int num_byte = my_strlen(to_print);
    if (my_write(STDOUT, to_print, num_byte)!=-1){
        return;
    }
    else{
        print("no anda my_write", MAXBUFF);
    }
    if (STDOUT >= FD_MAX) my_close_pipe(STDOUT);

}

void read_process_test(){
    char buff[64];
    int read=my_read(STDIN,buff,63);
    if (read > 0) {
        buff[read] = '\0';
        print("Leido del pipe: ", 16);
        print(buff, read);
        print("\n", 1);
    }
    else {
        print("Error al leer del pipe\n", 22);
    }
    if (STDOUT >= FD_MAX) my_close_pipe(STDIN);

}


void slow_writer_test() {
    char *msg = "Mensaje lento por pipe...";
    for (int i = 0; msg[i] != 0; i++) {
        my_write(STDOUT, &msg[i], 1);
        for (volatile int j = 0; j < 1000000; j++); // Pequeño delay
    }
    if (STDOUT >= FD_MAX) my_close_pipe(STDOUT);
}

void fast_reader_test() {
    char buff[64];
    int read;
    while ((read = my_read(STDIN, buff, 63)) > 0) {
        buff[read] = '\0';
        print("Leido: ", 7);
        print(buff, read);
    }
    if (STDIN >= FD_MAX) my_close_pipe(STDIN);
}
void blocking_reader_test() {
    char buff[64];
    int read = my_read(STDIN, buff, 63);
    if (read > 0) {
        buff[read] = '\0';
        print("Reader recibió: ", 17);
        print(buff, read);
    } else {
        print("Reader no recibió nada\n", 24);
    }
    if (STDIN >= FD_MAX) my_close_pipe(STDIN);
}

void orphan_writer_test() {
    char *msg = "Esto no debería llegar a nadie";
    int res = my_write(STDOUT, msg, my_strlen(msg));
    if (res == -1) {
        print("No hay reader, write falló como corresponde\n", 41);
    } else {
        print("ERROR: write no falló\n", 23);
    }
    if (STDOUT >= FD_MAX) my_close_pipe(STDOUT);
}

void eof_reader_test() {
    char buff[64];
    int read;
    while ((read = my_read(STDIN, buff, 63)) > 0) {
        buff[read] = '\0';
        print("EOF Reader: ", 12);
        print(buff, read);
    }
    print("EOF recibido, reader termina\n", 29);
    if (STDIN >= FD_MAX) my_close_pipe(STDIN);
}

void clear_screen(){
    paint_all_vd(BLACK);
    return;
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



void invalid_comand(){
    // paint_all_vd(0x000000);
    print("Invalid Command!! \n",18);
    err_print("Invalid Command!! \n",18);
}

parsed_command parse_command(char *input) {
    parsed_command result = {0};
    result.argc = 0;

    char *arg_start = my_strchr(input, '[');
    if (arg_start != NULL) {
        int len = arg_start - input;
        my_strncpy(result.name, input, len);
        result.name[len] = '\0';

        arg_start++; // saltar '['
        char *arg_end = my_strchr(arg_start, ']');
        if (arg_end != NULL) {
            *arg_end = '\0';
            char *token = my_strtok(arg_start, ",");
            while (token != NULL && result.argc < 8) {
                result.args[result.argc++] = token;
                token = my_strtok(NULL, ",");
            }
        }
    } else {
        my_strcpy(result.name, input);
    }

    return result;
}

int run_special_command(parsed_command cmd) {
    for (int i = 0; special_commands[i].name != NULL; i++) {
        if (my_strcmp(cmd.name, special_commands[i].name) == 0) {
            special_commands[i].func(cmd.args, cmd.argc);
            return 1;
        }
    }
    return 0;
}

void run_piped_program(char *input1, char *input2) {
    parsed_command cmd1 = parse_command(input1);
    parsed_command cmd2 = parse_command(input2);

    int c1 = -1, c2 = -1;
    for (int i = 0; i < menuDIM + 1; i++) {
        if (my_strcmp(cmd1.name, menu[i].name) == 0) c1 = i;
        if (my_strcmp(cmd2.name, menu[i].name) == 0) c2 = i;
    }

    if (run_special_command(cmd1) || run_special_command(cmd2)) return;

    if (c1 == -1 || c2 == -1) {
        invalid_comand();
        return;
    }

    int64_t id_pipe = my_get_available_pipe_id();
    if (id_pipe == -1) {
        err_print("No hay pipes disponibles\n", 24);
        return;
    }

    fd_t fds1[FD_MAX] = { STDIN, id_pipe + FD_MAX, STDERR };   // escritor
    fd_t fds2[FD_MAX] = { id_pipe + FD_MAX, STDOUT, STDERR };  // lector

    clear_screen();
    uint64_t pid1 = my_create_process((void (*)())menu[c1].function, cmd1.args, cmd1.argc, 0, fds1);
    uint64_t pid2 = my_create_process((void (*)())menu[c2].function, cmd2.args, cmd2.argc, 0, fds2);

    if (pid1 == -1 || pid2 == -1) {
        err_print("Fallo la creacion de procesos con pipe\n", 39);
        return;
    }

    my_wait(pid1, NULL);

    my_wait(pid2, NULL);

    my_close_pipe(id_pipe);  // solo después de que ambos hayan terminado
}

void run_simple_program(char* input) {
    parsed_command cmd = parse_command(input);

    if (run_special_command(cmd)) return;

    for (int i = 0; i < menuDIM; i++) {
        if (my_strcmp(cmd.name, menu[i].name) == 0) {
            if (menu[i].is_builtin) {
                // Ejecutar directamente el comando built-in
                menu[i].function();
            } else {
                // Crear un proceso para comandos no built-in
                char **args = get_args_preparer(&cmd);
                fd_t fds[FD_MAX] = {STDIN, STDOUT, STDERR};
                clear_screen();
                my_create_process_shell((void (*)())menu[i].function, args, cmd.argc, 0, fds);
            }
            return;
        }
    }

    paint_all_vd(0x000000);
    err_print("Invalid Command!! \n", 18);
}

void command_wait(){
        print("\n",1);
        print("shell_TP_ARQUI$> ", MAXBUFF);

        char buff[MAXBUFF];

        my_get_string(buff, MAXBUFF);


        if (my_strlen(buff)!=0){

            comands_pipe comands = get_comands_pipe(buff);

            if (comands.pipe){
                run_piped_program(comands.cm1,comands.cm2);          
            }
            else {
                run_simple_program(buff);
                }
            }else{
                return;
            }
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


void shell(){
    print("Available Features:\n",30);
     for(int i=0; i<menuDIM; i++){
         print(menu[i].name, MAXBUFF);
         print("\n", 1);
     }

    while(1){
        command_wait();
    }

    return ;
}

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

void philos_shell(){
    fd_t fds[FD_MAX]={STDIN,STDOUT,STDERR};
    int pid  = my_create_process_shell((void(*))philos, NULL, 0, 0,fds );
}
