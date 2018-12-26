//
// Created by Tomasz Piechocki on 26/12/2018.
//

#ifndef INC_2048_DISPLAY_H
#define INC_2048_DISPLAY_H

#include"core.h"

#ifdef __cplusplus
extern "C" {
#endif

// draw a text txt on surface screen, starting from the point (x, y)
// charset is a 128x128 bitmap containing character images in ASCII order
void DrawString(SDL_Surface *screen, int x, int y, const char *text, SDL_Surface *charset);

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

#ifdef __cplusplus
}
#endif
#endif //INC_2048_DISPLAY_H
