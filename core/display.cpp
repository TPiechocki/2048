//
// Created by Tomasz Piechocki on 26/12/2018.
//

#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include"display.h"


#ifdef __GNUC__
#include<stdlib.h>
#endif
#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

EXTERNC
void updateScreen(SDL_Surface *screen, SDL_Texture *scrtex, SDL_Renderer *renderer) {
    SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, scrtex, NULL, NULL);
    SDL_RenderPresent(renderer);
}

EXTERNC
void DrawString(SDL_Surface *screen, int x, int y, const char *text,
        SDL_Surface * charset, double scale) {
    int px, py, c;
    SDL_Rect s, d;
    s.w = 8;
    s.h = 8;
    d.w = (int)(8*scale);
    d.h = (int)(8*scale);
    while(*text) {
        c = *text & 255;
        px = (c % 16) * 8;
        py = (c / 16) * 8;
        s.x = px;
        s.y = py;
        d.x = x;
        d.y = y;
        SDL_BlitScaled(charset, &s, screen, &d);
        x += (int)(8*scale);
        text++;
    };
};

EXTERNC
void DrawSurface(SDL_Surface *screen, SDL_Surface *sprite, int x, int y) {
    SDL_Rect dest;
    dest.x = x - sprite->w / 2;
    dest.y = y - sprite->h / 2;
    dest.w = sprite->w;
    dest.h = sprite->h;
    SDL_BlitSurface(sprite, NULL, screen, &dest);
};

EXTERNC
void DrawPixel(SDL_Surface *surface, int x, int y, Uint32 color) {
    int bpp = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
    *(Uint32 *)p = color;
};

EXTERNC
void DrawLine(SDL_Surface *screen, int x, int y, int l, int dx, int dy, Uint32 color) {
    for(int i = 0; i < l; i++) {
        DrawPixel(screen, x, y, color);
        x += dx;
        y += dy;
    };
};

EXTERNC
void DrawRectangle(SDL_Surface *screen, int x, int y, int l, int k,
                   Uint32 outlineColor, Uint32 fillColor) {
    DrawLine(screen, x, y, k, 0, 1, outlineColor);
    DrawLine(screen, x + l - 1, y, k, 0, 1, outlineColor);
    DrawLine(screen, x, y, l, 1, 0, outlineColor);
    DrawLine(screen, x, y + k - 1, l, 1, 0, outlineColor);
    for(int i = y + 1; i < y + k - 1; i++)
        DrawLine(screen, x + 1, i, l - 2, 1, 0, fillColor);
}

EXTERNC
void DrawLegend(SDL_Surface *screen, SDL_Surface *charset, double fps, game_t game_status) {
    char text[128];
    int legendPosition = 10;
    sprintf(text, "%.0lf FPS", fps);
    DrawString(screen, GAME_WIDTH + LEGEND_WIDTH - (int)strlen(text)*8 - 8, legendPosition, text, charset, 1);
    legendPosition += 32;
    sprintf(text, "Points: %d", game_status.points);
    DrawString(screen, GAME_WIDTH + 10, legendPosition, text, charset, 1);
    legendPosition += 16;
    sprintf(text, "Game duration: %.1lf s ", game_status.timer);
    DrawString(screen, GAME_WIDTH + 10, legendPosition, text, charset, 1);
    legendPosition += 32;
    sprintf(text, " Esc  - exit");
    DrawString(screen, GAME_WIDTH + 10, legendPosition, text, charset, 1);
    legendPosition += 16;
    sprintf(text, "  n   - new game");
    DrawString(screen, GAME_WIDTH + 10, legendPosition, text, charset, 1);
    legendPosition += 16;
    sprintf(text, "\032\030\033\031  - move");
    DrawString(screen, GAME_WIDTH + 10, legendPosition, text, charset, 1);
    legendPosition += 16;
    sprintf(text, "  u   - undo move");
    DrawString(screen, GAME_WIDTH + 10, legendPosition, text, charset, 1);
}

EXTERNC
void DrawBorder(SDL_Surface *screen, game_t game) {
    // width and height of single block, board with padding of one block
    int width = GAME_WIDTH/(game.board_size+2);
    int height = SCREEN_HEIGHT/(game.board_size+2);

    // board border
    DrawLine(screen, width-1, height-1, game.board_size*height + 2, 0, 1, colour(screen, (char *)"black"));
    DrawLine(screen, (game.board_size+1)*width, height-1, game.board_size*height + 2, 0, 1, colour(screen, (char *)"black"));
    DrawLine(screen, width-1, height-1, game.board_size*width + 2, 1, 0, colour(screen, (char *)"black"));
    DrawLine(screen, width-1, (game.board_size+1)*height, game.board_size*width + 2, 1, 0, colour(screen, (char *)"black"));
}

