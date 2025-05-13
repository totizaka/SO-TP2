#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <stdio.h>
#include "stdlib_user.h"
#include <snake.h>

//Seed global 
uint32_t seed;

//Seteamos la semilla en los segundos;
void set_seed(){
    seed = get_seconds();
}

//Generador de numeros pseoudo-aleatorios
uint32_t rand_num() {
    seed = seed * 1664525 + 1013904223;
    return seed;
}

//Dibujo de grillas 
void draw_lines(){
    for (int i = DIM_HEADER; i <= DIM_BOARD; i++){
        for (int j = DIM_HEADER; j <= DIM_BOARD; j+=40)
        {
            draw_pixel(BLACK_COLOR, i, j);
            draw_pixel(BLACK_COLOR, j, i);
        }
    }
}

void draw_background(){
    for (int i = DIM_HEADER; i <= DIM_BOARD; i++){
        for (int j = DIM_HEADER; j <= DIM_BOARD; j++)
        {
            draw_pixel(BACKGROUND_COLOR, i, j);
        }
    }
}

uint32_t rand_position_x() {
    int aux1 = (rand_num() % DIM_BOARD) + 1;
    if (aux1<DIM_HEADER)
    {
        return ((aux1+DIM_HEADER)/40)*40;
    }
    return (aux1/40)*40;
}
uint32_t rand_position_y() {
    int aux2 = (rand_num() % DIM_BOARD) + 1;
    if (aux2<DIM_HEADER)
    {
        return ((aux2+DIM_HEADER)/40)*40;
    }
    return (aux2/40)*40;
}


// Función para inicializar la serpiente
void initialize_snake(struct Snake *snake, uint64_t hexcolor) {
    snake->length = 5; // Comienza con 5 segmentos
    //snake->x[0] = get_width_vd() / 2; // Posición inicial en X
    //nake->y[0] = get_height_vd() / 2; // Posición inicial en Y

    snake->x[0] = 320;
    snake->y[0] = 320;

    for (int i = 1; i < snake->length; i++) {
        snake->x[i] = snake->x[i - 1] - THICKNESS;  // Segmentos iniciales hacia la izquierda
        snake->y[i] = snake->y[0];                  // Mantener la misma Y
    }

    snake->direction_X = 1; // Movimiento inicial en X (derecha)
    snake->direction_Y = 0; // No hay movimiento inicial en Y
    snake->is_dead = false;
    snake->color = hexcolor; // Color de la serpiente
    snake->score = 0;
    snake->has_eaten_apple=0;
}

// Dibuja la serpiente
void draw_snake(struct Snake *snake) {
    for (int i = 0; i < snake->length; i++) {
        draw_square(snake->color, snake->x[i], snake->y[i], THICKNESS);
    }
}

//Chequea que la manzana no se vaya a dibujar abajo de la serpiente 
bool overlap(Apple *apple , Snake *snake){
            for (int i = 0; i < snake->length; i++) {
                if (snake->x[i] < apple->x + APPLE_SIZE && snake->x[i] + THICKNESS > apple->x &&
                    snake->y[i] < apple->y + APPLE_SIZE && snake->y[i] + THICKNESS > apple->y) {
                    return true;
                }
            }
        return false;
}

//Dibuja los pixeles del premio
void draw_apple(uint64_t color , uint64_t start_x, uint64_t start_y) {
    draw_square(color, start_x , start_y , APPLE_SIZE);
}

void find_apple(Apple *apple , Snake *snake){
       if (snake->x[0] < apple->x + APPLE_SIZE && snake->x[0] + THICKNESS > apple->x &&
        snake->y[0] < apple->y + APPLE_SIZE && snake->y[0] + THICKNESS > apple->y) {
        
        make_beep(1, 1500);
        draw_apple(BACKGROUND_COLOR, apple->x, apple->y);   //borro la manzana
        
        snake->length++;
        
        snake->score++;

        snake->x[snake->length - 1] = snake->x[snake->length - 2];
        snake->y[snake->length - 1] = snake->y[snake->length - 2];

        snake->has_eaten_apple=1;
        } 
}

//Setea la manzana en la pantalla (gameloop 1)
void set_apple(Apple *apple,  Snake *snake ){
    do {
            apple->x = rand_position_x();
            apple->y = rand_position_y();
    } while (overlap(apple, snake));
        draw_apple(PRIZE_COLOR, apple->x, apple->y);        

}

//Setea la manzana en la pantalla (gameloop 2)
void set_apple_2(Apple *apple,  Snake *snake1, Snake *snake2 ){
    do {
            apple->x = rand_position_x();
            apple->y = rand_position_y();
    } while (overlap(apple, snake1) || overlap(apple, snake2));
        draw_apple(PRIZE_COLOR, apple->x, apple->y);       

}



