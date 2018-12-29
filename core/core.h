//
// Created by Tomasz Piechocki on 26/12/2018.
//

#ifndef INC_2048_CORE_H
#define INC_2048_CORE_H
#ifdef __cplusplus
extern "C" {
#endif
#include"../sdl-2.0.7/include/SDL.h"
#include"../sdl-2.0.7/include/SDL_main.h"

#define SCREEN_WIDTH	640
#define SCREEN_HEIGHT	480
#define GAME_WIDTH (SCREEN_WIDTH*2/3)
#define LEGEND_WIDTH (SCREEN_WIDTH*1/3)
#define BOARD_SIZE 4
#define EMPTY 0

typedef struct {
    int value;
    int moved;      // 1 if this tile was moved already in this move, 2 if not
}block_t;

typedef struct {
    double timer;
    block_t blocks[BOARD_SIZE][BOARD_SIZE];
}game_t;

// initialise SDL, create windows etc.
int InitAll(SDL_Window **window, SDL_Renderer **renderer);

// move all blocks which can be moved in the direction given by the arrow
int moveAll(block_t blocks[BOARD_SIZE][BOARD_SIZE], int direction);

// return colour from given name
// in main called with "colour(screen, (char *)colour name)"
// possible colours: black, green, red, blue, border, background, emptyblock
// and colours for every possible block 2, 4, 8, ..., 2048
uint32_t colour(SDL_Surface *screen, char *name);

#ifdef __cplusplus
}
#endif
#endif //INC_2048_CORE_H
