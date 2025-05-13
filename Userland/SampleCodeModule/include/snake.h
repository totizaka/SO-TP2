#ifndef SNAKE_H
#define SNAKE_H

#include <stdint.h>
#include <stdbool.h>

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
    int direction_X;          // 1: derecha, -1: izquierda, 0: no se mueve
    int direction_Y;          // 1: abajo, -1: arriba, 0: no se mueve
    bool is_dead;             // Estado de la serpiente (muerta o viva)
    uint32_t color;          // Color de la serpiente
    int tail_X;
    int tail_Y;
    int score ; 
    int has_eaten_apple;
}typedef Snake ;

struct Apple{
    int x;
    int y;
}typedef Apple;


// Funciones
void set_seed();
uint32_t rand_num();
void draw_lines();
void draw_background();
uint32_t rand_position_x();
uint32_t rand_position_y();
void initialize_snake(struct Snake *snake, uint64_t hexcolor);
void draw_snake(struct Snake *snake);
bool overlap(struct Apple *apple, struct Snake *snake);
void find_apple(struct Apple *apple, struct Snake *snake);
void set_apple(struct Apple *apple, struct Snake *snake);
void set_apple_2(struct Apple *apple, struct Snake *snake1, struct Snake *snake2);
void draw_apple(uint64_t color, uint64_t start_x, uint64_t start_y);
void move_snake(struct Snake *snake);
void keyboard_managment_snake(char input, struct Snake *snake, char K1, char K2, char K3, char K4);
void loosing_sound();
void exit_snake();
void game_loop_1();
void game_loop_2();


#endif // SNAKE_H