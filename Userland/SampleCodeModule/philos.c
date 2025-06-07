
#include <philos.h>




typedef struct philo_t{
    uint64_t pid; //proceso
    philo_state state;
}philo_t;



static uint8_t num_philos=0;
static philo_t philos[MAX_PHILOS]={0};
static int64_t sems[MAX_PHILOS];
static int64_t array_mutex;
static int64_t num_mutex;


void print_state();

uint8_t is_even(uint64_t id){
    return id%2;
}

uint64_t left(uint64_t i, uint64_t n){
    return (i+n-1)%n
}
uint64_t right(uint64_t i, uint64_t n){
    return (i+1)%n
}

void think(){
    nano_sleep(THINKING_TIME);
}
void take_forks(int i){
     if(is_even(i)){
            sem_wait(num_mutex);
            sem_wait(sems[left(i, num_philos)]);
            sem_wait(sems[right(i, num_philos)]);
            sem_post(num_mutex);
    }else{
            sem_wait(num_mutex);
            sem_wait(sems[right(i, num_philos)]);
            sem_wait(sems[left(i, num_philos)]);
            sem_post(num_mutex);
        }
}
void eat(int i){
    sem_wait(array_mutex);
    philos[i].state=EATING;
    print_state();
    sem_post(array_mutex);

    nano_sleep(EATING_TIME);

    sem_wait(array_mutex);
    philos[i].state=THINKING;
    print_state();
    sem_post(array_mutex);

}
void put_forks(uint64_t i){
        sem_wait(num_mutex);
        sem_post(sems[right(i, num_philos)]);
        sem_post(sems[left(i, num_philos)]); 
        sem_post(num_mutex);    
}

void print_state(){
    for(int i=0; i<num_philos; i++){
        if(philos[i].state==EATING){
            print ("E", 1);
        }else{
            print (".", 1);
        }
        print(" ", 1);
    }
    print ("\n", 1);
}
void clean_resources(){
    sem_wait(array_mutex);
    for(int i=0; i<num_philos; i++){
        sem_close(sems);
        my_kill(philos[i].pid);
    }
    sem_post(array_mutex);
    close_mutexes();
}

void add_philo(){
    sem_wait(num_mutex);
    sem_wait(array_mutex);
    if(num_philos==MAX_PHILOS){
        err_print("Error: No podes agregar mas filosofos el maximo es:10 \n", 50);
        sem_post(num_mutex);
        return;
    }
    philos[num_philos++].pid= my_create_process((void*)philosopher, NULL, 0, 0);
    sem_post(array_mutex);
    sem_post(num_mutex);
    sems[right(num_philos, num_philos)]=sem_open_get_id(1);
}

void remove_philo(){
    sem_wait(num_mutex);
    sem_wait(array_mutex);
    if(num_philos==MIN_PHILOS){
        err_print("Error: no podes eliminar mas filosofos el minimo es:5 \n",50);
        sem_post(num_mutex);
        return;
    }
    my_kill(philos[num_philos].pid); //el indice puede estar mal
    philos[num_philos--].pid=0;
    sem_post(num_mutex);
    sem_post(array_mutex);
    sem_close(sems[right(num_philos+1, num_philos+1)]);
}

//por ahi meterle argv, argc si quiero pasarle i
void philosopher(char ** argv, argc){
    int i;
     if ((i = satoi(argv[1])) <= 0){
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
    array_mutex=sem_open_get_id(1);
        if(array_mutex==-1){
            err_print("ERROR: error abriendo semaforo\n", 35);
            return-1;
        }
    num_mutex= sem_open_get_id(1);
        if(num_mutex==-1){
            err_print("ERROR: error abriendo semaforo\n", 35);
            sem_close(array_mutex);
            return-1;
        }
        
}
void close_mutexes(){
    sem_close(array_mutex);
    sem_close(num_mutex);
}

int64_t init_philos(){
    char[5] n=" ";
    char[5] s;
    for(int i=0; i<=num_philos; i++){
        itoa(i,s);
        char * argv[2]={n,s};
        philos[i].pid=my_create_process((void*)philosopher, argv, 2, 0);
        //ver q onda argv y argc ?? el index de philo??
        if(philos[i].pid==-1){
             err_print("ERROR: error creando proceso de filosofo\n", 35);
             i--;
             //limpieza recursos
            for(int j=i; j>=0; j--){
                sem_close(sems[j]);
                sems[j]=0;
                my_kill(philos[j].pid); //el indice puede estar mal
                philos[j].pid=0;
            }
            return -1;
        }
        sems[i]=sem_open_get_id(1);
        if(sems[i]==-1){
            err_print("ERROR: error abriendo semaforo\n", 35);
            sems[i]=0;
            my_kill(philos[i].pid); //el indice puede estar mal
            philos[i--].pid=0;
            for(int j=i; j>=0; j--){
                sem_close(sems[j]);
                sems[j]=0;
                my_kill(philos[j].pid); //el indice puede estar mal
                philos[j].pid=0;
            }
            return -1;
        }

    }
}

void keyboard_handler(){
    char c;
    while(c!=EOF){
     c=get_char_user();
        if(c==ADD){
            add_philo();
        } else if(c==REMOVE){
            remove_philo();
        } else{
            //ver q hago?? imprimo che bro la tecla q estas tocando no hace nada aca???
        }
    }    
}
void print_instructions(){
    print("Toca la tecla 'a' para agregar un filosofo y la tecla 'r' para borrar uno. \n", 50);
    print("El minimo de filosofos es 5 y el maximo 10", 50);
    print(" E en la posicion del filosofo significa que esta comiendo\n", 50);
    print(" . en la posicion del filosofo significa que esta pensando\n", 50);
    nano_sleep(10000);// para que usuario llegue a leer
}

uint64_t philos(char *argv[], uint64_t argc){

    num_philos= MIN_PHILOS;
    print_instructions();

    if(open_mutexes()==-1){//si solo abro los mutex cambiar nombre func
    return -1;
    }
    if(init_philos()==-1){
    close_mutexes();//limpiar recursos
     return -1;
    }

keyboard_handler();

clean_resources();



}



