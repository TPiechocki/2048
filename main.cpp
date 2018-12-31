#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_WARNINGS
#include<math.h>
#include<stdio.h>
#include<string.h>
#include<time.h>
#include"core/display.h"

void defaultSettings(game_t *game_status);

// main
#ifdef __cplusplus
extern "C"
#endif
int main(int argc, char *argv[]) {
    int t1, t2, quit, frames;
    double delta, fpsTimer, fps;
    srand((time_t)time(NULL));
    game_t game_status;
    SDL_Event event;
    SDL_Surface *screen = NULL, *charset = NULL;
    SDL_Surface *eti = NULL;
    SDL_Texture *scrtex = NULL;
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    defaultSettings(&game_status);

    printf("printf output goes here\n");

    if(InitAll(&window, &renderer))
        return 1;

    screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
                                  0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

    scrtex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                               SDL_TEXTUREACCESS_STREAMING,
                               SCREEN_WIDTH, SCREEN_HEIGHT);


    charset = SDL_LoadBMP("./cs8x8.bmp");
    if(charset == NULL) {
        printf("SDL_LoadBMP(cs8x8.bmp) error: %s\n", SDL_GetError());
        SDL_FreeSurface(screen);
        SDL_DestroyTexture(scrtex);
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
        return 1;
    };
    SDL_SetColorKey(charset, true, 0xFFFFFF);

    t1 = SDL_GetTicks();

    frames = 0;
    fpsTimer = 0;
    fps = 0;
    quit = 0;

    while(!quit) {
        t2 = SDL_GetTicks();

        // here t2-t1 is the time in milliseconds since
        // the last screen was drawn
        // delta is the same time in seconds
        delta = (t2 - t1) * 0.001;
        t1 = t2;

        game_status.timer += delta;

        SDL_FillRect(screen, NULL, colour(screen, (char *)"background"));

        /*DrawSurface(screen, eti,
                    (int)(GAME_WIDTH / 2 + sin(distance) * SCREEN_HEIGHT / 3),
                    (int)(SCREEN_HEIGHT / 2 + cos(distance) * SCREEN_HEIGHT / 3));
        */
        fpsTimer += delta;
        if(fpsTimer > 0.5) {
            fps = frames * 2;
            frames = 0;
            fpsTimer -= 0.5;
        };

        // info text
        DrawLine(screen, GAME_WIDTH, 0, SCREEN_HEIGHT, 0, 1, colour(screen, (char *)"border"));
        DrawLegend(screen, charset, fps, game_status.timer, game_status.points);

        DrawBoard(screen, charset, game_status);

        SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
		SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, scrtex, NULL, NULL);
        SDL_RenderPresent(renderer);

        // handling of events (if there were any)
        while(SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_KEYDOWN: {
                    switch (event.key.keysym.sym) {
                        case SDLK_ESCAPE:   // quit game
                            quit = 1;
                            break;
                        case SDLK_n:        // new game
                            defaultSettings(&game_status);
                            break;
                        case SDLK_UP:       // arrows for moves
                        case SDLK_DOWN:
                        case SDLK_LEFT:
                        case SDLK_RIGHT:
                            if (moveAll(game_status.blocks, event.key.keysym.sym, &game_status.points)) {
                                collapseAll(game_status.blocks, event.key.keysym.sym, &game_status.points);
                                randomOne(game_status.blocks);
                            }
                            break;
                        default: break;
                    }
                    break;
                }
                case SDL_KEYUP:
                    break;
                case SDL_QUIT:
                    quit = 1;
                    break;
                default: break;
            };
        };
        frames++;
    };

    // freeing all surfaces
    SDL_FreeSurface(charset);
    SDL_FreeSurface(screen);
    SDL_DestroyTexture(scrtex);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;
};

void defaultSettings(game_t *game_status) {
    game_status->timer = 0;
    game_status->points = 0;
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            game_status->blocks[i][j].value = EMPTY;
            game_status->blocks[i][j].moved = 0;
        }
    }
    //randomOne(game_status->blocks);

    game_status->blocks[0][0].value = 16;
    game_status->blocks[0][1].value = 8;
    game_status->blocks[0][2].value = 4;
    game_status->blocks[0][3].value = 4;

    /*game_status->blocks[0][0].value = 2;
    game_status->blocks[1][0].value = 4;
    game_status->blocks[2][0].value = 8;
    game_status->blocks[3][0].value = 16;
    game_status->blocks[0][1].value = 32;
    game_status->blocks[1][1].value = 64;
    game_status->blocks[2][1].value = 128;
    game_status->blocks[3][1].value = 256;
    game_status->blocks[0][2].value = 512;
    game_status->blocks[1][2].value = 1024;
    game_status->blocks[2][2].value = 2048;*/
}
