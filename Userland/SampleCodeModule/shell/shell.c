// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <shell.h>
#include <test_mm.h>
#include <test_prio.h>
#include <test_processes.h>
#include <test_sync.h>

int run_special_command(parsed_command cmd);
void run_piped_program(char *input1, char *input2, int is_background);
void run_simple_program(char* input, int is_background);
parsed_command parse_command(char *input);


module menu[] = {
    {"help", help, BUILTIN},
    {"clear", clear_screen, BUILTIN},
    {"ps", my_ps, BUILTIN},
    {"mem", my_mem_state, BUILTIN},
    {"loop", shell_loop,  NOT_BUILTIN},
    {"cat", shell_cat, NOT_BUILTIN},
    {"filter", shell_filter, NOT_BUILTIN},
    {"wc", shell_wc, NOT_BUILTIN},
    {"phylos", (void*)philos, NOT_BUILTIN},
    {"mmtest", (void*)test_mm, NOT_BUILTIN},
    {"testprio", (void*)test_prio, NOT_BUILTIN},
    {"testprocesses", (void*)test_processes, NOT_BUILTIN}, 
    {"testsyncro", (void*)test_sync, NOT_BUILTIN},
};

// array de funciones con argumentos
special_command_t special_commands[] = {
    {"kill", (void*)shell_kill},
    {"nice", (void*)shell_nice},
    {"block", (void*)shell_block},
    {"unblock", (void*)shell_unblock},
    {NULL, NULL}
};

