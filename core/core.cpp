//
// Created by Tomasz Piechocki on 26/12/2018.
//

#include"core.h"
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

    return 0;
}
