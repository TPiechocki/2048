//
// Created by Tomasz Piechocki on 26/12/2018.
//

#ifndef INC_2048_DISPLAY_H
#define INC_2048_DISPLAY_H

#include"core.h"

#ifdef __cplusplus
extern "C" {
#endif

void updateScreen(SDL_Surface *screen, SDL_Texture *scrtex, SDL_Renderer *renderer);

// draw a text txt on surface screen, starting from the point (x, y)
// scale multiplies font size, default font size is 8px
// charset is a 128x128 bitmap containing character images in ASCII order
void DrawString(SDL_Surface *screen, int x, int y, const char *text,
                SDL_Surface * charset, double scale);

// draw a surface sprite on a surface screen in point (x, y)
// (x, y) is the center of sprite on screen
void DrawSurface(SDL_Surface *screen, SDL_Surface *sprite, int x, int y);

// draw a single pixel
void DrawPixel(SDL_Surface *surface, int x, int y, Uint32 color);

// draw a vertical (when dx = 0, dy = 1) or horizontal (when dx = 1, dy = 0) line
void DrawLine(SDL_Surface *screen, int x, int y, int l, int dx, int dy, Uint32 color);

// draw a rectangle of size l by k
void DrawRectangle(SDL_Surface *screen, int x, int y, int l, int k,
                   Uint32 outlineColor, Uint32 fillColor);

/**
 * Draw a legend
 * @param screen
 * @param charset - font
 * @param fps - frames per second value
 * @param game_status - game structure (timer from start and player's points)
 */
void DrawLegend(SDL_Surface *screen, SDL_Surface *charset, double fps, game_t game_status);

/**
 * Display board and blocks within it
 * @param screen
 * @param charset - font
 * @param game - game structure (board)
 */
void DrawBoard(SDL_Surface *screen, SDL_Surface *charset, game_t game);

/**
 * Put prompt into screen surface
 * @param screen
 * @param charset
 * @param msg - message to be displayed
 */
void prompt(SDL_Surface *screen, SDL_Surface *charset, char *msg);

/**
 * Also show prompt but this time wait for enter or escape and quit the program.
 * @param screen
 * @param scrtex
 * @param renderer
 * @param charset - font
 * @param msg - message to be shown
 */
void error(SDL_Surface *screen, SDL_Texture *scrtex, SDL_Renderer *renderer, SDL_Surface *charset, char *msg);

#ifdef __cplusplus
}
#endif
#endif //INC_2048_DISPLAY_H
