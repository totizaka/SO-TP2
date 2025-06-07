#include <shell.h>
#include <test_mm.h>
#include <test_prio.h>
#include <test_processes.h>
#include <test_sync.h>



module menu[] ={{"help", help}, {"snake", snake}, {"regvalues",show_regs},{"fontsize", font_size},{"time", show_time},
{"div0", div0_exc}, {"opcode", opcode_exc}, {"mmtest", mm_test_shell}, {"testprio", prio_test_shell}, 
{"testprocesses", proc_test_shell}, {"testsyncro", sync_test_shell}, {"ps", ps}, {"memstate", show_mem_state}};

uint64_t regs[18];
static char * regstxt[18]={"RAX:", "RBX:", "RCX:", "RDX:", "RDI:", "RSI:", "RBP:", "RSP:", "R8:", "R9:", "R10:", "R11:", "R12:", "R13:", "R14:", "R15:", "RIP:", "RFLAGS:" };




typedef struct command{
char * name; 
int argc;
char ** argv;
}command;

static command commands[2];
void help(){
    paint_all_vd(BLACK);
    print("To print the different functions of the shell >> enter: help\n", MAXBUFF);
    print("To play Snake Game >> enter: snake\n", MAXBUFF);
    print("To show snapshot of the Register values >> enter: regvalues\n", MAXBUFF);
    print("To change the font size >> enter: fontsize \n", MAXBUFF);
    print("To show current time >> enter: time \n", MAXBUFF);
    print("To try Divide by Zero Exception>> enter: div0 \n", MAXBUFF);
    print("To try Invalid Opcode Exception >> enter: opcode \n", MAXBUFF);
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

//fijarnos como hacer para llamar a funcion con parametros
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
//ver si tenemos en userland un enum de prios??
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
    if ((new_prio = satoi(argv[1])) <= 0){
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

//VER CON ARGUMENTOS, NO HARDCODEAR LOS TESTS

void mm_test_shell(){
    paint_all_vd(BLACK);
    fd_t fd={ STDIN, STDOUT, STDERR };
    int pid  = my_create_process_shell((void(*))test_mm, NULL, 0, 0,fd);
}

void proc_test_shell(){
    paint_all_vd(BLACK);
     fd_t fd={ STDIN, STDOUT, STDERR };

    char *argv[] = {"test_processes", "30", NULL};  
    int64_t pid = my_create_process_shell((void(*))test_processes, argv, 2, 0, fd);
}

void prio_test_shell(){
    paint_all_vd(BLACK);
    fd_t fd={ STDIN, STDOUT, STDERR };

    char *argv[] = {"test_prio", NULL};
    int64_t pid= my_create_process_shell((void(*))test_prio, argv, 1, 0, fd);
}

void sync_tests(char* use_sem){
    //ver si me armo mini funcion para armarme los argv con malloc??
    fd_t fd={ STDIN, STDOUT, STDERR };
    char * argv[]={"sync_test","5", use_sem, NULL };
    int64_t pid= my_create_process_shell((void(*))test_sync, argv, 2, 0, fd);
}

void sync_test_shell(){
    paint_all_vd(BLACK);
    char* use_sem="1\0";
    sync_tests(use_sem);
    //ver si tiro msjito dependiendo valor ret??
}

void no_sync_test_shell(){
    paint_all_vd(BLACK);
    char* use_sem="0\0";
    sync_tests(use_sem);
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

        get_string(buff, MAXBUFF);

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
       paint_all_vd(0x000000);
    err_print("Invalid Command!! \n",18);
}

void run_piped_program(char * comand1, char* comand2){
    int c1=-1;
    int c2=-1;
    for(int i=0; i<menuDIM+1; i++){
        if(strcmp_user(comand1,menu[i].name)==0){
            c1=i;
        }
        if(strcmp_user(comand2,menu[i].name)==0){
            c2=i;
        }
    }
    if(c1==-1|| c2==-1){
        invalid_comand();
        return;
    }


    int64_t id_pipe = my_get_available_pipe_id();

    if (id_pipe==-1){
        err_print("No hay pipes disponibles\n",24);
        return;
    }
    else
    {
       err_print("SI HAY PIPES DISPONIBLES\n",24);
        return;
    }
    

    //COMANDO 1= WRITER 
    //COMANDO 2= READER
    
    fd_t com1_fds[FD_MAX]={STDIN,id_pipe+FD_MAX,STDERR}; //pongo la salida al pipe 
    fd_t com2_fds[FD_MAX]={id_pipe+FD_MAX,STDOUT,STDERR};//pongo la entrada al pipe

    char *argv[] = {"", NULL};
    
    my_create_process_shell((void(*))menu[c1].function, 0, 0, 0, com1_fds);
    my_create_process_shell((void(*))menu[c2].function, 0, 0, 0,com2_fds);

    my_close_pipe(id_pipe);

    }



void run_simple_program(char* comand){
    for(int i=0; i<menuDIM+1; i++){
                    if(strcmp_user(comand,menu[i].name)==0){
                        menu[i].function();
                        return;
                    }
                }
                paint_all_vd(0x000000);
                err_print("Invalid Command!! \n",18);

}

void command_wait(){
    while (1){
        print("\n",1);
        print("shell_TP_ARQUI$> ", MAXBUFF);

        char buff[MAXBUFF];

        get_string(buff, MAXBUFF);


        if (strlen_user(buff)!=0){

            comands_pipe comands = get_comands_pipe(buff);

            if (comands.pipe){
               run_piped_program(comands.cm1,comands.cm2);          
            }
            else {
                run_simple_program(buff);
                }
            }     
        }

    }
    


void font_size(){
    
    paint_all_vd(BLACK);

    while(1){
        char buff[MAXBUFF];

        print("Write letter \"q\" to exit\nPress \"i\" to increase the font\nPress \"d\" to decrease the font\n", MAXBUFF);

        get_string(buff, MAXBUFF);
        
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

//no esta terminado
/*void philos_shell(){
    paint_all_vd(BLACK);
    int pid  = my_create_process_shell((void(*))philos, NULL, 0, 0);
}*/



int64_t get_argv(char ** args, char * str){
    if(str==NULL){
        return 0;
    }
    int argc=0;
    char buf[100]={0};
    int j=0;
    for(int i=0; str[i]!='\0' && str[i]!='|' && str[i]!=']'; i++){
        if(str[i]==','){
            args[argc]=my_malloc(sizeof(char)*j);
            my_strcpy(args[argc], buf);
            argc++;
        }
        buf[j]=str[i];
    }
    return argc;
}


int64_t parse_command(char * str, int num_cmd){
    int i=0;
    for(; str[i]!='\0' && str[i]!='|';i++){
        if(str[i]=='['){
            commands[num_cmd].argv=my_malloc(sizeof(char*)*4);//ver max args???
            commands[num_cmd].argc=get_argv(commands[num_cmd].argv,str+i);
        }
    }
}

int64_t parse (char* str){
    int count=0;
    int i=0;
    while(*str!='\0'){
        i=parse_command(str, count);
        str+i;
    }
}