void help(){
    paint_all_vd(BLACK);
    print("Enter: help >> To print the different functions of the shell\n", MAXBUFF);
    print("Enter: clear >> To clear the screen\n", MAXBUFF);
    print("Enter: ps >> To see the current processes\n", MAXBUFF);
    print("Enter: mem >> To see the current memory state\n", MAXBUFF);
    print("Enter: loop[segs] >> To test the shell loop\n", MAXBUFF);
    print("Enter: cat >> To test the shell cat command\n", MAXBUFF);
    print("Enter: filter >> To test the shell filter command\n", MAXBUFF);
    print("Enter: wc >> To test the shell wc command\n", MAXBUFF);
    print("Enter: phylos >> To test the philosophers problem\n", MAXBUFF);
    print("Enter: kill [pid] >> To kill a process with the given PID\n", MAXBUFF);
    print("Enter: nice [pid,new_priority] >> To change the priority of a process with the given PID, new_priority must be a        number between 1(LOWEST) and 5(HIGHEST)\n", MAXBUFF);
    print("Enter: block [pid] >> To block a process with the given PID\n", MAXBUFF);
    print("Enter: unblock [pid] >> To unblock a process with the given PID\n", MAXBUFF);
    print("\n\nApartado de tests de la catedra:\n", MAXBUFF);
    print("Enter: mmtest >> To test the Memory Manager\n", MAXBUFF);
    print("Enter: testprio >> To test the Process Manager\n", MAXBUFF);
    print("Enter: testprocesses[max-proc] >> To test the Priority Manager\n", MAXBUFF);
    print("Enter: testsyncro[n, use-sem] >> To test Synchronization\n", MAXBUFF);
    return;
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

void command_wait(){
    print("\n",1);
    print("shell_TP_SO$> ", MAXBUFF);

    char buff[MAXBUFF];

    my_get_string(buff, MAXBUFF);


    if (my_strlen(buff)!=0){

        comands_pipe comands = get_comand_background_pipe(buff);
        comand_background comand_bg = get_comand_background(buff);

        if (comands.pipe && comands.background) {
            run_piped_program(comands.cm1, comands.cm2, IS_BACKGROUND);          
        }
        else if (comands.pipe && !comands.background) {
            run_piped_program(comands.cm1, comands.cm2, IS_FOREGROUND);
        }
        else if (comand_bg.background){
            run_simple_program(comand_bg.cm, IS_BACKGROUND);
        }
        else {
            run_simple_program(buff, IS_FOREGROUND);
        }
        }else{
            return;
    }
}

void run_piped_program(char *input1, char *input2, int is_background) {
    parsed_command cmd1 = parse_command(input1);
    parsed_command cmd2 = parse_command(input2);

    int c1 = -1, c2 = -1;
    for (int i = 0; i < menuDIM ; i++) {
        if (my_strcmp(cmd1.name, menu[i].name) == 0){
            c1 = i;
            cmd1.args[0] = menu[i].name;  // Asegurarse de que el primer argumento sea el nombre del comando
        }
        if (my_strcmp(cmd2.name, menu[i].name) == 0){
            c2 = i;
            cmd2.args[0] = menu[i].name;  // Asegurarse de que el primer argumento sea el nombre del comando
        }
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

    if(!is_background){
        uint64_t pid1 = my_create_process((void*)menu[c1].function, cmd1.args, cmd1.argc, IS_FOREGROUND, fds1);
        uint64_t pid2 = my_create_process((void*)menu[c2].function, cmd2.args, cmd2.argc, IS_FOREGROUND, fds2);

        if (pid1 == -1 || pid2 == -1) {
            err_print("Fallo la creacion de procesos con pipe\n", 39);
            return;
        }
        my_wait(pid1, NULL);
        my_wait(pid2, NULL);
        my_close_pipe(id_pipe);  // solo después de que ambos hayan terminado

    } else {
        my_create_process((void*)menu[c1].function, cmd1.args, cmd1.argc, IS_BACKGROUND, fds1);
        my_create_process((void*)menu[c2].function, cmd2.args, cmd2.argc, IS_BACKGROUND, fds2);
    }
}

void run_simple_program(char* input, int is_background) {
    parsed_command cmd = parse_command(input);

    if (run_special_command(cmd)) return;

    for (int i = 0; i < menuDIM; i++) {
        if (my_strcmp(cmd.name, menu[i].name) == 0) {
            if (menu[i].is_builtin) {                   // Ejecutar directamente el comando built-in
                clear_screen();
                menu[i].function();
            } else if (is_background){
                cmd.args[0] = menu[i].name;
                fd_t fds[FD_MAX] = {STDIN, STDOUT, STDERR};
                clear_screen();
                my_create_process_shell((void (*)())menu[i].function, cmd.args, cmd.argc, IS_BACKGROUND, fds);
            } else {
                cmd.args[0] = menu[i].name;
                fd_t fds[FD_MAX] = {STDIN, STDOUT, STDERR};
                clear_screen();
                my_create_process_shell((void (*)())menu[i].function, cmd.args, cmd.argc, IS_FOREGROUND, fds);
            }
            return;
        }
    }
    clear_screen();
    err_print("Invalid Command!! \n", 18);
}


parsed_command parse_command(char *input) {
    parsed_command result = {0};
    result.argc = 1;                                //porque el name va en agrv[0]

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

// shell functions

void shell_kill(char ** argv, uint64_t argc){
     paint_all_vd(BLACK);
    if(argc!=2){
        return;
    }
    int64_t pid;
    if ((pid = satoi(argv[1])) <= 1){
        err_print("ERROR: invalid PID", MAXBUFF);
      return;
    }
    my_kill(pid);
    return;
}

void shell_nice(char** argv, uint64_t argc){
    paint_all_vd(BLACK);
    if(argc!=3){
        return;
    }
    int64_t pid;
    int64_t new_prio;
    if ((pid = satoi(argv[1])) <= 1){
        err_print("ERROR: invalid PID", MAXBUFF);
      return;
    }
    new_prio = satoi(argv[2]);
    if (new_prio <= 0 || new_prio > 5){
        err_print("ERROR: error getting new priority, must be between 1 and 5", MAXBUFF);
      return;
    }  
    my_nice(pid, new_prio);
    return;
}

void shell_block(char ** argv, uint64_t argc){
    paint_all_vd(BLACK);
    if(argc!=2){
        return;
    }
    int64_t pid;
    if ((pid = satoi(argv[1])) <= 1){
        err_print("ERROR: invalid PID", MAXBUFF);
      return;
    }
    my_block(pid);
    return;
}

void shell_unblock(char ** argv, uint64_t argc){
     paint_all_vd(BLACK);
    if(argc!=2){
        return;
    }
    int64_t pid;
    if ((pid = satoi(argv[1])) <= 1){
        err_print("ERROR: invalid PID", MAXBUFF);
      return;
    }
    my_unblock(pid);
    return;  
}
