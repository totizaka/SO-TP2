
#include <philos.h>

static table_t table;
static int64_t philosopher(char ** argv, int argc);


void print_state(){
    sem_wait(table.array_mutex);
    for(int i=0; i<table.amount; i++){
        if(table.philos_array[i].state==EATING){
            print ("E", 1);
        }else{
            print (".", 1);
        }
        print(" ", 1);
    }
    print ("\n", 1);
    sem_post(table.array_mutex);
}


void wait_sems(){
    sem_wait(table.num_mutex);
    sem_wait(table.array_mutex);
}

void post_sems(){
    sem_post(table.array_mutex);
    sem_post(table.num_mutex);
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

    for(int i=0; i<MIN_PHILOS; i++){
        table.philos_array[i].right_fork = table.philos_array[(i + 1) % MIN_PHILOS].left_fork;
        
        if(   new_philo(i) ==-1){
            return philos_init_error( "ERROR: error creando proceso de filosofo\n",i);}
        }
        
    return 0;
}

uint8_t is_even(uint64_t id){
    return (id%2==0);
}

// uint64_t left(uint64_t i, uint64_t n){
//     return (i+n-1)%n;
// }
// uint64_t right(uint64_t i, uint64_t n){
//     return (i+1)%n;
// }


int64_t philos(char **argv, uint64_t argc){
    table.amount= MIN_PHILOS;
    print_instructions();
    print('a', 1);


    if(open_mutexes()==-1){//si solo abro los mutex cambiar nombre func
       print('b', 1);
        return -1;
    }
    print('c', 1);

    if(init_philos()==-1){
        close_mutexes();//limpiar recursos
        print('d', 1);

        return -1;
    }
    print('f', 1);

    print_state();
    keyboard_handler();
    clean_resources();
    return 0;
}


void think(){
    nano_sleep(THINKING_TIME);
}

void take_forks(int i){
     print("Filosofo ", 9); 
    print(" intentando tomar tenedores\n", 28);

     if(is_even(i)){
            sem_wait(table.num_mutex);
            sem_wait(table.philos_array[i].left_fork);
            sem_wait(table.philos_array[i].right_fork);
            sem_post(table.num_mutex);
    }else{
            sem_wait(table.num_mutex);
            sem_wait(table.philos_array[i].right_fork);
            sem_wait(table.philos_array[i].left_fork);
            sem_post(table.num_mutex);
        }
}
void eat(int i){
        print("Filosofo ", 9);
        print(" comiendo\n", 10);

    sem_wait(table.array_mutex);
    table.philos_array[i].state=EATING;
    print_state();
    sem_post(table.array_mutex);

    nano_sleep(EATING_TIME);

    sem_wait(table.array_mutex);
    table.philos_array[i].state=THINKING;
    print_state();
    sem_post(table.array_mutex);
}

void put_forks(uint64_t i){
        sem_wait(table.num_mutex);
        sem_post(table.philos_array[i].right_fork);
        sem_post(table.philos_array[i].left_fork); 
        sem_post(table.num_mutex);    
}


void clean_resources(){
    sem_wait(table.array_mutex);
    for(int i=0; i<table.amount; i++){
        sem_close(table.philos_array[i].left_fork);
        my_kill(table.philos_array[i].pid);
    }
    sem_post(table.array_mutex);
    close_mutexes();
}

int philos_add_remove_error(char *msg){
    post_sems();
    err_print(msg, my_strlen(msg));
    return -1;
}

int new_philo(int i){
    table.philos_array[i].state = THINKING;
    char index[4];  // suficiente para un número de hasta 3 cifras + null terminator
    itoa(i, index);

    char* argv[] = {"filosofo", index, NULL};

    table.philos_array[i].pid = my_create_process((void*)philosopher, argv, 2, 0, table.fds);
    if (table.philos_array[i].pid == -1) {
        print("Error al crear filosofo ", 25);
    }
    return table.philos_array[i].pid;
}

int add_philo(){
    wait_sems();

    if(table.amount==MAX_PHILOS){
        return philos_add_remove_error("Error: No podes agregar mas filosofos el maximo es:10 \n");
    }

    int i=table.amount;

    table.philos_array[i].left_fork=sem_open_get_id(1);
    if(table.philos_array[i].left_fork==-1){
         return philos_add_remove_error("Error: abriendo semaforos \n");
    }

    table.philos_array[i - 1].right_fork = table.philos_array[i].left_fork;
    table.philos_array[i].right_fork = table.philos_array[0].left_fork;
    table.philos_array[i].state = THINKING;



    if (  new_philo(i) ==-1){
        sem_close(table.philos_array[i].left_fork);
        table.philos_array[i - 1].right_fork = table.philos_array[0].left_fork;
        return philos_add_remove_error("Error: creando el proceso\n");
    }
    table.amount++;

    post_sems();
    print_state();
    return 0;
}


int  remove_philo(){
    wait_sems();
    if(table.amount<=MIN_PHILOS){
        return philos_add_remove_error("Error: no podes eliminar mas filosofos el minimo es:5 \n");
    }

    int last= table.amount - 1;
    my_kill(table.philos_array[last ].pid); 
    sem_close(table.philos_array[last].left_fork);

    table.philos_array[last-1].right_fork= table.philos_array[0].left_fork;
    
    table.amount--;

    post_sems();
    print_state();
    return 0;
}

//por ahi meterle argv, argc si quiero pasarle i
int64_t philosopher(char ** argv, int argc){

    print("philosopher lanzado\n", 21);

    if (argc<=1){
        print("keeeeeee filosofo ", 18);
         return -1;
    }

    print("argv[1] = ", 10);

    print(argv[1], my_strlen(argv[1]));
    print("\n", 1);

    int i = satoi(argv[1]);

    if(i < 0 || i >= MAX_PHILOS){
        err_print("ERROR: índice de filósofo inválido\n", 35);
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
    table.array_mutex=sem_open_get_id(1);
    if(table.array_mutex==-1){
        err_print("ERROR: error abriendo semaforo\n", 35);
        return-1;
    }

    table.num_mutex= sem_open_get_id(1);
        if(table.num_mutex==-1){
            err_print("ERROR: error abriendo semaforo\n", 35);
            sem_close(table.array_mutex);
            return-1;
        }
    return 0;   
}

void close_mutexes(){
    sem_close(table.array_mutex);
    sem_close(table.num_mutex);
}

void keyboard_handler(){
    char c;
    while((c= get_char_user())!=EOF){
        if(c==ADD){
            add_philo();
        } else if(c==REMOVE){
            remove_philo();
        } 
        else if(c==STATE){
            print_state();
        }

    }    
}

void print_instructions(){
    print("Toca la tecla 'a' para agregar un filosofo y la tecla 'r' para borrar uno.\n"
      "El minimo de filosofos es 5 y el maximo 10.\n"
      "E en la posicion del filosofo significa que esta comiendo.\n"
      ". en la posicion del filosofo significa que esta pensando.\n", 200);
}




