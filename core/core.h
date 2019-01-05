//
// Created by Tomasz Piechocki on 26/12/2018.
//

#ifndef INC_2048_CORE_H
#define INC_2048_CORE_H
#ifdef __cplusplus
extern "C" {
#endif
#include"../SDL-2.0.7/include/SDL.h"
#include"../SDL-2.0.7/include/SDL_main.h"

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
    int points;
    block_t blocks[BOARD_SIZE][BOARD_SIZE];
}game_t;

/**
 * Initialize SDL and create window
 * @param window
 * @param renderer
 * @return 1 on error, normally 0
 */
int InitAll(SDL_Window **window, SDL_Renderer **renderer);

/**
 * return RGBMap of colour of given name
 * in main called with "colour(screen, (char *)colour name)"
 * @param screen
 * @param name - colour name; possible
 * possible colours: black, green, red, blue, border, background, emptyblock
 * and colours for every possible block 2, 4, 8, ..., 2048
 * @return RGBMap of colour or 0 if colour name was not found
 */
uint32_t colour(SDL_Surface *screen, char *name);

/**
 * move all blocks which can be moved in the direction given by the arrow
 * @param blocks - actual board
 * @param direction - move direction
 * @return 1 if anything was moved, 0 if nothing moved
 */
int moveAll(block_t blocks[BOARD_SIZE][BOARD_SIZE], int direction);

/**
 * Merge all block neighbouring in right direction
 * @param blocks - actual board
 * @param direction
 * @param points - player score
 * @return 1 if anything was mergd, 0 if nothing changed
 */
int mergeAll(block_t blocks[BOARD_SIZE][BOARD_SIZE], int direction, int *points);

/**
 * Put one random block on the board.
 * "2" with 95% possibility or "4" with 5% possibility
 * @param blocks - actual board
 */
void randomOne(block_t blocks[BOARD_SIZE][BOARD_SIZE]);

#ifdef __cplusplus
}
#endif
#endif //INC_2048_CORE_H