// Mueve la serpiente
void move_snake(struct Snake *snake) {
    // Borrar la cola de la serpiente antes de moverla
    if (!snake->has_eaten_apple) {  // has_eaten_apple debe ser una bandera que activas al comer una manzana.
        int tail_index = snake->length - 1;
        snake->tail_X = snake->x[tail_index]; // Guarda la posición actual de la cola para borrarla después
        snake->tail_Y = snake->y[tail_index];
        draw_square(BACKGROUND_COLOR, snake->tail_X, snake->tail_Y, THICKNESS);
        draw_lines();
    }

    snake->has_eaten_apple=0;

    for (int i = snake->length-1; i > 0; i--) {
        snake->x[i] = snake->x[i - 1];
        snake->y[i] = snake->y[i - 1];
    }

    // Actualiza la posición de la cabeza
    snake->x[0] += snake->direction_X * THICKNESS;
    snake->y[0] += snake->direction_Y * THICKNESS;


    // Restringir los límites de la serpiente
    if (snake->x[0] < DIM_HEADER) snake->x[0] = DIM_HEADER;
    if (snake->x[0] > DIM_BOARD - THICKNESS) snake->x[0] = DIM_BOARD - THICKNESS;
    if (snake->y[0] < DIM_HEADER) snake->y[0] = DIM_HEADER;
    if (snake->y[0] > DIM_BOARD - THICKNESS) snake->y[0] = DIM_BOARD - THICKNESS;

    // Marcar como muerta si se sale de los límites
    if (snake->x[0] < DIM_HEADER || snake->x[0] >= DIM_BOARD || 
        snake->y[0] < DIM_HEADER || snake->y[0] >= DIM_BOARD) {
        snake->is_dead = true;
    }

    // Verifica colisión con ella misma
    for (int i = 1; i < snake->length; i++) {
        if (snake->x[0] == snake->x[i] && snake->y[0] == snake->y[i]) {
            snake->is_dead = true; // La serpiente se ha muerto
            break;
        }
    }
}

//Analiza las teclas apretadas 
void keyboard_managment_snake (char input, Snake *snake,char K1,char K2,char K3,char K4) {
    if (input==K1) {
        if (snake->direction_Y != 1) { // No permitir el giro en dirección opuesta
            snake->direction_X = 0; // Se mantiene en horizontal
            snake->direction_Y = -1; // Cambiar a dirección vertical
        }
    }
    else if (input==K2) {
        if (snake->direction_X != 1) {
            snake->direction_X = -1; // Cambiar a dirección horizontal a la izquierda
            snake->direction_Y = 0; // Se mantiene en horizontal
        }
    }
    else if (input==K3){
        if (snake->direction_Y != -1) {
            snake->direction_X = 0; // Se mantiene en horizontal
            snake->direction_Y = 1; // Cambiar a dirección vertical hacia abajo
        }
    }
    else if (input==K4){
        if (snake->direction_X != -1) {
            snake->direction_X = 1; // Cambiar a dirección horizontal a la derecha
            snake->direction_Y = 0; // Se mantiene en horizontal
        }
    }
}

void loosing_sound(){
    make_beep(1, 600);
    make_beep(1, 450);
    make_beep(1, 350); 
    make_beep(1, 250);
}

//Finalizacion del juego 
void exit_snake(){

    char matriz[15][15] =   {{0,0,0,0,1,1,1,1,1,0,0,0,0,0,0},
                            {0,0,0,1,2,2,2,2,2,1,0,0,0,0,0},
                            {0,0,0,1,2,2,2,2,2,2,1,0,0,0,0},

                            {0,0,1,2,1,2,2,2,1,2,1,0,0,0,0},
                            {0,0,1,2,1,2,2,2,1,2,2,1,0,0,0},
                            {0,0,1,2,2,2,2,2,2,2,2,1,0,0,0},
                            
                            {0,0,0,1,2,2,3,3,2,2,1,0,0,0,0},
                            {0,0,0,0,1,1,1,1,1,1,1,0,0,0,0},

                            {0,0,1,1,2,2,1,2,2,2,1,1,0,0,0},
                            {0,1,2,2,2,1,2,2,2,2,1,2,1,0,0},
                            {1,2,2,1,2,2,2,2,2,1,2,2,2,1,0},
                            {1,2,2,2,1,1,1,1,1,2,2,2,1,2,1},
                            {1,2,2,2,2,2,2,2,2,2,2,2,1,2,1},
                            {0,1,2,2,2,2,2,2,2,2,2,1,1,2,1},
                            {0,0,1,1,1,1,1,1,1,1,1,0,0,1,0}
                            };

    loosing_sound(); 

    paint_all_vd(BLACK_COLOR);

    for(int i=0; i<15; i++){
    for (int j=0; j<15; j++) {
        if (matriz[i][j] == 0) {
            draw_square(WHITE_COLOR, (get_width_vd()/4) + (j * 35), (get_height_vd()/4) + (i * 35),35);
        }
        else if (matriz[i][j] == 1) {
            draw_square(BLACK_COLOR, (get_width_vd()/4) + (j * 35), (get_height_vd()/4) + (i * 35),35);
        }
        else if (matriz[i][j] == 2) {
            draw_square(SNAKE1_COLOR, (get_width_vd()/4) + (j * 35), (get_height_vd()/4) + (i * 35),35);
        }
        else if (matriz[i][j] == 3) {
            draw_square(SNAKE2_COLOR, (get_width_vd()/4) + (j * 35), (get_height_vd()/4) + (i * 35),35);
        }
    }
}
    set_pixel_size(5);
    print("GAME OVER", 9);
    char input=0;
    print("\n",2);
    set_pixel_size(3);
    print("Press q to quit", 15);
    while(input!='q'){
        input = get_char_user();
        if (input=='q'){
            return;
        }
    }
    return;
}

