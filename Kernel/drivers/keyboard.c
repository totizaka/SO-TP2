// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <keyboard.h>
#include <naiveConsole.h> 
#include <process.h>

#define BUFF_MAX 50
#define MYEOF 0 // Definimos EOF como 0

extern int key_pressed();

uint8_t scan_code = 0;
static char char_to_ret[BUFF_MAX] = {0};
int current=0;
int dim=0;
PCB*  blocked=NULL;

void keyboard_handler() {
    // Obtener el scan_code 
    scan_code = key_pressed();

    // Mapa de scan_codes a caracteres
    char scan_codes[128] = {
        0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',   
        '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',     
        0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '|', // antes donde esta "|" estaba "\\"
        'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '-', 0, 0, 0, '+', 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

   if ( scan_code >= 128) {
        return;
    }

    // Detectar CTRL+C y CTRL+D
    static int ctrl_pressed = 0;
    if (scan_code == 0x1D) { // Scan code de CTRL
        ctrl_pressed = 1;
        return;
    } else if (scan_code == 0x9D) { // Ctrl LIBERADO
        ctrl_pressed = 0;
        return; // No meter Ctrl en el buffer
    }
    if (scan_code == 0x2E && ctrl_pressed) { // Scan code de C
        _cli(); // Deshabilitar interrupciones
        ctrl_c_handler(); // Llamar al manejador en process.c
        ctrl_pressed = 0; // Resetear el estado de CTRL
        return;
    } else if (scan_code == 0x20 && ctrl_pressed) { // Scan code de D
        _cli();
        if (dim < BUFF_MAX){
            char_to_ret[dim] = MYEOF; // 0 lo usamos como señal de EOF en read_line
            dim++;
            if (blocked != NULL) {
                ready(blocked);
                blocked = NULL;
            }
        }
        ctrl_pressed = 0;
        return; // No meter Ctrl+D en el buffer
    } else {
        ctrl_pressed = 0; // Resetear si no es CTRL+C
    }

	if ( dim < BUFF_MAX ) {
        if (scan_codes[scan_code] == 0) {
            return; // No meter en el buffer si no es un caracter imprimible
        }
        char_to_ret[dim] = scan_codes[scan_code];
		dim++;
	} else if ( current == dim ) {
		char_to_ret[0] = scan_codes[scan_code];
		dim = 1;
		current = 0;
	} else {
		return;
	}
	if ( blocked != NULL ) {
		ready ( blocked );
		blocked = NULL;
	}
}

int get_regs(){
    return (char_to_ret[0]=='=');
}



char get_char_pressed(){
    if (buf_has_next()){
        char to_ret = char_to_ret[current];
        current++;
        return to_ret;
    }
    return 0; //AGREGADO POR EL WARNING (CHEQUEAR !!!!!)
}



int buf_has_next(){
	return ( dim > 0 ) && ( current < dim );
}


int read_stdin(char * buff, int count){
    if(blocked!=NULL){
        return -1;
    }
    if(!buf_has_next()){
        blocked=get_running();
        block(blocked);
    }
    int i=0;
    while(buf_has_next() && i<count && char_to_ret[current]!=MYEOF){
        buff[i++]=get_char_pressed();
    }
    if ( char_to_ret[current] == MYEOF) {
		get_char_pressed();
	}
    return i;
}