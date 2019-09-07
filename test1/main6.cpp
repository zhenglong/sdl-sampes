/**
 * sdl event example 
 */
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

using namespace std;


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
    
    SDL_Event event;
    while (SDL_WaitEvent(&event) != 0) {
        SDL_Keysym keysym;
        switch (event.type) {
            case SDL_KEYDOWN:
                printf("key pressed.\n");
                keysym = event.key.keysym;
                printf("SDL keysym is %i.", keysym.sym);
                printf("(%s) ", SDL_GetKeyName(keysym.sym));
                if (event.key.keysym.mod & KMOD_LSHIFT) {
                    printf("Left Shift is down. \n");
                } else {
                    printf("Left Shift is up. \n");
                }
                if (keysym.sym == SDLK_q) {
                    printf("'Q' pressed exiting\n");
                    SDL_DestroyWindow(win);
                    exit(0);
                }
                break;
            case SDL_MOUSEMOTION:
                printf("New position is (%i, %i).", event.motion.x, event.motion.y);
                printf("That is a (%i, %i) change. \n", event.motion.xrel, event.motion.yrel);
                break;
            case SDL_MOUSEBUTTONDOWN:
                printf("Mouse button pressed.");
                printf("Button %i at (%i, %i)\n", event.button.button, event.button.x, event.button.y);
                break;
            case SDL_QUIT:
                printf("Quti event.\n");
                SDL_DestroyWindow(win);
                exit(0);
        }
    } 
    
    
    SDL_DestroyWindow(win);
    printf("Success!\n");
    return 0;
}
