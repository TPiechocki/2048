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
int moveAll(block_t blocks[BOARD_SIZE][BOARD_SIZE], int direction) {
    // move position is temp position of block moving because of possibility to move up to 3 blocks
    // temp stores value of block being moved
    int move_position, temp;
    switch (direction) {
        case SDLK_UP:
            // check for every block on board
            for (int i = 0; i < BOARD_SIZE; ++i) {
                for (int j = 0; j < BOARD_SIZE; ++j) {
                    // if block has a value and was not moved during this loop
                    if (blocks[j][i].value != EMPTY && blocks[j][i].moved == 0) {
                        move_position = i;
                        // while nieghbouring block is empty and move stays within the board
                        while (move_position > 0 && blocks[j][move_position - 1].value == EMPTY) {
                            temp = blocks[j][move_position].value;
                            blocks[j][move_position].value = EMPTY;
                            blocks[j][move_position - 1].value = temp;
                            blocks[j][move_position - 1].moved = 1;
                            --move_position;
                        }
                    }
                }
            }
            break;
        // for other direction similar
        case SDLK_DOWN:
            // checking begin from last row of the board, so every tile can move properly
            for (int i = BOARD_SIZE-1; i >=0; --i) {
                for (int j = 0; j < BOARD_SIZE; ++j) {
                    if (blocks[j][i].value != EMPTY && blocks[j][i].moved == 0) {
                        move_position = i;
                        while (move_position < BOARD_SIZE-1 && blocks[j][move_position + 1].value == EMPTY) {
                            temp = blocks[j][move_position].value;
                            blocks[j][move_position].value = EMPTY;
                            blocks[j][move_position + 1].value = temp;
                            blocks[j][move_position + 1].moved = 1;
                            ++move_position;
                        }
                    }
                }
            }
            break;
        case SDLK_LEFT:
            for (int i = 0; i < BOARD_SIZE; ++i) {
                for (int j = 0; j < BOARD_SIZE; ++j) {
                    if (blocks[j][i].value != EMPTY && blocks[j][i].moved == 0) {
                        move_position = j;
                        while (move_position > 0 && blocks[move_position - 1][i].value == EMPTY) {
                            temp = blocks[move_position][i].value;
                            blocks[move_position][i].value = EMPTY;
                            blocks[move_position - 1][i].value = temp;
                            blocks[move_position - 1][i].moved = 1;
                            --move_position;
                        }
                    }
                }
            }
            break;
        case SDLK_RIGHT:
            for (int i = 0; i < BOARD_SIZE; ++i) {
                // checking begin from last cloumn of the board, so every tile can move properly
                for (int j = BOARD_SIZE-1; j >= 0; --j) {
                    if (blocks[j][i].value != EMPTY && blocks[j][i].moved == 0) {
                        move_position = j;
                        while (move_position < BOARD_SIZE-1 && blocks[move_position + 1][i].value == EMPTY) {
                            temp = blocks[move_position][i].value;
                            blocks[move_position][i].value = EMPTY;
                            blocks[move_position + 1][i].value = temp;
                            blocks[move_position + 1][i].moved = 1;
                            ++move_position;
                        }
                    }
                }
            }
            break;
        default:break;

    }
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            blocks[i][j].moved = 0;
        }
    }
    return 0;
}
