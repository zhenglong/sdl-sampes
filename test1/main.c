#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdio.h>

void DrawLine32(SDL_Surface *surf, int x0, int y0, int x1, int y1, Uint32 color) {
    Uint32 *buffer;
    int drawpos;
    int xspan, yspan;
    int xinc, yinc;
    int sum;
    int i;
    
    if (SDL_MUSTLOCK(surf)) {
        if (SDL_LockSurface(surf) < 0) {
            printf("Error locking surface: %s\n", SDL_GetError());
            abort();
        }
    }
    
    buffer = (Uint32*)surf->pixels;
    
    xspan = x1 - x0;
    yspan = y1 - y0;
    
    if (xspan < 0) {
        xinc = -1;
        xspan = -xspan;
    } else {
        xinc = 1;
    }
    
    if (yspan < 0) {
        yinc = -surf->w;
        yspan = -yspan;
    } else {
        yinc = surf->w;
    }
    
    i = 0;
    sum = 0;
    
    drawpos = surf->w * y0 + x0;
    int xspan2 = xspan << 1;
    int yspan2 = yspan << 1;
    if (xspan < yspan) {
        for (i = 0; i < yspan; i++) {
            buffer[drawpos] = color;
            sum += (xspan2);
            
            if (sum >= 0) {
                drawpos += xinc;
                sum -= (yspan2);
            }
            drawpos += yinc;
        }
    } else {
        for (i = 0; i < xspan; i++) {
            buffer[drawpos] = color;
            sum += (yspan2);
            if (sum >= xspan) {
                drawpos += yinc;
                sum -= (xspan2);
            }
            drawpos += xinc;
        }
    }
    SDL_UnlockSurface(surf);
}

int main() {
    SDL_Surface *screen;
    
    if (SDL_Init(SDL_INIT_VIDEO)) {
        printf("Unable to initialize SDL: %s\n", SDL_GetError());
        return 1;
    }
    atexit(SDL_Quit);
    if (SDL_VideoInit(NULL)) {
        printf("Error initailizing SDL video: %s\n", SDL_GetError());
        return 2;
    }
    SDL_Window *win = SDL_CreateWindow("Hello world!", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
    if (win == 0) {
        printf("SDL_createWindow Error: %s\n", SDL_GetError());
        return 3;
    }
    screen = SDL_GetWindowSurface(win);
    DrawLine32(screen, 0, 0, 300, 300, 0xFF00);
    SDL_UpdateWindowSurface(win);
    
    SDL_Delay(10000);
    SDL_DestroyWindow(win);
    return 0;
}