#include <keyboard.h>
#include <naiveConsole.h> 

extern int key_pressed();

unsigned char scan_code = 0;
static char char_to_ret[10] = {0};
int current=0;

void keyboard_handler() {
    // Obtener el scan_code 
    scan_code = key_pressed();

    // Mapa de scan_codes a caracteres
    char scan_codes[128] = {
        0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',   
        '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',     
        0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\',
        'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '-', 0, 0, 0, '+', 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    // Verificar si el scan_code es válido y si corresponde a una tecla imprimible
    if (scan_code < 0 || scan_code >= 128) {
        return;
    }

    for(int i = 0; i < 10; i++){
        if (char_to_ret[i] == 0)
        {
            char_to_ret[i]=scan_codes[scan_code];
            break;
        }
    }
    //char_to_ret[0] = scan_codes[scan_code];
}

int get_regs(){
    return (char_to_ret[0]=='=');
}


void cleanchar_to_ret(){
    char_to_ret[0] = 0;
    if(char_to_ret[1] != 0){
        for (int i = 0; i < 9; i++)
        {
            char_to_ret[i] = char_to_ret[i+1];
        }
    }
}

char get_char_pressed(){
    if (char_to_ret[0]!=0)
    {
        char to_ret = char_to_ret[0];
        cleanchar_to_ret();
        return to_ret;
    }
    return 0; //AGREGADO POR EL WARNING (CHEQUEAR !!!!!)


}
