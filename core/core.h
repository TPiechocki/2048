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

// initialise SDL, create windows etc.
int InitAll(SDL_Window **window, SDL_Renderer **renderer);

// return colour from given name
// in main called with "colour(screen, (char *)colour name)"
// possible colours: black, green, red, blue
uint32_t colour(SDL_Surface *screen, char *name);

#ifdef __cplusplus
}
#endif
#endif //INC_2048_CORE_H
