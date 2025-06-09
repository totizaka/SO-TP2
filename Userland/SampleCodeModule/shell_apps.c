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

        if (c == EOF) {
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

// shell_apps commands

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