EXTERNC
void DrawBoard(SDL_Surface *screen, SDL_Surface *charset, game_t game) {
    // width and height of single block, board with padding of one block
    int width = GAME_WIDTH/(game.board_size+2);
    int height = SCREEN_HEIGHT/(game.board_size+2);

    //blocks
    for (int i = 0; i < game.board_size ; ++i) {
        for (int j = 0; j < game.board_size; ++j) {
            char txt[10];
            double scale = width / 32.0;
            switch (game.blocks[j][i].value) {
                case EMPTY:
                    DrawRectangle(screen, width * (j + 1), height * (i + 1), width, height,
                                  colour(screen, (char *)"black"), colour(screen, (char *)"emptyblock"));
                    break;
                default:    // for full tiles
                    sprintf(txt, "%d", game.blocks[j][i].value);
                    DrawRectangle(screen, width * (j + 1), height * (i + 1), width, height,
                                  colour(screen, (char *)"black"), colour(screen, txt));
                    DrawString(screen, (width * (j + 1)) + width/2 - (int)(strlen(txt)*4*scale),
                               height * (i + 1) + height/2 - (int)(4*scale), txt, charset, scale);
                    break;
            }
        }
    }
}

EXTERNC
void animateMove(SDL_Surface *screen, SDL_Surface *charset, game_t *game, double duration, double actual,
        int direction) {
    // width and height of single block, board with padding of one block
    int width = GAME_WIDTH/(game->board_size+2);
    int height = SCREEN_HEIGHT/(game->board_size+2);

    char txt[10] = "\0";
    int vertical = 0, horizontal = 0, length, x, y;
    switch (direction) {
        case SDLK_UP:
            vertical = -1;
            break;
        case SDLK_DOWN:
            vertical = 1;
            break;
        case SDLK_LEFT:
            horizontal = -1;
            break;
        case SDLK_RIGHT:
            horizontal = 1;
            break;
        default: break;
    }
    // empty blocks
    for (int i = 0; i < game->board_size; ++i) {
        for (int j = 0; j < game->board_size; ++j) {
            DrawRectangle(screen, width * (j + 1), height * (i + 1), width, height,
                          colour(screen, (char *) "black"), colour(screen, (char *) "emptyblock"));

        }
    }
    // moving blocks
    for (int i = 0; i < game->board_size; ++i) {
        for (int j = 0; j < game->board_size; ++j) {
            double scale = width / 32.0;
            if (game->buffer[j][i].move_length != 0) {
                sprintf(txt, "%d", game->buffer[j][i].value);
                length = game->buffer[j][i].move_length;
                x = width * (j + 1) + (int)(horizontal*width*((duration-actual)*length/duration));
                y = height * (i + 1) + (int)(vertical*width*((duration-actual)*length/duration));
                DrawRectangle(screen, x, y, width, height,
                              colour(screen, (char *) "black"), colour(screen, txt));
                DrawString(screen, x + width / 2 - (int) (strlen(txt) * 4 * scale),
                           y + height / 2 - (int) (4 * scale), txt, charset, scale);
            }
        }
    }
    // static blocks
    for (int i = 0; i < game->board_size; ++i) {
        for (int j = 0; j < game->board_size; ++j) {
            double scale = width / 32.0;
            if (game->buffer[j][i].move_length == 0 && game->buffer[j][i].value != EMPTY) {
                sprintf(txt, "%d", game->buffer[j][i].value);
                DrawRectangle(screen, width * (j + 1), height * (i + 1), width, height,
                              colour(screen, (char *) "black"), colour(screen, txt));
                DrawString(screen, (width * (j + 1)) + width / 2 - (int) (strlen(txt) * 4 * scale),
                           height * (i + 1) + height / 2 - (int) (4 * scale), txt, charset, scale);
            }
        }
    }
}

EXTERNC
void prompt(SDL_Surface *screen, SDL_Surface *charset, char *msg) {
    DrawRectangle(screen, SCREEN_WIDTH/10, SCREEN_HEIGHT/10, 8*SCREEN_WIDTH/10, 8*SCREEN_HEIGHT/10,
                  colour(screen, (char *)"black"), colour(screen, (char *)"white"));
    float scale = 1.5;
    DrawString(screen, SCREEN_WIDTH/2 - (int)(strlen(msg)*4*scale), SCREEN_HEIGHT/2 - (int)(4*scale), msg, charset, scale);
}

EXTERNC
void error(SDL_Surface *screen, SDL_Texture *scrtex, SDL_Renderer *renderer, SDL_Surface *charset, char *msg) {
    prompt(screen, charset, msg);

    updateScreen(screen, scrtex, renderer);

    SDL_Event event;
    while (1) {
        while(SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_RETURN) {
                        return;
                    }
                    break;
                case SDL_KEYUP:
                    break;
                case SDL_QUIT:
                    return;
                default:break;
            }
        }
    }
}
