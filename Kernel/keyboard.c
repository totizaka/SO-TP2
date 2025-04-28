#include <keyboard.h>
#include <naiveConsole.h> 

extern int keyPressed();

unsigned char scancode = 0;
static char charToRet[10] = {0};
int current=0;

void keyboard_handler() {
    // Obtener el scancode 
    scancode = keyPressed();

    // Mapa de scancodes a caracteres
    char scanCodes[128] = {
        0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',   
        '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',     
        0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\',
        'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '-', 0, 0, 0, '+', 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    // Verificar si el scancode es válido y si corresponde a una tecla imprimible
    if (scancode < 0 || scancode >= 128) {
        return;
    }

    for(int i = 0; i < 10; i++){
        if (charToRet[i] == 0)
        {
            charToRet[i]=scanCodes[scancode];
            break;
        }
    }
    //charToRet[0] = scanCodes[scancode];
}

int get_regs(){
    return (charToRet[0]=='=');
}


void cleanCharToRet(){
    charToRet[0] = 0;
    if(charToRet[1] != 0){
        for (int i = 0; i < 9; i++)
        {
            charToRet[i] = charToRet[i+1];
        }
    }
}

char getCharPressed(){
    if (charToRet[0]!=0)
    {
        char toRet = charToRet[0];
        cleanCharToRet();
        return toRet;
    }
    return 0; //AGREGADO POR EL WARNING (CHEQUEAR !!!!!)


}
