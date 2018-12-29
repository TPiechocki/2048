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
void DrawLegend(SDL_Surface *screen, SDL_Surface *charset, double fps, double timer) {
    char text[128];
    int legendPosition = 10;
    sprintf(text, "%.0lf FPS", fps);
    DrawString(screen, GAME_WIDTH + LEGEND_WIDTH - strlen(text)*8 - 8, legendPosition, text, charset, 1);
    legendPosition += 32;
    sprintf(text, "Game duration: %.1lf s ", timer);
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
}

EXTERNC
void DrawBoard(SDL_Surface *screen, SDL_Surface *charset, game_t game_status) {
    // width and height of single block, board with padding of one block
    int width = GAME_WIDTH/(BOARD_SIZE+2);
    int height = SCREEN_HEIGHT/(BOARD_SIZE+2);

    // board border
    DrawLine(screen, width-1, height-1, 4*height + 2, 0, 1, colour(screen, (char *)"black"));
    DrawLine(screen, 5*width, height-1, 4*height + 2, 0, 1, colour(screen, (char *)"black"));
    DrawLine(screen, width-1, height-1, 4*width + 2, 1, 0, colour(screen, (char *)"black"));
    DrawLine(screen, width-1, 5*height, 4*width + 2, 1, 0, colour(screen, (char *)"black"));

    //blocks
    for (int i = 0; i < BOARD_SIZE ; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            char txt[10];
            double scale = 1.8;
            switch (game_status.blocks[j][i].value) {
                case EMPTY:
                    DrawRectangle(screen, width * (j + 1), height * (i + 1), width, height,
                                  colour(screen, (char *)"black"), colour(screen, (char *)"emptyblock"));
                    break;
                default:    // for full tiles
                    sprintf(txt, "%d", game_status.blocks[j][i].value);
                    DrawRectangle(screen, width * (j + 1), height * (i + 1), width, height,
                                  colour(screen, (char *)"black"), colour(screen, txt));
                    DrawString(screen, (width * (j + 1)) + width/2 - strlen(txt)*(int)(4*scale),
                               height * (i + 1) + height/2 - (int)(4*scale), txt, charset, scale);
                    break;
            }
        }
    }
}
