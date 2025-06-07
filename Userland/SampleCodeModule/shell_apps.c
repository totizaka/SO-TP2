#include <shell_apps.h>

void shell_loop(uint8_t segs){
     int pid;
        char buff[30]="Hola PID: ";
    while(1){
       pid == my_getpid();
       itoa(pid, buff+10);
       print(buff, 30);
       nano_sleep(segs);
    }
}