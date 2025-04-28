#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <stdio.h>
#include "stdlib_user.h"

// Definiciones de constantes
#define WIDHT 768
#define HEIGHT 1024
#define DIM_BOARD 760               // Tamano del board
#define DIM_HEADER 80
#define THICKNESS 40                // Grosor de la serpiente
#define MAX_SNAKE_LENGTH 90         // Longitud máxima de la serpiente
#define APPLE_SIZE 40               // Tamano manzana

// Colores
#define PRIZE_COLOR 0xA43A53        // Bordo
#define BLACK_COLOR 0x000000        // Negro
#define WHITE_COLOR 0xFFFFFF        // Blanco
#define BACKGROUND_COLOR 0x111111   // Color de fondo
#define SNAKE1_COLOR 0x93DD6F       //0x5434B3       // Verde
#define SNAKE2_COLOR 0xDDAEFF       //0xBE8015       // Rosa


// Estructura de la serpiente
struct Snake {
    int x[MAX_SNAKE_LENGTH];  // Posiciones en X de la serpiente
    int y[MAX_SNAKE_LENGTH];  // Posiciones en Y de la serpiente
    int length;               // Longitud de la serpiente
    int directionX;          // 1: derecha, -1: izquierda, 0: no se mueve
    int directionY;          // 1: abajo, -1: arriba, 0: no se mueve
    bool isDead;             // Estado de la serpiente (muerta o viva)
    uint32_t color;          // Color de la serpiente
    int tailX;
    int tailY;
    int score ; 
    int hasEatenApple;
}typedef Snake ;

struct Apple{
    int x;
    int y;
}typedef Apple;

//Seed global 
uint32_t seed;

//Seteamos la semilla en los segundos;
void setSeed(){
    seed = get_seconds();
}

//Generador de numeros pseoudo-aleatorios
uint32_t randNum() {
    seed = seed * 1664525 + 1013904223;
    return seed;
}

//Dibujo de grillas 
void drawLines(){
    for (int i = DIM_HEADER; i <= DIM_BOARD; i++){
        for (int j = DIM_HEADER; j <= DIM_BOARD; j+=40)
        {
            drawPixel(BLACK_COLOR, i, j);
            drawPixel(BLACK_COLOR, j, i);
        }
    }
}

void drawBackground(){
    for (int i = DIM_HEADER; i <= DIM_BOARD; i++){
        for (int j = DIM_HEADER; j <= DIM_BOARD; j++)
        {
            drawPixel(BACKGROUND_COLOR, i, j);
        }
    }
}

uint32_t randPositionx() {
    int aux1 = (randNum() % DIM_BOARD) + 1;
    if (aux1<DIM_HEADER)
    {
        return ((aux1+DIM_HEADER)/40)*40;
    }
    return (aux1/40)*40;
}
uint32_t randPositiony() {
    int aux2 = (randNum() % DIM_BOARD) + 1;
    if (aux2<DIM_HEADER)
    {
        return ((aux2+DIM_HEADER)/40)*40;
    }
    return (aux2/40)*40;
}


// Función para inicializar la serpiente
void initializeSnake(struct Snake *snake, uint64_t hexcolor) {
    snake->length = 5; // Comienza con 5 segmentos
    //snake->x[0] = getWidth_vd() / 2; // Posición inicial en X
    //nake->y[0] = getHeight_vd() / 2; // Posición inicial en Y

    snake->x[0] = 320;
    snake->y[0] = 320;

    for (int i = 1; i < snake->length; i++) {
        snake->x[i] = snake->x[i - 1] - THICKNESS;  // Segmentos iniciales hacia la izquierda
        snake->y[i] = snake->y[0];                  // Mantener la misma Y
    }

    snake->directionX = 1; // Movimiento inicial en X (derecha)
    snake->directionY = 0; // No hay movimiento inicial en Y
    snake->isDead = false;
    snake->color = hexcolor; // Color de la serpiente
    snake->score = 0;
    snake->hasEatenApple=0;
}

// Dibuja la serpiente
void drawSnake(struct Snake *snake) {
    for (int i = 0; i < snake->length; i++) {
        drawSquare(snake->color, snake->x[i], snake->y[i], THICKNESS);
    }
}

//Chequea que la manzana no se vaya a dibujar abajo de la serpiente 
bool overLap(Apple *apple , Snake *snake){
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
    drawSquare(color, start_x , start_y , APPLE_SIZE);
}

