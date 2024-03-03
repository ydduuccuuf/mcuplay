#ifndef _TIME_H_
#define _TIME_H_

#include <stdint.h>

struct snake {  // ?????
    int length;          // ??
    char snakecolor;      // ??
    uint16_t snakeX[100];     // ???X??
    uint16_t snakeY[100];     // ???Y??
    int headX;           // ???X??
    int headY;           // ???Y??
    int dir;             // ??
    int tpdir;           // ????
    int life;            // ??
};

extern struct snake mySnake;

struct food {  // ??????
    uint16_t foodX;
    uint16_t foodY;
    uint16_t foodcolor;
};

typedef enum {
    UP,
    DOWN,
    LEFT,
    RIGHT
} Direction;

extern Direction snakeDirection;  // ????

void TIM2_Configuration(void);

#endif