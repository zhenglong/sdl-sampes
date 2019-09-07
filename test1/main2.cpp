#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

using namespace std;

Uint32 CreateHicolorPixel(SDL_PixelFormat *fmt, Uint8 red, Uint8 green, Uint8 blue) {
//    Uint32 value;
//    
//    value = ((red >> fmt->Rloss) << fmt->Rshift) +
//        ((green >> fmt->Gloss) << fmt->Gshift) +
//        ((blue >> fmt->Bloss) << fmt->Bshift);
//    return value;
return 0xff000000;
}

int main(int argc, char **argv)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Unable to initialize SDL: %s\n", SDL_GetError());
        return 1;
    }
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
    
//    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
//    if (ren == nullptr) {
//        SDL_DestroyWindow(win);
//        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
//        return 4;
//    }

    SDL_Surface * bmp = SDL_GetWindowSurface(win);
    
//    SDL_Surface *bmp = SDL_CreateRGBSurface(0, 256, 256, 32, 
//        0, 0, 0, 0);
    
//    if (bmp == nullptr) {
//        SDL_DestroyRenderer(ren);
//        SDL_DestroyWindow(win);
//        printf("SDL_LoadBMP Error: %s\n", SDL_GetError());
//        return 5;
//    }
    
    Uint32 *raw_pixels = (Uint32*)bmp->pixels;
    int x, y;
    int temp = bmp->pitch / 4;
    for (x = 0; x < 640; x++) {
        for (y = 0; y < 480; y++) {
            Uint32 pixel_color = SDL_MapRGB(bmp->format, 0xff, 0, 0);
            int offset;
//            pixel_color = CreateHicolorPixel(bmp->format, x, 0, y);
            offset = (temp * y + x);
            raw_pixels[offset] = pixel_color;
        }
    }
    SDL_UpdateWindowSurface(win);
    
//    SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, bmp);
//    SDL_FreeSurface(bmp);
//    if (tex == nullptr) {
//        SDL_DestroyRenderer(ren);
//        SDL_DestroyWindow(win);
//        printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
//        return 6;
//    }
//    
//    SDL_RenderClear(ren);
//    SDL_RenderCopy(ren, tex, NULL, NULL);
//    SDL_RenderPresent(ren);;
    SDL_Delay(5000);
//    
//    SDL_DestroyTexture(tex);
//    SDL_DestroyRenderer(ren);
    SDL_FreeSurface(bmp);
    SDL_DestroyWindow(win);
    printf("Success!\n");

//    if (SDL_Init(SDL_INIT_VIDEO) == 0) {
//        SDL_Window* window = NULL;
//        SDL_Renderer* renderer = NULL;
//
//        if (SDL_CreateWindowAndRenderer(640, 480, 0, &window, &renderer) == 0) {
//            SDL_bool done = SDL_FALSE;
//
//            while (!done) {
//                SDL_Event event;
//
//                SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
//                SDL_RenderClear(renderer);
//
//                SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
//                SDL_RenderDrawLine(renderer, 320, 200, 300, 240);
//                SDL_RenderDrawLine(renderer, 300, 240, 340, 240);
//                SDL_RenderDrawLine(renderer, 340, 240, 320, 200);
//                SDL_RenderPresent(renderer);
//
//                while (SDL_PollEvent(&event)) {
//                    if (event.type == SDL_QUIT) {
//                        done = SDL_TRUE;
//                    }
//                }
//            }
//        }
//
//        if (renderer) {
//            SDL_DestroyRenderer(renderer);
//        }
//        if (window) {
//            SDL_DestroyWindow(window);
//        }
//    }
//    SDL_Quit();

    return 0;
}