void findApple(Apple *apple , Snake *snake){
       if (snake->x[0] < apple->x + APPLE_SIZE && snake->x[0] + THICKNESS > apple->x &&
        snake->y[0] < apple->y + APPLE_SIZE && snake->y[0] + THICKNESS > apple->y) {
        
        makeBeep(1, 1500);
        draw_apple(BACKGROUND_COLOR, apple->x, apple->y);   //borro la manzana
        
        snake->length++;
        
        snake->score++;

        snake->x[snake->length - 1] = snake->x[snake->length - 2];
        snake->y[snake->length - 1] = snake->y[snake->length - 2];

        snake->hasEatenApple=1;
        } 
}

//Setea la manzana en la pantalla (gameloop 1)
void setApple(Apple *apple,  Snake *snake ){
    do {
            apple->x = randPositionx();
            apple->y = randPositiony();
    } while (overLap(apple, snake));
        draw_apple(PRIZE_COLOR, apple->x, apple->y);        

}

//Setea la manzana en la pantalla (gameloop 2)
void setApple2(Apple *apple,  Snake *snake1, Snake *snake2 ){
    do {
            apple->x = randPositionx();
            apple->y = randPositiony();
    } while (overLap(apple, snake1) || overLap(apple, snake2));
        draw_apple(PRIZE_COLOR, apple->x, apple->y);       

}



// Mueve la serpiente
void moveSnake(struct Snake *snake) {
    // Borrar la cola de la serpiente antes de moverla
    if (!snake->hasEatenApple) {  // hasEatenApple debe ser una bandera que activas al comer una manzana.
        int tailIndex = snake->length - 1;
        snake->tailX = snake->x[tailIndex]; // Guarda la posición actual de la cola para borrarla después
        snake->tailY = snake->y[tailIndex];
        drawSquare(BACKGROUND_COLOR, snake->tailX, snake->tailY, THICKNESS);
        drawLines();
    }

    snake->hasEatenApple=0;

    for (int i = snake->length-1; i > 0; i--) {
        snake->x[i] = snake->x[i - 1];
        snake->y[i] = snake->y[i - 1];
    }

    // Actualiza la posición de la cabeza
    snake->x[0] += snake->directionX * THICKNESS;
    snake->y[0] += snake->directionY * THICKNESS;


    // Restringir los límites de la serpiente
    if (snake->x[0] < DIM_HEADER) snake->x[0] = DIM_HEADER;
    if (snake->x[0] > DIM_BOARD - THICKNESS) snake->x[0] = DIM_BOARD - THICKNESS;
    if (snake->y[0] < DIM_HEADER) snake->y[0] = DIM_HEADER;
    if (snake->y[0] > DIM_BOARD - THICKNESS) snake->y[0] = DIM_BOARD - THICKNESS;

    // Marcar como muerta si se sale de los límites
    if (snake->x[0] < DIM_HEADER || snake->x[0] >= DIM_BOARD || 
        snake->y[0] < DIM_HEADER || snake->y[0] >= DIM_BOARD) {
        snake->isDead = true;
    }

    // Verifica colisión con ella misma
    for (int i = 1; i < snake->length; i++) {
        if (snake->x[0] == snake->x[i] && snake->y[0] == snake->y[i]) {
            snake->isDead = true; // La serpiente se ha muerto
            break;
        }
    }
}

//Analiza las teclas apretadas 
void keyboard_managment_snake (char input, Snake *snake,char K1,char K2,char K3,char K4) {
    if (input==K1) {
        if (snake->directionY != 1) { // No permitir el giro en dirección opuesta
            snake->directionX = 0; // Se mantiene en horizontal
            snake->directionY = -1; // Cambiar a dirección vertical
        }
    }
    else if (input==K2) {
        if (snake->directionX != 1) {
            snake->directionX = -1; // Cambiar a dirección horizontal a la izquierda
            snake->directionY = 0; // Se mantiene en horizontal
        }
    }
    else if (input==K3){
        if (snake->directionY != -1) {
            snake->directionX = 0; // Se mantiene en horizontal
            snake->directionY = 1; // Cambiar a dirección vertical hacia abajo
        }
    }
    else if (input==K4){
        if (snake->directionX != -1) {
            snake->directionX = 1; // Cambiar a dirección horizontal a la derecha
            snake->directionY = 0; // Se mantiene en horizontal
        }
    }
}

