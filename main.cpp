#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include"core/display.h"

// set default settings, in case of error display prompt
void defaultSettings(game_t *game_status, int *quit, SDL_Surface *screen, SDL_Texture *scrtex, SDL_Renderer *renderer,
                     SDL_Surface *charset);

// free the allocated memory of board of given size
void freeBoard(block_t **board, int size);

// main
#ifdef __cplusplus
extern "C"
#endif
int main(int argc, char *argv[]) {
    int t1, t2, quit = 0, frames = 0, moveStatus = 0, fps = 0, direction = 0;
    double delta = 0, fpsTimer = 0, animation = 0, animation_duration = 0;
    srand((unsigned int)time(NULL));
    game_t game_status;
    game_status.blocks = NULL;
    game_status.buffer = NULL;
	game_status.previous = NULL;
	game_status.undo = NULL;
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

    prompt(screen, charset, (char *)"Set board size: 3-9");
    updateScreen(screen, scrtex, renderer);

    defaultSettings(&game_status, &quit, screen, scrtex, renderer, charset);
    t1 = SDL_GetTicks();

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

        // board border
        DrawBorder(screen, game_status);

        // show animation
        if (animation > 0) {
            animateMove(screen, charset, &game_status, animation_duration, animation, direction);
            updateScreen(screen, scrtex, renderer);
            animation -= delta;
            frames++;
            while(SDL_PollEvent(&event)) {
                // void
            }
            continue;
        }
        // continue move, e.g. merge after blocks' movement
        else if (moveStatus) {
            moveStatus = 0;
            copyBoard(game_status.blocks, game_status.buffer, game_status.board_size);
            // reset animation info
            for (int i = 0; i < game_status.board_size; ++i) {
                for (int j = 0; j < game_status.board_size; ++j) {
                    game_status.buffer[i][j].move_length = 0;
                }
            }
            moveStatus += mergeAll(&game_status, direction);
            moveStatus += moveAll(&game_status, direction);
            if (moveStatus) {
                animation = animation_duration = ANIMATION_DURATION;
                moveStatus = 0;
            }
            copyBoard(game_status.previous, game_status.undo, game_status.board_size);
            randomOne(&game_status);
            continue;
        }
        // else
        DrawBoard(screen, charset, game_status);

        // check for win
        if (isEnd(game_status) == 1)
            prompt(screen, charset, (char *)"Congratulations");
        // check for move possibility
        if (isEnd(game_status) == 2)
            prompt(screen, charset, (char *)"No possible move");


        updateScreen(screen, scrtex, renderer);

        // handling of events (if there were any)
        while(SDL_PollEvent(&event) && animation <= 0) {
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
                            t1 = SDL_GetTicks();
                            break;
                        case SDLK_UP:       // arrows for moves
                        case SDLK_DOWN:
                        case SDLK_LEFT:
                        case SDLK_RIGHT:
                            direction = event.key.keysym.sym;
                            copyBoard(game_status.blocks, game_status.previous, game_status.board_size);
                            copyBoard(game_status.blocks, game_status.buffer, game_status.board_size);
                            // reset animation info
                            for (int i = 0; i < game_status.board_size; ++i) {
                                for (int j = 0; j < game_status.board_size; ++j) {
                                    game_status.buffer[i][j].move_length = 0;
                                }
                            }
                            moveStatus = moveAll(&game_status, direction);
                            if (moveStatus) {
                                animation = animation_duration = ANIMATION_DURATION;
                                continue;   // first animate and then handle merge if  blocks were moved
                            }
                            else {
                                moveStatus += mergeAll(&game_status, direction);
                                moveStatus += moveAll(&game_status, direction);
                                if (moveStatus) {
                                    animation = animation_duration = 0.2;
                                    randomOne(&game_status);
                                    copyBoard(game_status.previous, game_status.undo, game_status.board_size);
                                    moveStatus = 0;
                                }
                            }
                            break;
                        case SDLK_u:
                            copyBoard(game_status.undo, game_status.blocks, game_status.board_size);
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

    freeBoard(game_status.blocks, game_status.board_size);
    freeBoard(game_status.buffer, game_status.board_size);
    freeBoard(game_status.previous, game_status.board_size);
    freeBoard(game_status.undo, game_status.board_size);


    SDL_Quit();
    return 0;
};

block_t **allocBoard(int size) {
    block_t **board = NULL;
    board = (block_t **)malloc(sizeof(block_t *)*size);
    if (board) {
        for (int i = 0; i < size; ++i) {
            board[i] = (block_t *) malloc(sizeof(block_t) * size);
            if (board[i] == NULL) {
                return NULL;
            }
            for (int j = 0; j < size; ++j) {
                board[i][j].value = EMPTY;
                board[i][j].moved = 0;
                board[i][j].move_length = 0;
            }
        }
    }
    else {
        return NULL;
    }
    return board;
}

void freeBoard(block_t **board, int size) {
    if (board != NULL) {
        for (int i = 0; i < size; ++i) {
            free(board[i]);
        }
        free(board);
    }
}

void defaultSettings(game_t *game_status, int *quit, SDL_Surface *screen, SDL_Texture *scrtex, SDL_Renderer *renderer,
                     SDL_Surface *charset) {

    // free last board on new game in the same instance
    freeBoard(game_status->blocks, game_status->board_size);
    freeBoard(game_status->buffer, game_status->board_size);
    freeBoard(game_status->previous, game_status->board_size);
    freeBoard(game_status->undo, game_status->board_size);

    // wait for size provided from user
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

    // set everything connected with game to default values
    game_status->timer = 0;
    game_status->points = 0;
    game_status->board_size = size;
    game_status->blocks = allocBoard(size);
    if (game_status->blocks == NULL) {
        *quit = 1;
        error(screen, scrtex, renderer, charset, (char *)"Error: Not enough memory.");
        return;
    }
    game_status->buffer = allocBoard(size);
    if (game_status->buffer == NULL) {
        *quit = 1;
        error(screen, scrtex, renderer, charset, (char *)"Error: Not enough memory.");
        return;
    }
    game_status->previous = allocBoard(size);
    if (game_status->previous == NULL) {
        *quit = 1;
        error(screen, scrtex, renderer, charset, (char *)"Error: Not enough memory.");
        return;
    }
    game_status->undo = allocBoard(size);
    if (game_status->undo == NULL) {
        *quit = 1;
        error(screen, scrtex, renderer, charset, (char *)"Error: Not enough memory.");
        return;
    }

    randomOne(game_status);

    copyBoard(game_status->blocks, game_status->buffer, game_status->board_size);

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
