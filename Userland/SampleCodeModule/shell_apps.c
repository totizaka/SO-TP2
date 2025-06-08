#include <shell_apps.h>

void shell_loop(char** argv, uint64_t argc){
   if(argc!=2){
      err_print("ERROR: seconds required", 30);
   }
   int segs;
   if ((segs = satoi(argv[1])) <= 0){
      err_print("ERROR: seconds required", 30);
         return;
   }
   int pid;
        char buff[30]="Hola PID: ";
    while(1){
       pid == my_getpid();
       itoa(pid, buff+10);
       print(buff, 30);
       nano_sleep(segs);
    }
}