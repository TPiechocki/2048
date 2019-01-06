#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_WARNINGS
#include<math.h>
#include<stdio.h>
#include<string.h>
#include<time.h>
#include"core/display.h"

void defaultSettings(game_t *game_status, int *quit, SDL_Surface *screen, SDL_Texture *scrtex, SDL_Renderer *renderer,
                     SDL_Surface *charset);

// main
#ifdef __cplusplus
extern "C"
#endif
int main(int argc, char *argv[]) {

    int t1, t2, quit, frames, size = 0;
    double delta, fpsTimer, fps, moveStatus;
    srand((unsigned int)time(NULL));
    game_t game_status;
    game_status.blocks = NULL;
    game_status.previous = NULL;
    SDL_Event event;
    SDL_Surface *screen = NULL, *charset = NULL;
    SDL_Texture *scrtex = NULL;
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    if(InitAll(&window, &renderer))
        return 1;

    screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
                                  0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

    scrtex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                               SDL_TEXTUREACCESS_STREAMING,
                               SCREEN_WIDTH, SCREEN_HEIGHT);

    SDL_FillRect(screen, NULL, colour(screen, (char *)"background"));

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

    prompt(screen, charset, (char *)"Set board size: 3-9");
    updateScreen(screen, scrtex, renderer);

    defaultSettings(&game_status, &quit, screen, scrtex, renderer, charset);

    while(!quit) {
        t2 = SDL_GetTicks();

        // here t2-t1 is the time in milliseconds since
        // the last screen was drawn
        // delta is the same time in seconds
        delta = (t2 - t1) * 0.001;
        t1 = t2;

        game_status.timer += delta;

        SDL_FillRect(screen, NULL, colour(screen, (char *)"background"));

        fpsTimer += delta;
        if(fpsTimer > 0.5) {
            fps = frames * 2;
            frames = 0;
            fpsTimer -= 0.5;
        };

        // info text
        DrawLine(screen, GAME_WIDTH, 0, SCREEN_HEIGHT, 0, 1, colour(screen, (char *)"border"));
        DrawLegend(screen, charset, fps, game_status);

        DrawBoard(screen, charset, game_status);

        if (isEnd(game_status) == 1)
            prompt(screen, charset, (char *)"Congatulations");
        if (isEnd(game_status) == 2)
            prompt(screen, charset, (char *)"No possible move");


        updateScreen(screen, scrtex, renderer);

        // handling of events (if there were any)
        while(SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_KEYDOWN: {
                    switch (event.key.keysym.sym) {
                        case SDLK_ESCAPE:   // quit game
                            quit = 1;
                            break;
                        case SDLK_n:        // new game
                            prompt(screen, charset, (char *)"Set board size: 3-9");
                            updateScreen(screen, scrtex, renderer);
                            defaultSettings(&game_status, &quit, screen, scrtex, renderer, charset);
                            break;
                        case SDLK_UP:       // arrows for moves
                        case SDLK_DOWN:
                        case SDLK_LEFT:
                        case SDLK_RIGHT:
                            moveStatus = moveAll(&game_status, event.key.keysym.sym);
                            moveStatus += mergeAll(&game_status, event.key.keysym.sym);
                            if (moveStatus) {
                                randomOne(&game_status);
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

    // freeing all surfaces and allocated memory
    SDL_FreeSurface(charset);
    SDL_FreeSurface(screen);
    SDL_DestroyTexture(scrtex);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    if (game_status.blocks != NULL) {
        for (int i = 0; i < size; ++i) {
            free(game_status.blocks[i]);
        }
        free(game_status.blocks);
    }
    if (game_status.previous != NULL) {
        for (int i = 0; i < size; ++i) {
            free(game_status.previous[i]);
        }
        free(game_status.previous);
    }

    SDL_Quit();
    return 0;
};

void defaultSettings(game_t *game_status, int *quit, SDL_Surface *screen, SDL_Texture *scrtex, SDL_Renderer *renderer,
                     SDL_Surface *charset) {
    SDL_Event event;
    int size = 0;
    while ((size < 3 || size > 9)) {
        while(SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_ESCAPE) {
                        *quit = 1;
                        return;
                    }
                    else if (event.key.keysym.sym >= SDLK_3 && event.key.keysym.sym <= SDLK_9) {
                        size = event.key.keysym.sym - SDLK_1 + 1;
                        break;
                    }
                    else if (event.key.keysym.sym >= SDLK_KP_3 && event.key.keysym.sym <= SDLK_KP_9) {
                        size = event.key.keysym.sym - SDLK_KP_1 + 1;
                        break;
                    }
                    break;
                case SDL_KEYUP:
                    break;
                case SDL_QUIT:
                    *quit = 1;
                    return;
                default:break;
            }
        }
    }

    // free on new game in the same instance
    if (game_status->blocks != NULL) {
        for (int i = 0; i < size; ++i) {
            free(game_status->blocks[i]);
        }
        free(game_status->blocks);
    }
    if (game_status->previous != NULL) {
        for (int i = 0; i < size; ++i) {
            free(game_status->previous[i]);
        }
        free(game_status->previous);
    }

    game_status->timer = 0;
    game_status->points = 0;
    game_status->board_size = size;
    game_status->blocks = (block_t **)malloc(sizeof(block_t *)*size);
    if (game_status->blocks) {
        for (int i = 0; i < size; ++i) {
            game_status->blocks[i] = (block_t *) malloc(sizeof(block_t) * size);
            if (game_status->blocks[i] == NULL) {
                *quit = 1;
                error(screen, scrtex, renderer, charset, (char *)"Error: Not enough memory.");
                return;
            }
            for (int j = 0; j < size; ++j) {
                game_status->blocks[i][j].value = EMPTY;
                game_status->blocks[i][j].moved = 0;
            }
        }
    }
    else {
        *quit = 1;
        error(screen, scrtex, renderer, charset, (char *)"Error: Not enough memory.");
        return;
    }

    game_status->previous = (block_t **)malloc(sizeof(block_t *)*size);
    if (game_status->previous) {
        for (int i = 0; i < size; ++i) {
            game_status->previous[i] = (block_t *) malloc(sizeof(block_t) * size);
            if (game_status->previous[i] == NULL) {
                *quit = 1;
                error(screen, scrtex, renderer, charset, (char *)"Error: Not enough memory.");
                return;
            }
            for (int j = 0; j < size; ++j) {
                game_status->previous[i][j].value = EMPTY;
                game_status->previous[i][j].moved = 0;
            }
        }
    }
    else {
        *quit = 1;
        error(screen, scrtex, renderer, charset, (char *)"Error: Not enough memory.");
        return;
    }


    randomOne(game_status);

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
