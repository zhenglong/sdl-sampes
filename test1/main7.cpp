/**
 * sdl thread example 
 */
#include <SDL2/SDL.h>
#include <stdio.h>
#include <map>
#include <SDL2/SDL_thread.h>

using namespace std;

static int counter = 0;
SDL_mutex *counter_mutex;
static int exit_flag = 0;
static map<SDL_threadID, SDL_Thread*> s;

int ThreadEntryPoint(void *data) {
    const char *threadname;
    
    threadname = SDL_GetThreadName(s[SDL_ThreadID()]);
    
    while (exit_flag == 0) {
        printf("This is %s!", threadname);
        SDL_mutexP(counter_mutex);
        printf("The counter is currently %i\n", counter);
        counter++;
        SDL_mutexV(counter_mutex);
        SDL_Delay(rand() % 3000);
    }
    
    printf("%s is now exiting. \n", threadname);
    return 0;
}

int main(int argc, char **argv)
{
    SDL_Thread *thread1, *thread2, *thread3;
    
    counter_mutex = SDL_CreateMutex();
    
    thread1 = SDL_CreateThread(ThreadEntryPoint, "Thread 1", NULL);
    s.insert({SDL_GetThreadID(thread1), thread1});
    thread2 = SDL_CreateThread(ThreadEntryPoint, "Thread 2", NULL);
    s.insert({SDL_GetThreadID(thread2), thread2});
    thread3 = SDL_CreateThread(ThreadEntryPoint, "Thread 3", NULL);
    s.insert({SDL_GetThreadID(thread3), thread3});
    
    while (counter < 20) {
        SDL_Delay(1000);
    }
    
    exit_flag = 1;
    SDL_Delay(3500);
    
    SDL_DestroyMutex(counter_mutex);
    return 0;
}
