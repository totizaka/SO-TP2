#include <stdlib_user.h>
#include "snake.h"

#define menuDIM 7
#define MAXBUFF 100
#define BLACK 0x000000


typedef struct module {
    char * name;
    void (*function)();
}module;


void help();
void snake();
void show_regs();
void font_size();
void printTime(); 
void div0Exc();
void opcodeExc();
void showTime();
extern void div0();
extern void opcode_exc();


module menu[] ={{"help", help}, {"snake", snake}, {"regvalues",show_regs},{"fontsize", font_size},{"time", showTime},
{"div0", div0Exc}, {"opcode", opcodeExc}};

uint64_t regs[18];
static char * regstxt[18]={"RAX:", "RBX:", "RCX:", "RDX:", "RDI:", "RSI:", "RBP:", "RSP:", "R8:", "R9:", "R10:", "R11:", "R12:", "R13:", "R14:", "R15:", "RIP:", "RFLAGS:" };

void help(){
    paintAll_vd(BLACK);
    print("To print the different functions of the shell >> enter: help\n", MAXBUFF);
    print("To play Snake Game >> enter: snake\n", MAXBUFF);
    print("To show snapshot of the Register values >> enter: regvalues\n", MAXBUFF);
    print("To change the font size >> enter: fontsize \n", MAXBUFF);
    print("To show current time >> enter: time \n", MAXBUFF);
    print("To try Divide by Zero Exception>> enter: div0 \n", MAXBUFF);
    print("To try Invalid Opcode Exception >> enter: opcode \n", MAXBUFF);
    return;
}

void opcodeExc(){
    paintAll_vd(BLACK);
    if(getCurrentPixelSize() > 2){
        print("FontSize too big\nDecrease fontSize to see al Registers", MAXBUFF);
    }
    else{
        opcode_exc();
    }
}
void div0Exc(){
    paintAll_vd(BLACK);
    if(getCurrentPixelSize() > 2){
        print("Fontsize too big\nDecrease fontSize to see al Registers", MAXBUFF);
    }
    else{
        div0();
    }
}

void showTime(){
    paintAll_vd(BLACK);
    printTime();
}


void snake(){
    paintAll_vd(BLACK);

    char buff[MAXBUFF];

    int aux1 = getCurrentPixelSize();
    setPixelSize(3);
    print("WELCOME TO THE SNAKE-GAME\n", MAXBUFF);
    setPixelSize(2);
    print("Key Control:\nPlayer1: 'w', 'd', 'a', 's'\nPlayer2: 'i', 'l', 'j', 'k'\n", MAXBUFF);
    int correctAmount=0;
    while(!correctAmount){

        print("\nWrite letter \"q\" to exit\n\nIntroduce Players to start game (1 or 2): ", MAXBUFF);

        getString(buff, MAXBUFF);

        if(buff[0] =='q'&& buff[1] == '\0'){
            setPixelSize(aux1);
            paintAll_vd(BLACK);
            return;
        }
        else if(buff[0] =='1'&& buff[1] == '\0'){
            correctAmount=1;
            uint8_t aux2 = getCurrentPixelSize();
            gameLoop1(); //while (gameLoop=='r') y en gameloop pongo que get char hasta q sea r o q(si es q sale ) 
            setPixelSize(aux2);
        }
        else if (buff[0] =='2' && buff[1] == '\0'){
            correctAmount=1;
            uint8_t aux2 = getCurrentPixelSize();
            gameLoop2();
            setPixelSize(aux2);
        }
        else{
            paintAll_vd(0x000000);
            err_print("Invalid amount!! \n",18); 
        }
    }
    setPixelSize(aux1);
    paintAll_vd(BLACK);
    return;
}


void command_wait(){
    while (1){
        print("\n",1);
        print("shell_TP_ARQUI$> ", MAXBUFF);

        char buff[MAXBUFF];

        getString(buff, MAXBUFF);

        if(strlen(buff)!=0){
            for(int i=0; i<menuDIM; i++){
                if(strcmp(buff,menu[i].name)==0){
                    menu[i].function();
                    return;
                }
            }
            paintAll_vd(0x000000);
            err_print("Invalid Command!! \n",18);
        }
    }
    
}

void font_size(){
    
    paintAll_vd(BLACK);

    while(1){
        char buff[MAXBUFF];

        print("Write letter \"q\" to exit\nPress \"i\" to increase the font\nPress \"d\" to decrease the font\n", MAXBUFF);

        getString(buff, MAXBUFF);
        
        if(buff[0] =='q' && buff[1] == '\0'){
            paintAll_vd(BLACK);
            return;
        }
        else if(buff[0] == 'i' && buff[1] == '\0'){
            increaseFontSize();
            paintAll_vd(BLACK);
            //return;
        }
        else if(buff[0] == 'd' && buff[1] == '\0'){
            decreaseFontSize();
            paintAll_vd(BLACK);
            //return;
        }
        else{
            paintAll_vd(BLACK);
            err_print("Invalid amount!! \n",18); 
        }
    }
    paintAll_vd(0x000000);
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
    paintAll_vd(BLACK);
    if(getCurrentPixelSize() > 2){
        print("FontSize too big\nDecrease fontSize to see al Registers", MAXBUFF);
    }
    else{
        paintAll_vd(BLACK);
        int b= register_snapshot(regs);
        char buffer[17];
        if(b==1){
            for(int i=0; i<18; i++){
            print(regstxt[i], 4);
            uintToBase(regs[i], buffer, 16);
            buffer[16]=0;
            print(buffer, 16);
            print("\n", 2);
            }
        }else{
            paintAll_vd(BLACK);
            print("No hay registro para imprimir \n", 25);
        }
    }
    return;
}

