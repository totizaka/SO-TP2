/*#define ADD 'a'
#define REMOVE 'r'
#define MAX_PHILOS 10
#define MIN_PHILOS 5


typedef enum {
    FREE=0,
    THINKING,
    EATING,
    HUNGRY
}philo_state;

typedef struct philo_t{
    uint64_t pid; //proceso
    philo_state state;
    uint left_fork;
    uint right_fork;
}philo_t;



static uint8_t num_philos=0;
static philo_t philos[MAX_PHILOS]={0};
static int64_t sems[MAX_PHILOS];
static int64_t array_mutex;
static int64_t num_mutex;



uint8_t is_even(uint id){
    return id%2;
}

uint left(uint i, uint n){
    return (i+n-1)%n
}
uint right(uint i, uint n){
    return (i+1)%n
}

void think(){
    //sleep(tiempo t) <- falta syscall
}
void take_forks(int i){
     if(is_even(i)){
            sem_wait(sems[left(i)]);
            sem_wait(sems[right(i)]);
        }else{
            sem_wait(sems[right(i)]);
            sem_wait(sems[left(i)]);
        }
}
void eat(int i){
    sem_wait(array_mutex);
    philos[i].state=EATING;
    //tengo q imprimir estado??
    sem_post(array_mutex);

    //simulo q come -->sleep

    sem_wait(array_mutex);
    philos[i].state=THINKING;
    sem_post(array_mutex);

}
void put_forks(uint i){
    if(is_even(i)){
            sem_post(sems[left(i)]);
            sem_post(sems[right(i)]);
        }else{
            sem_post(sems[right(i)]);
            sem_post(sems[left(i)]);
        }
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

void add_philo(){
    if(num_philos==MAX_PHILOS){
        err_print("Error: No podes agregar mas filosofos el maximo es:10 \n");
    }
    philos[num_philos++].pid= my_create_process((void*)philosopher, 1, argv, argc);
    //ver q onda argv y argc ?? el index de philo??
}
void remove_philo(){
    if(num_philos==MIN_PHILOS){
        err_print("Error: no podes eliminar mas filosofos el minimo es:5 \n");
        return;
    }
    my_kill(philos[num_philos].pid); //el indice puede estar mal
    philos[num_philos--].pid=0;
    //falta cerrar sems de este filosof
}

//por ahi meterle argv, argc si quiero pasarle i
void philosopher(int i){
    while(1){
       
        think();
        take_forks(i);
        eat(i);
        put_forks(i);
    }
}

void open_mutexes(){
    array_mutex=sem_open_get_id(1);
        if(array_mutex==-1){
            err_print("ERROR: error abriendo semaforo\n", 35);
            return;
        }
    num_mutex= sem_open_get_id(1);
        if(num_mutex==-1){
            err_print("ERROR: error abriendo semaforo\n", 35);
            sem_close(array_mutex);
            return;
        }
        
}
int64_t init_philos(){
    for(int i=0; i<MIN_PHILOS; i++){
        philos[i].pid=my_create_process((void*)philosopher, 1, argv, argc);
        //ver q onda argv y argc ?? el index de philo??
        if(philos[i].pid==-1){
             err_print("ERROR: error creando proceso de filosofo\n", 35);
             i--;
             //limpieza recursos
            for(int j=i; j=>0; j--){
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
            for(int j=i; j=>0; j--){
                sem_close(sems[j]);
                sems[j]=0;
                my_kill(philos[j].pid); //el indice puede estar mal
                philos[j].pid=0;
            }
            return;
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

uint64_t philos(char *argv[], uint64_t argc){
num_philos= MIN_PHILOS;

print("Toca la tecla 'a' para agregar un filosofo y la tecla 'r' para borrar uno. \n", 50);
print("El minimo de filosofos es 5 y el maximo 10", 50);
print(" E en la posicion del filosofo significa que esta comiendo\n", 50);
print(" . en la posicion del filosofo significa que esta pensando\n", 50);
//sleep() para que usuario llegue a leer
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
*/

