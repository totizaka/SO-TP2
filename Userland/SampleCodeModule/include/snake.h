#ifndef SNAKE_H
#define SNAKE_H

#include <stdint.h>
#include <stdbool.h>

// Definiciones de constantes
#define WIDHT 768
#define HEIGHT 1024
#define DIM_BOARD 760               // Tamaño del board
#define DIM_HEADER 80
#define THICKNESS 40                // Grosor de la serpiente
#define MAX_SNAKE_LENGTH 100        // Longitud máxima de la serpiente
#define APPLE_SIZE 40               // Tamaño de la manzana

// Colores
#define PRIZE_COLOR 0xA43A53        // Bordo
#define BLACK_COLOR 0x000000        // Negro
#define BACKGROUND_COLOR 0x111111   // Color de fondo
#define SNAKE1_COLOR 0xddaeff       // Violeta
#define SNAKE2_COLOR 0x93dd6f       // Naranja

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
    int score; 
    int hasEatenApple;
};

// Estructura de la manzana
struct Apple {
    int x;
    int y;
};

// Seed global 
extern uint32_t seed;

// Funciones
void setSeed();
uint32_t randNum();
void drawLines();
void drawBackground();
uint32_t randPositionx();
uint32_t randPositiony();
void initializeSnake(struct Snake *snake, uint64_t hexcolor);
void drawSnake(struct Snake *snake);
bool overLap(struct Apple *apple, struct Snake *snake);
void findApple(struct Apple *apple, struct Snake *snake);
void setApple(struct Apple *apple, struct Snake *snake);
void setApple2(struct Apple *apple, struct Snake *snake1, struct Snake *snake2);
void draw_apple(uint64_t color, uint64_t start_x, uint64_t start_y);
void moveSnake(struct Snake *snake);
void keyboard_managment_snake(char input, struct Snake *snake, char K1, char K2, char K3, char K4);
void loosingSound();
void exit_snake();
void gameLoop1();
void gameLoop2();


#endif // SNAKE_H