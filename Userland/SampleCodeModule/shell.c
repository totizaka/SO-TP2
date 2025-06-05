#include <shell.h>
#include <test_mm.h>
#include <test_prio.h>
#include <test_processes.h>
#include <test_sync.h>


module menu[] ={{"help", help}, {"snake", snake}, {"regvalues",show_regs},{"fontsize", font_size},{"time", show_time},
{"div0", div0_exc}, {"opcode", opcode_exc}, {"mmtest", mm_test_shell}, {"testprio", prio_test_shell}, 
{"testprocesses", proc_test_shell},{"testsync", sync_test_shell}, {"testnosync", no_sync_test_shell} };

uint64_t regs[18];
static char * regstxt[18]={"RAX:", "RBX:", "RCX:", "RDX:", "RDI:", "RSI:", "RBP:", "RSP:", "R8:", "R9:", "R10:", "R11:", "R12:", "R13:", "R14:", "R15:", "RIP:", "RFLAGS:" };

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

//VER CON ARGUMENTOS, NO HARDCODEAR LOS TESTS

void mm_test_shell(){
    paint_all_vd(BLACK);
    // test_mm(1, "100000");
    // char args[1][6] = {"100000"};
    int64_t ret;
    int pid= my_create_process((void(*))test_mm, 1, NULL, 0);
    my_wait(pid, &ret);
}

void proc_test_shell(){
    paint_all_vd(BLACK);
    int64_t ret;
    char **argv = my_malloc(sizeof(char *) * 2);  
    argv[0] = my_malloc(sizeof(char) * 3);  
    if (argv[0] == NULL) return NULL;
    my_strcpy(argv[0], "30");
    argv[1] = NULL;

    int64_t pid= my_create_process((void(*))test_processes, 1, argv, 1);
    my_wait(pid, &ret);
    my_free(argv[0]);
    my_free(argv);
}

void prio_test_shell(){
    paint_all_vd(BLACK);
    int64_t ret;
    int64_t pid= my_create_process((void(*))test_prio, 1, NULL, 0);
    my_wait(pid, &ret);
}

void sync_tests(char* use_sem){
    //ver si me armo mini funcion para armarme los argv con malloc??
    char **argv = my_malloc(sizeof(char *) * 3);
    argv[0] = my_malloc(sizeof(char)*2); 
    if (argv[0] == NULL) return NULL;
    my_strcpy(argv[0], "5");
    argv[1] = my_malloc(sizeof(char)*2);
    if (argv[1] == NULL) return NULL;
    my_strcpy(argv[1], use_sem);

    argv[2] = NULL;
    int64_t ret;
    int64_t pid= my_create_process((void(*))test_sync, 1, argv, 2);
    my_wait(pid, &ret);
    my_free(argv[0]);
    my_free(argv[1]);
    my_free(argv);
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



void command_wait(){
    while (1){
        print("\n",1);
        print("shell_TP_ARQUI$> ", MAXBUFF);

        char buff[MAXBUFF];

        get_string(buff, MAXBUFF);

        if(strlen_user(buff)!=0){
            for(int i=0; i<menuDIM+1; i++){
                if(strcmp_user(buff,menu[i].name)==0){
                    menu[i].function();
                    return;
                }
            }
            paint_all_vd(0x000000);
            err_print("Invalid Command!! \n",18);
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

