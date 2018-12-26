#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_WARNINGS
#include<math.h>
#include<stdio.h>
#include<string.h>
#include"core/display.h"

// main
#ifdef __cplusplus
extern "C"
#endif
int main(int argc, char *argv[]) {
    int t1, t2, quit, frames, legendPosition;
    double delta, worldTime, fpsTimer, fps, distance, etiSpeed;
    SDL_Event event;
    SDL_Surface *screen = NULL, *charset = NULL;
    SDL_Surface *eti = NULL;
    SDL_Texture *scrtex = NULL;
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

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

    eti = SDL_LoadBMP("./eti.bmp");
    if(eti == NULL) {
        printf("SDL_LoadBMP(eti.bmp) error: %s\n", SDL_GetError());
        SDL_FreeSurface(charset);
        SDL_FreeSurface(screen);
        SDL_DestroyTexture(scrtex);
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
        return 1;
    };

    char text[128];

    t1 = SDL_GetTicks();

    frames = 0;
    fpsTimer = 0;
    fps = 0;
    quit = 0;
    worldTime = 0;
    distance = 0;
    etiSpeed = 1;

    while(!quit) {
        t2 = SDL_GetTicks();

        // here t2-t1 is the time in milliseconds since
        // the last screen was drawn
        // delta is the same time in seconds
        delta = (t2 - t1) * 0.001;
        t1 = t2;

        worldTime += delta;

        distance += etiSpeed * delta;

        SDL_FillRect(screen, NULL, colour(screen, (char *)"background"));

        DrawSurface(screen, eti,
                    (int)(GAME_WIDTH / 2 + sin(distance) * SCREEN_HEIGHT / 3),
                    (int)(SCREEN_HEIGHT / 2 + cos(distance) * SCREEN_HEIGHT / 3));

        fpsTimer += delta;
        if(fpsTimer > 0.5) {`
            fps = frames * 2;
            frames = 0;
            fpsTimer -= 0.5;
        };

        // info text
        DrawLine(screen, GAME_WIDTH, 0, SCREEN_HEIGHT, 0, 1, colour(screen, (char *)"border"));
        legendPosition = 10;
        sprintf(text, "%.0lf FPS", fps);
        DrawString(screen, GAME_WIDTH + LEGEND_WIDTH - strlen(text)*8 - 8, legendPosition, text, charset);
        legendPosition += 32;
        sprintf(text, "Game duration: %.1lf s ", worldTime);
        DrawString(screen, GAME_WIDTH + 10, legendPosition, text, charset);
        legendPosition += 16;
        sprintf(text, "Esc - exit");
        DrawString(screen, GAME_WIDTH + 10, legendPosition, text, charset);
        legendPosition += 16;
        sprintf(text, " \030  - faster");
        DrawString(screen, GAME_WIDTH + 10, legendPosition, text, charset);
        legendPosition += 16;
        sprintf(text, " \031  - slower");
        DrawString(screen, GAME_WIDTH + 10, legendPosition, text, charset);

        SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
		SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, scrtex, NULL, NULL);
        SDL_RenderPresent(renderer);

        // handling of events (if there were any)
        while(SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_KEYDOWN:
                    if(event.key.keysym.sym == SDLK_ESCAPE) quit = 1;
                    else if(event.key.keysym.sym == SDLK_UP) etiSpeed = 2.0;
                    else if(event.key.keysym.sym == SDLK_DOWN) etiSpeed = 0.3;
                    break;
                case SDL_KEYUP:
                    etiSpeed = 1.0;
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
