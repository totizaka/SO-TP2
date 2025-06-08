#include <shell_apps.h>

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

void shell_cat(){
   char c;
   while((c=get_char_user())>=0){
      print(&c, 1);
   }
   print("\n",1);
}
void shell_wc(){
   char c;
   int64_t l=1;
   while((c=get_char_user())>=0){
      if(c=='\n'){
         l++;
      }
      print(&c, 1);
   }
   print("Lines: ",7);
   char s[3];
   itoa(l, s);
   print(s,3);
}

int8_t vowel(char c) {
    return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' ||
           c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U';
}

void shell_filter(){
    char c;
   while((c=get_char_user())>=0){
      if(!vowel(c)){
         print(&c, 1);
      }
   }
   print("\n",1);
}