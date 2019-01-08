//
// Created by Tomasz Piechocki on 26/12/2018.
//

#include"core.h"
#include "core.h"

#include<string.h>
#include<stdio.h>

#ifdef __GNUC__
#include<stdlib.h>
#endif
#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

/**
 * Move one block of x,y coordinates
 * @param blocks - actual board
 * @param x - x coordinate of block
 * @param y -  y coordinate of block
 * @param horizontal - -1 for left; +1 for right
 * @param vertical - -1 for up; +1 for down
 * @return -1 for error, 0 if no move, 1 block was moved
 */
int moveOne(game_t *game, int x, int y, int horizontal, int vertical) {
    if (horizontal != 0 && vertical != 0) {
        printf("Move can be only in one dimension");
        return -1;
    }
    if (horizontal > 1 || horizontal < -1 || vertical > 1 || vertical < -1) {
        printf("Move can only have value 1 or -1");
        return -1;
    }
    int actual_x = x, actual_y = y, temp, status = 0;
    // while neighbouring block is empty and move stays within the board
    while (actual_x + horizontal >= 0 && actual_y + vertical >= 0 &&
           actual_x + horizontal < game->board_size && actual_y + vertical < game->board_size &&
           game->blocks[actual_x + horizontal][actual_y + vertical].value == EMPTY) {
        temp = game->blocks[actual_x][actual_y].value;
        game->blocks[actual_x][actual_y].value = EMPTY;
        game->blocks[actual_x][actual_y].moved = 0;
        game->blocks[actual_x + horizontal][actual_y + vertical].value = temp;
        game->blocks[actual_x + horizontal][actual_y + vertical].moved = 1;
        actual_x += horizontal;
        actual_y += vertical;
        status = 1;

        // for animation purpose
        ++game->previous[x][y].move_length;
    }
    return status;
}

/**
 * Check block and if possible merge it with neighbouring one
 * @param blocks - actual board
 * @param x - x coordinate of block
 * @param y -  y coordinate of block
 * @param horizontal - -1 for left; +1 for right
 * @param vertical - -1 for up; +1 for down
 * @param points - player's score
 * @return -1 for error, 0 if no change on board, 1 if block was merged
 */
int mergeOne(game_t *game, int x, int y, int horizontal, int vertical) {
    if (horizontal != 0 && vertical != 0) {
        printf("Move can be only in one dimension");
        return -1;
    }
    if (horizontal > 1 || horizontal < -1 || vertical > 1 || vertical < -1) {
        printf("Move can only have value 1 or -1");
        return -1;
    }

    if (x + horizontal < 0 || y + vertical < 0 ||
        x + horizontal >= game->board_size || y + vertical >= game->board_size)
        return 0;
    // if block next to moved is the same than change both to one with 2 times higher value
    if (game->blocks[x + horizontal][y + vertical].value == game->blocks[x][y].value) {
        game->blocks[x][y].value = EMPTY;
        game->blocks[x + horizontal][y + vertical].value *= 2;
        game->points += game->blocks[x + horizontal][y + vertical].value;
        return  1;
    }

    return 0;
}

EXTERNC
int InitAll(SDL_Window **window, SDL_Renderer **renderer) {
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("SDL_Init error: %s\n", SDL_GetError());
        return 1;
    }
    // fullscreen mode
//	rc = SDL_CreateWindowAndRenderer(0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP,
//	                                 &window, &renderer);
    int check = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0,
                                     window, renderer);
    if(check != 0) {
        SDL_Quit();
        printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
        return 1;
    };
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(*renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_SetRenderDrawColor(*renderer, 255, 0, 0, 255);
    SDL_RenderClear(*renderer);

    SDL_SetWindowTitle(*window, "Tomasz Piechocki, 175690");

    SDL_ShowCursor(SDL_DISABLE);

    return 0;
}

