
#include <philos.h>

static table_t table;
static int64_t philosopher(char ** argv, int argc);

uint8_t is_even(uint64_t id){
    return (id%2==0);
}

uint64_t left(uint64_t i, uint64_t n){
    return (i+n-1)%n;
}

uint64_t right(uint64_t i, uint64_t n){
    return (i+1)%n;
}

void print_state(){
    sem_wait(table.mutex);
    for(int i=0; i<table.amount; i++){
        if(table.philos_array[i].state==EATING){
            print ("E ", 2);
        }else{
            print (". ", 2);
        };
    }
    print ("\n", 1);
    sem_post(table.mutex);
}

int philos_init_error(char *msg, int i){
    err_print(msg, my_strlen(msg));
    for(int j=0; j<i; j++){
        sem_close(table.philos_array[j].left_fork);
        my_kill(table.philos_array[j].pid); 
    }
    return -1;
}

int64_t init_philos(){
    table.amount=MIN_PHILOS;
    table.fds[STDIN] = STDIN;
    table.fds[STDOUT] = STDOUT;
    table.fds[STDERR] = STDERR;

    for(int i=0; i<MIN_PHILOS; i++){
        table.philos_array[i].left_fork= sem_open_get_id(1);

        if(table.philos_array[i].left_fork==-1){
            return philos_init_error( "ERROR: error abriendo semaforos\n",i);
        }
    }
    int left;

    for(int i=0; i<MIN_PHILOS; i++){
        if(i+1==MIN_PHILOS){
             left=0;
        }
        else {
             left=i+1;
        }
        table.philos_array[i].right_fork = table.philos_array[left].left_fork;
        
        if(new_philo(i) ==-1){
            return philos_init_error( "ERROR: error creando proceso de filosofo\n",i);}
        }
        
    return 0;
}

int64_t philos(char **argv, uint64_t argc){
    table.amount= MIN_PHILOS;
    print_instructions();

    if(open_mutexes()==-1){
        return -1;
    }

    if(init_philos()==-1){
        close_mutexes();
        return -1;
    }
    
    keyboard_handler();
    clean_resources();
    return 0;
}

void think(){
    nano_sleep(THINKING_TIME);
}

void take_forks(int i){
    sem_wait(table.mutex);
    int border = (i == 0 || i == table.amount - 1);
    sem_post(table.mutex);

    if (border) {
        sem_wait(table.border_mutex);
    }

    if(is_even(i)){
        sem_wait(table.philos_array[i].left_fork);
        sem_wait(table.philos_array[i].right_fork);
    } else {
        sem_wait(table.philos_array[i].right_fork);
        sem_wait(table.philos_array[i].left_fork);
    }
}

void eat(int i){

    sem_wait(table.mutex);

    table.philos_array[i].state=EATING;
    
    nano_sleep(EATING_TIME);

    sem_post(table.mutex);

    print_state(); 

    sem_wait(table.mutex);
    table.philos_array[i].state=THINKING;
    sem_post(table.mutex);
}

void put_forks(uint64_t i){

    if(is_even(i)){
        sem_post(table.philos_array[i].right_fork);
        sem_post(table.philos_array[i].left_fork); 
    } else {
        sem_post(table.philos_array[i].left_fork); 
        sem_post(table.philos_array[i].right_fork);
    }

    if (i == 0 || i == table.amount - 1) {
        sem_post(table.border_mutex);
    }
    
}

void clean_resources(){
    sem_wait(table.mutex);
    for(int i=0; i<table.amount; i++){
        sem_close(table.philos_array[i].left_fork);
        my_kill(table.philos_array[i].pid);
    }
    sem_post(table.mutex);
    close_mutexes();
}

int philos_add_remove_error(char *msg){
    sem_post(table.mutex);
    err_print(msg, my_strlen(msg));
    return -1;
}

int new_philo(int i){
    table.philos_array[i].state = THINKING;
    char index[4];  
    itoa(i, index);

    char* argv[] = {"filosofo", index, NULL};

    table.philos_array[i].pid = my_create_process((void*)philosopher, argv, 2, 0, table.fds);
    if (table.philos_array[i].pid == -1) {
        print("Error al crear filosofo ", MAXBUFF);
    }
    sem_post(table.mutex);
    return table.philos_array[i].pid;
}

int add_philo(){
    sem_wait(table.mutex);

    if(table.amount==MAX_PHILOS){
        return philos_add_remove_error("Error: No podes agregar mas filosofos el maximo es: 10 \n");
    }

    int i=table.amount;

    sem_wait(table.border_mutex);
 
    table.philos_array[i].left_fork=sem_open_get_id(1);

    if(table.philos_array[i].left_fork==-1){
         return philos_add_remove_error("Error: abriendo semaforos \n");
    }
    table.amount++;

    table.philos_array[i-1].right_fork = table.philos_array[i].left_fork;
    table.philos_array[i].right_fork = table.philos_array[0].left_fork;
    table.philos_array[i].state = THINKING;

    if ( new_philo(i) == -1){
        sem_close(table.philos_array[i].left_fork);
        table.philos_array[i-1].right_fork = table.philos_array[0].left_fork;
        table.amount--;
        return philos_add_remove_error("Error: creando el proceso\n");
    }

    sem_post(table.border_mutex);
    sem_post(table.mutex);
    return 0;
}

int remove_philo() {
    sem_wait(table.border_mutex);
    sem_wait(table.mutex);

    if (table.amount <= MIN_PHILOS) {
        sem_post(table.mutex);
        sem_post(table.border_mutex);
        return philos_add_remove_error("Error: no podes eliminar mas filosofos, el minimo es 5.\n");
    }

    int last = table.amount - 1;

    my_kill(table.philos_array[last].pid);
    sem_close(table.philos_array[last].left_fork);

    table.philos_array[last - 1].right_fork = table.philos_array[0].left_fork;

    table.amount--;

    sem_post(table.mutex);
    sem_post(table.border_mutex);

    return 0;
}

int64_t philosopher(char ** argv, int argc){
    if (argc<1){
         return -1;
    }

    int i = satoi(argv[0]);
    if(i < 0 || i >= MAX_PHILOS){
        err_print("ERROR: indice de filosofo invalido\n", MAXBUFF);
        return -1;
    }

    while(1){
        think();
        take_forks(i);
        eat(i);
        put_forks(i);
    }
}

int open_mutexes(){
    table.mutex=sem_open_get_id(1);
    if(table.mutex==-1){
        err_print("ERROR: error abriendo mutex\n", MAXBUFF);
        return-1;
    }
    table.border_mutex = sem_open_get_id(1);
    if (table.border_mutex == -1) {
        sem_close(table.mutex);
        err_print("ERROR: error abriendo border_mutex\n", MAXBUFF);
        return -1;
    }
    return 0;   
}

void close_mutexes(){
    sem_close(table.mutex);
    sem_close(table.border_mutex);
}

void keyboard_handler(){
    char c;

    while((c = get_char_user()) != EOF && c != 'q'){
      
        if(c == ADD){
            add_philo();
        } else if(c == REMOVE){
            remove_philo();
        } 
    }
}

void print_instructions(){
    print("Toca la tecla 'a' para agregar un filosofo y la tecla 'r' para borrar uno.\n"
        "Toca la tecla 'q' para salir.\n"
      "El minimo de filosofos es 5 y el maximo 10.\n"
      "E en la posicion del filosofo significa que esta comiendo.\n"
      ". en la posicion del filosofo significa que esta pensando.\n", 500);
}


