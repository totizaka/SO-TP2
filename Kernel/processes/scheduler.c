#include <scheduler.h>


int compare_elem(list_elem_t e1, list_elem_t e2){
    return ((PCB*)e1)->pid - ((PCB*)e2)->pid;
}


void initialize_lists(){
    t_cmp cmp= compare_elem;

    readys=new_list(cmp);
    blockeds=new_list(cmp);
    initialized=1;
    to_begin(readys);
}

//Cuando interumpe el timer se llama a esta funcion
void time_interruptions_handler(){
    if (running == NULL || running->state != RUNNING)
        return;

    //Chequear esto : esta hecho pensando que sacamos los running de readys 
    if(--(running->time) <= 0){
        running->state = READY;
        add_list(readys, running);//Si no sacamos los running de readys BORRAR esto
        running = NULL;
    }

}

//IDEA: tener un quantum y multiplicarlo por la prioridad 
//LISTA DE PROCESOS READY

//En la clase dijieron: armar lista de los readys y lista de los blocked y pasar por los readys 

uint64_t scheduler(uint64_t current_rsp){
    if(!initialized){
        iniatialize_lists();
    }

    //Guardamos el contexto
    if(running != NULL){
        running->rsp = current_rsp;
    }

    //Vemos si no hay procesos listos

    if(is_empty(readys)){
        //proceso q hace hlt falta hacerlo
        return current_rsp; 
    }


    if(!has_next(readys)){
        to_begin(readys); //lista circular
    }

    running = (PCB*) next(readys);
    running->state=RUNNING;
    running->time= QUANTUM * (10-running->priority);//depende de que algoritmo usemos. esto es pensadoq eu mayor prioridad = 0 y menor =10

    return running->rsp;
}