EXTERNC
uint32_t colour(SDL_Surface *screen, char *name) {
    if (strcmp(name, "black") == 0)
        return SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
    if (strcmp(name, "green") == 0)
        return SDL_MapRGB(screen->format, 0x00, 0xFF, 0x00);
    if (strcmp(name, "red") == 0)
        return SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00);
    if (strcmp(name, "blue") == 0)
        return SDL_MapRGB(screen->format, 0x11, 0x11, 0xCC);
    if (strcmp(name, "white") == 0)
        return SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF);
    if (strcmp(name, "background") == 0)
        return SDL_MapRGB(screen->format, 0xA0, 0xA0, 0xA0);
    if (strcmp(name, "border") == 0)
        return SDL_MapRGB(screen->format, 0x60, 0x60, 0x60);
    if (strcmp(name, "emptyblock") == 0)
        return SDL_MapRGB(screen->format, 0xCC, 0xCC, 0xCC);
    if (strcmp(name, "2") == 0)
        return SDL_MapRGB(screen->format, 0xEE, 0xE4, 0xDA);
    if (strcmp(name, "4") == 0)
        return SDL_MapRGB(screen->format, 0xED, 0xE0, 0xC8);
    if (strcmp(name, "8") == 0)
        return SDL_MapRGB(screen->format, 0xF2, 0xB1, 0x79);
    if (strcmp(name, "16") == 0)
        return SDL_MapRGB(screen->format, 0xF5, 0x95, 0x63);
    if (strcmp(name, "32") == 0)
        return SDL_MapRGB(screen->format, 0xF9, 0x7C, 0x5F);
    if (strcmp(name, "64") == 0)
        return SDL_MapRGB(screen->format, 0xF6, 0x5E, 0x3B);
    if (strcmp(name, "128") == 0)
        return SDL_MapRGB(screen->format, 0xED, 0xCF, 0x72);
    if (strcmp(name, "256") == 0)
        return SDL_MapRGB(screen->format, 0xED, 0xCC, 0x61);
    if (strcmp(name, "512") == 0)
        return SDL_MapRGB(screen->format, 0xED, 0xC8, 0x50);
    if (strcmp(name, "1024") == 0)
        return SDL_MapRGB(screen->format, 0xED, 0xC5, 0x2F);
    if (strcmp(name, "2048") == 0)
        return SDL_MapRGB(screen->format, 0xED, 0xC2, 0x2E);
    return 0;
}

EXTERNC
int moveAll(game_t *game, int direction) {
    // reset animation info
    for (int i = 0; i < game->board_size; ++i) {
        for (int j = 0; j < game->board_size; ++j) {
            game->previous[i][j].move_length = 0;
        }
    }
    // move position is temp position of block moving because of possibility to move up to 3 blocks
    // temp stores value of block being moved, status is 0 when nothing was moved
    int move_position, temp, status = 0;
    switch (direction) {
        case SDLK_UP:
            // check for every block on board
            for (int i = 0; i < game->board_size; ++i) {
                for (int j = 0; j < game->board_size; ++j) {
                    // if block has a value and was not moved during this loop
                    if (game->blocks[j][i].value != EMPTY && game->blocks[j][i].moved == 0) {
                        if (moveOne(game, j, i, 0, -1) == 1)
                            status += 1;
                    }
                }
            }
            break;
        // for other direction similar
        case SDLK_DOWN:
            // checking begin from last row of the board, so every tile can move properly
            for (int i = game->board_size-1; i >=0; --i) {
                for (int j = 0; j < game->board_size; ++j) {
                    if (game->blocks[j][i].value != EMPTY && game->blocks[j][i].moved == 0) {
                        if (moveOne(game, j, i, 0, 1) == 1)
                            status += 1;
                    }
                }
            }
            break;
        case SDLK_LEFT:
            for (int i = 0; i < game->board_size; ++i) {
                for (int j = 0; j < game->board_size; ++j) {
                    if (game->blocks[j][i].value != EMPTY && game->blocks[j][i].moved == 0) {
                        if (moveOne(game, j, i, -1, 0) == 1)
                            status += 1;
                    }
                }
            }
            break;
        case SDLK_RIGHT:
            for (int i = 0; i < game->board_size; ++i) {
                // checking begin from last column of the board, so every tile can move properly
                for (int j = game->board_size-1; j >= 0; --j) {
                    if (game->blocks[j][i].value != EMPTY && game->blocks[j][i].moved == 0) {
                        if (moveOne(game, j, i, 1, 0) == 1)
                            status += 1;
                    }
                }
            }
            break;
        default:break;

    }
    for (int i = 0; i < game->board_size; ++i) {
        for (int j = 0; j < game->board_size; ++j) {
            game->blocks[i][j].moved = 0;
        }
    }
    return status;
}