void loosingSound(){
    makeBeep(1, 600);
    makeBeep(1, 450);
    makeBeep(1, 350); 
    makeBeep(1, 250);
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

    loosingSound(); 

    paintAll_vd(BLACK_COLOR);

    for(int i=0; i<15; i++){
    for (int j=0; j<15; j++) {
        if (matriz[i][j] == 0) {
            drawSquare(WHITE_COLOR, (getWidth_vd()/4) + (j * 35), (getHeight_vd()/4) + (i * 35),35);
        }
        else if (matriz[i][j] == 1) {
            drawSquare(BLACK_COLOR, (getWidth_vd()/4) + (j * 35), (getHeight_vd()/4) + (i * 35),35);
        }
        else if (matriz[i][j] == 2) {
            drawSquare(SNAKE1_COLOR, (getWidth_vd()/4) + (j * 35), (getHeight_vd()/4) + (i * 35),35);
        }
        else if (matriz[i][j] == 3) {
            drawSquare(SNAKE2_COLOR, (getWidth_vd()/4) + (j * 35), (getHeight_vd()/4) + (i * 35),35);
        }
    }
}
    setPixelSize(5);
    print("GAME OVER", 9);
    char input=0;
    print("\n",2);
    setPixelSize(3);
    print("Press q to quit", 15);
    while(input!='q'){
        input = getCharUser();
        if (input=='q'){
            return;
        }
    }
    return;
}

// Función principal del juego
void gameLoop1() {


    setSeed();

    struct Snake snake1;
    struct Apple apple;

    //Inicializar manzana y snake

    setApple(&apple, &snake1);

    initializeSnake(&snake1, SNAKE1_COLOR);

    //Seteo del tablero

    paintAll_vd(BLACK_COLOR);

    drawBackground();

    drawLines();

    draw_apple(PRIZE_COLOR, apple.x, apple.y);
           
    //Score

    char *score= "SCORE : 0";
    char buffer[3];

    setPixelSize(2);
    print(score, 9);
    
    while (!snake1.isDead && snake1.length < MAX_SNAKE_LENGTH) {

        nano_sleep(2);

        // Dibujar la serpiente
        drawSnake(&snake1);
        
        // Mover la serpiente
        moveSnake(&snake1);
        
        // Ver si encuentra la manzana
        findApple(&apple, &snake1);

        if(snake1.hasEatenApple){
            setApple(&apple, &snake1);

            itoa(snake1.score, buffer);
            if(snake1.length > 15){
                erraseChar(BLACK_COLOR);
            }
            erraseChar(BLACK_COLOR);
            print(buffer, 2);
        }

        // Manejo de entrada
        char input = getCharUser();
        keyboard_managment_snake (input, &snake1, 'w','a','s','d');

    }
    exit_snake(1);
}

void gameLoop2() {

    setSeed();

    struct Snake snake1;
    struct Snake snake2;
    struct Apple apple;

    setApple2(&apple, &snake1, &snake2);
    
    initializeSnake(&snake1,SNAKE1_COLOR);
    initializeSnake(&snake2,SNAKE2_COLOR);

    paintAll_vd(BLACK_COLOR);

    drawBackground();

    drawLines();

    draw_apple(PRIZE_COLOR, apple.x, apple.y);
           
    char *score1= "SCORE SNAKE1 : ";
    char *score2= "SCORE SNAKE2 : ";
    char buff1[3];
    char buff2[3];

    setPixelSize(2);
    print(score1, 9);
    itoa(snake1.score, buff1);
    erraseChar(BLACK_COLOR);
    print(buff1, 2);

    print("\n", 2);

    print(score2, 9);
    itoa(snake2.score, buff2);
    erraseChar(BLACK_COLOR);
    print(buff2, 2);
    
    while (!snake1.isDead && !snake2.isDead && snake1.length < MAX_SNAKE_LENGTH  && snake2.length < MAX_SNAKE_LENGTH) {

        nano_sleep(2);

        // Dibujar las serpientes
        drawSnake(&snake1);
        drawSnake(&snake2);
        
        // Mover las serpientes
        moveSnake(&snake1);
        moveSnake(&snake2);
        
        // Ver si encuentran la manzana
        findApple(&apple, &snake1);
        findApple(&apple, &snake2);


        if(snake1.hasEatenApple){
            setApple2(&apple, &snake1, &snake2);

            erraseLine();
            erraseLine();

            print(score1, 9);
            itoa(snake1.score, buff1);
            erraseChar(BLACK_COLOR);
            print(buff1, 2);

            print("\n", 2);

            print(score2, 9);
            itoa(snake2.score, buff2);
            erraseChar(BLACK_COLOR);
            print(buff2, 2);

        }

        else if(snake2.hasEatenApple){
            setApple2(&apple, &snake1, &snake2);

            erraseLine();
            erraseLine();

            print(score1, 9);
            itoa(snake1.score, buff1);
            erraseChar(BLACK_COLOR);
            print(buff1, 2);

            print("\n", 2);

            print(score2, 9);
            itoa(snake2.score, buff2);
            erraseChar(BLACK_COLOR);
            print(buff2, 2);

        }

        // Manejo de entrada
        char input = getCharUser();
        keyboard_managment_snake (input, &snake1, 'w','a','s','d');
        keyboard_managment_snake (input, &snake2, 'i','j','k','l');

    }
    exit_snake(2);
}