// Función principal del juego
void game_loop_1() {


    set_seed();

    struct Snake snake1;
    struct Apple apple;

    //Inicializar manzana y snake

    set_apple(&apple, &snake1);

    initialize_snake(&snake1, SNAKE1_COLOR);

    //Seteo del tablero

    paint_all_vd(BLACK_COLOR);

    draw_background();

    draw_lines();

    draw_apple(PRIZE_COLOR, apple.x, apple.y);
           
    //Score

    char *score= "SCORE : 0";
    char buffer[3];

    set_pixel_size(2);
    print(score, 9);
    
    while (!snake1.is_dead && snake1.length < MAX_SNAKE_LENGTH) {

        nano_sleep(2);

        // Dibujar la serpiente
        draw_snake(&snake1);
        
        // Mover la serpiente
        move_snake(&snake1);
        
        // Ver si encuentra la manzana
        find_apple(&apple, &snake1);

        if(snake1.has_eaten_apple){
            set_apple(&apple, &snake1);

            itoa(snake1.score, buffer);
            if(snake1.length > 15){
                errase_char(BLACK_COLOR);
            }
            errase_char(BLACK_COLOR);
            print(buffer, 2);
        }

        // Manejo de entrada
        char input = get_char_user();
        keyboard_managment_snake (input, &snake1, 'w','a','s','d');

    }
    exit_snake(1);
}

void game_loop_2() {

    set_seed();

    struct Snake snake1;
    struct Snake snake2;
    struct Apple apple;

    set_apple_2(&apple, &snake1, &snake2);
    
    initialize_snake(&snake1,SNAKE1_COLOR);
    initialize_snake(&snake2,SNAKE2_COLOR);

    paint_all_vd(BLACK_COLOR);

    draw_background();

    draw_lines();

    draw_apple(PRIZE_COLOR, apple.x, apple.y);
           
    char *score1= "SCORE SNAKE1 : ";
    char *score2= "SCORE SNAKE2 : ";
    char buff1[3];
    char buff2[3];

    set_pixel_size(2);
    print(score1, 9);
    itoa(snake1.score, buff1);
    errase_char(BLACK_COLOR);
    print(buff1, 2);

    print("\n", 2);

    print(score2, 9);
    itoa(snake2.score, buff2);
    errase_char(BLACK_COLOR);
    print(buff2, 2);
    
    while (!snake1.is_dead && !snake2.is_dead && snake1.length < MAX_SNAKE_LENGTH  && snake2.length < MAX_SNAKE_LENGTH) {

        nano_sleep(2);

        // Dibujar las serpientes
        draw_snake(&snake1);
        draw_snake(&snake2);
        
        // Mover las serpientes
        move_snake(&snake1);
        move_snake(&snake2);
        
        // Ver si encuentran la manzana
        find_apple(&apple, &snake1);
        find_apple(&apple, &snake2);


        if(snake1.has_eaten_apple){
            set_apple_2(&apple, &snake1, &snake2);

            errase_line();
            errase_line();

            print(score1, 9);
            itoa(snake1.score, buff1);
            errase_char(BLACK_COLOR);
            print(buff1, 2);

            print("\n", 2);

            print(score2, 9);
            itoa(snake2.score, buff2);
            errase_char(BLACK_COLOR);
            print(buff2, 2);

        }

        else if(snake2.has_eaten_apple){
            set_apple_2(&apple, &snake1, &snake2);

            errase_line();
            errase_line();

            print(score1, 9);
            itoa(snake1.score, buff1);
            errase_char(BLACK_COLOR);
            print(buff1, 2);

            print("\n", 2);

            print(score2, 9);
            itoa(snake2.score, buff2);
            errase_char(BLACK_COLOR);
            print(buff2, 2);

        }

        // Manejo de entrada
        char input = get_char_user();
        keyboard_managment_snake (input, &snake1, 'w','a','s','d');
        keyboard_managment_snake (input, &snake2, 'i','j','k','l');

    }
    exit_snake(2);
}




