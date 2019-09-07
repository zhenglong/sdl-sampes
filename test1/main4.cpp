#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

using namespace std;

Uint32 CreateHicolorPixel(SDL_PixelFormat *fmt, Uint8 red, Uint8 green, Uint8 blue) {
    Uint32 value;
    
    value = ((red >> fmt->Rloss) << fmt->Rshift) +
        ((green >> fmt->Gloss) << fmt->Gshift) +
        ((blue >> fmt->Bloss) << fmt->Bshift);
    return value;
}

int main(int argc, char **argv)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Unable to initialize SDL: %s\n", SDL_GetError());
        return 1;
    }
    IMG_Init(IMG_INIT_PNG);
    atexit(SDL_Quit);
    auto screen = SDL_VideoInit(NULL);
    if (screen != 0) {
        printf("Error initailizing SDL video: %s\n", SDL_GetError());
        return 2;
    }
    SDL_Window *win = SDL_CreateWindow("Hello world!", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
    if (win == 0) {
        printf("SDL_createWindow Error: %s\n", SDL_GetError());
        return 3;
    }
    
    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (ren == nullptr) {
        SDL_DestroyWindow(win);
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        return 4;
    }

    
    SDL_Surface *bmp = IMG_Load("/home/tristan/Documents/test/build-debug/2.png");
    
    if (bmp == nullptr) {
        SDL_DestroyRenderer(ren);
        SDL_DestroyWindow(win);
        printf("SDL_LoadBMP Error: %s\n", SDL_GetError());
        return 5;
    }
    
    SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, bmp);
    SDL_FreeSurface(bmp);
    if (tex == nullptr) {
        SDL_DestroyRenderer(ren);
        SDL_DestroyWindow(win);
        printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
        return 6;
    }
    
    SDL_RenderClear(ren);
    SDL_RenderCopy(ren, tex, NULL, NULL);
    SDL_RenderPresent(ren);
    SDL_Delay(5000);
    
    IMG_Quit();
    SDL_DestroyTexture(tex);
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    printf("Success!\n");
    return 0;
}