EXTERNC
int mergeAll(game_t *game, int direction) {
    // status is 0 when nothing was moved
    int status = 0;
    switch (direction) {
        case SDLK_UP:
            // check for every block on board
            for (int i = 0; i < game->board_size; ++i) {
                for (int j = 0; j < game->board_size; ++j) {
                    // if block has a value and was not moved during this loop
                    if (game->blocks[j][i].value != EMPTY) {
                        if (mergeOne(game, j, i, 0, -1) == 1) {
                            moveAll(game, direction);
                            status += 1;
                        }
                    }
                }
            }
            break;
            // for other direction similar
        case SDLK_DOWN:
            // checking begin from last row of the board, so every tile can move properly
            for (int i = game->board_size-1; i >=0; --i) {
                for (int j = 0; j < game->board_size; ++j) {
                    if (game->blocks[j][i].value != EMPTY) {
                        if (mergeOne(game, j, i, 0, 1) == 1) {
                            moveAll(game, direction);
                            status += 1;
                        }
                    }
                }
            }
            break;
        case SDLK_LEFT:
            for (int i = 0; i < game->board_size; ++i) {
                for (int j = 0; j < game->board_size; ++j) {
                    if (game->blocks[j][i].value != EMPTY) {
                        if (mergeOne(game, j, i, -1, 0) == 1) {
                            moveAll(game, direction);
                            status += 1;
                        }

                    }
                }
            }
            break;
        case SDLK_RIGHT:
            for (int i = 0; i < game->board_size; ++i) {
                // checking begin from last column of the board, so every tile can move properly
                for (int j = game->board_size-1; j >= 0; --j) {
                    if (game->blocks[j][i].value != EMPTY) {
                        if (mergeOne(game, j, i, 1, 0) == 1) {
                            moveAll(game, direction);
                            status += 1;
                        }
                    }
                }
            }
            break;
        default:break;

    }

    return status;
}

EXTERNC
void randomOne(game_t *game) {
    int empty = 0, value;

    // pick 2 or 4 (4 with 5% possibility)
    int random = rand() % 20;
    if (random == 0)
        value = 4;
    else
        value = 2;

    for (int i = 0; i < game->board_size; ++i) {
        for (int j = 0; j < game->board_size; ++j) {
            if (game->blocks[i][j].value == EMPTY)
                empty++;
        }
    }
    int position = rand() % empty, count = 0;
    for (int i = 0; i < game->board_size; ++i) {
        for (int j = 0; j < game->board_size; ++j) {
            if (position == count && game->blocks[i][j].value == EMPTY) {
                game->blocks[i][j].value = value;
                return;
            }
            if (game->blocks[i][j].value == EMPTY)
                count++;
        }
    }
}

EXTERNC
int isEnd(game_t game) {
    int empty = 0;

    // look for 2048 block or empty block
    for (int i = 0; i < game.board_size; ++i) {
        for (int j = 0; j < game.board_size; ++j) {
            if (game.blocks[i][j].value == 2048)
                return 1;   // win if any of the blocks is 2048
            if (game.blocks[i][j].value == EMPTY)
                empty = 1;
        }
    }
    if (empty == 1)     // continue if there's at least one empty
        return 0;

    // look for two same blocks next to each other
    for (int i = 0; i < game.board_size; ++i) {
        for (int j = 0; j < game.board_size; ++j) {
            if (i < game.board_size-1)
                if (game.blocks[i][j].value == game.blocks[i+1][j].value)
                    return 0;

            if (j < game.board_size-1)
                if (game.blocks[i][j].value == game.blocks[i][j+1].value)
                    return 0;
        }
    }

    return 2;   // no win and no possible move
}

EXTERNC
void copyBoard(block_t **source, block_t **destination, int size) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            destination[i][j].value = source[i][j].value;
        }

    }
}

