#include <shell_apps.h>



// void my_get_string(char* buff, int count){
//     int i=0;
//     char c=0;
//     char s[2];
//     while( ( c = get_char_user() ) && i < (count -1)){
//         if(c=='\b'){
//             if(i!=0){
//              i--;
//              errase_char(0x000000);
//             }
//             buff[i]=0;
//         }
//         if ( c != '\b' && c!='=' && i < count - 1 && c != 0 ) {
//             s[0]=c;
//             s[1]=0;
// 			print ( s, 1 );
// 			buff[i++] = c;
// 		}
//     }

//     print("\n",1);
//     buff[i]='\0';
// }



void shell_loop(char** argv, uint64_t argc){
   if(argc!=1){
      err_print("ERROR: argc", 30);
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

void read_line_with_backspace(char *buffer, int max_len) {
    int i = 0;
    char c;
    char s[2] = {0};

    while (i < max_len - 1) {
         c = get_char_user();

         if (c = 0) { // Valor especial devuelto por tu handler para Ctrl+D
            buffer[0] = '\0';
            return 1; // EOF detectado
         }

        if (c == '\b') {
            if (i > 0) {
                i--;
                buffer[i] = '\0';
                errase_char(0x000000); // Borra el carácter en pantalla
            }
        } else if (c != 0 && c >= 32 && c < 127) { // caracteres imprimibles
            buffer[i++] = c;
            s[0] = c;
            print(s, 1); // Imprimir el carácter
        }
    }

    buffer[i] = '\0';
    print("\n", 1); // Avanzar de línea al terminar
}

#define MAX_LINE 128

void shell_cat() {
    char buffer[MAX_LINE];
    while (1) {
        read_line_with_backspace(buffer, MAX_LINE);
        if (buffer[0] == 0) break; // Salir si se presiona Enter sin texto
        print(buffer, 1);
        print("\n", 1);
    }
}

void shell_wc(){
   char c;
   char s[2];
   int64_t l=1;
   while((c=get_char_user())>0){
      if(c=='\n'){
         l++;
      }
      s[0]=c;
      s[1]=0;
      print(s, 1);
   }
   if(c=='\b'){
      errase_char(0x000000);
   }
   print("Lines: ",7);
   char st[3];
   itoa(l, s);
   print(st,3);
}

int8_t vowel(char c) {
    return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' ||
           c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U';
}

void shell_filter(){
   char c;
   char s[2];
   while((c=get_char_user())>0 ){
      if(c=='\b'){
         errase_char(0x000000);
      }
      if(!vowel(c)){
         s[0]=c;
         s[1]=0;
         print(s, 1);
      }
   }
   print("\n",1);
}