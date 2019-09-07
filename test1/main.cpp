/**
 * sdl audio example 
 */
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL_thread.h>

using namespace std;

typedef struct sound_s {
    Uint8 *samples;
    Uint32 length;
} sound_t, *sound_p;

typedef struct playing_s {
    int active;
    sound_p sound;
    Uint32 position;
} playing_t, *playing_p;

#define MAX_PLAYING_SOUNDS 10

playing_t playing[MAX_PLAYING_SOUNDS];

#define VOLUME_PER_SOUND SDL_MIX_MAXVOLUME / 2

void AudioCallback(void *user_data, Uint8 *audio, int length) {
    int i;
    
    memset(audio, 0, length);
    
    for (i = 0; i < MAX_PLAYING_SOUNDS; i++) {
        if (playing[i].active) {
            Uint8 *sound_buf;
            Uint32 sound_len;
            
            sound_buf = playing[i].sound->samples;
            sound_buf += playing[i].position;
            
            if ((playing[i].sound + length) > playing[i].sound->length) {
                sound_len = playing[i].sound->length - playing[i].position;
            } else {
                sound_len = length;
            }
            SDL_MixAudio(audio, sound_buf, sound_len, VOLUME_PER_SOUND);
            playing[i].position += length;
            if (playing[i].position >= playing[i].sound->length) {
                playing[i].active = 0;
            }
        }
    }
}

int LoadAndConvertSound(char *filename, SDL_AudioSpec *spec, sound_p sound) {
    SDL_AudioCVT cvt;
    SDL_AudioSpec loaded;
    Uint8 *new_buf;
    if (SDL_LoadWAV(filename, &loaded, &sound->samples, &sound->length) == NULL) {
        printf("Unable to load sound: %s\n", SDL_GetError());
        return 1;
    }
    
    if (SDL_BuildAudioCVT(&cvt, loaded.format, loaded.channels, loaded.freq, 
        spec->format, spec->channels, spec->freq) < 0) {
        printf("Unable to convert sound: %s\n", SDL_GetError());
        return 1;
    }
    cvt.len = sound->length;
    new_buf = (Uint8*)malloc(cvt.len * cvt.len_mult);
    if (new_buf == NULL) {
        printf("Memory allocation failed.\n");
        SDL_FreeWAV(sound->samples);
        return 1;
    }
    
    memcpy(new_buf, sound->samples, sound->length);
    
    cvt.buf = new_buf;
    if (SDL_ConvertAudio(&cvt) < 0) {
        printf("Audio conversion error: %s\n", SDL_GetError());
        free(new_buf);
        SDL_FreeWAV(sound->samples);
        return 1;
    }
    
    SDL_FreeWAV(sound->samples);
    sound->samples = new_buf;
    sound->length - sound->length * cvt.len_mult;
    printf("%s was loaded and converted successfully.\n", filename);
    return 0;
}

void ClearPlayingSounds(void) {
    int i;
    for (i = 0; i < MAX_PLAYING_SOUNDS; i++) {
        playing[i].active = 0;
    }
}

int PlaySound(sound_p sound) {
    int i;
    for (i = 0; i < MAX_PLAYING_SOUNDS; i++) {
        if (playing[i].active == 0) {
            break;
        }
    }
    
    if (i == MAX_PLAYING_SOUNDS) {
        return 1;
    }
    
    SDL_LockAudio();
    playing[i].active = 1;
    playing[i].sound = sound;
    playing[i].position = 0;
    SDL_UnlockAudio();
    return 0;
}

int main(int argc, char **argv)
{
    SDL_Window *screen;
    SDL_Event event;
    int quit_flag = 0;
    SDL_AudioSpec desired, obtained;
    sound_t cannon, explosion;
    
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        printf("Unable to initialize SDL: %s\n", SDL_GetError());
        return 1;
    }
    
    atexit(SDL_Quit);
    atexit(SDL_CloseAudio);
    
    screen = SDL_VideoInit(NULL);
    if (screen == NULL) {
        return 1;
    }
    desired.freq = 44100;
    desired.format = AUDIO_S16;
    desired.samples = 4096;
    desired.channels = 2;
    desired.callback = AudioCallback;
    desired.userdata = NULL;
    if (SDL_OpenAudio(&desired, &obtained) < 0) {
        printf("Unable to open audio device: %s\n", SDL_GetError());
        return 1;
    }
    
    if (LoadAndConvertSound("cannon.wav", &obtained, &cannon) != 0) {
        printf("Unable to load sound.\n");
        return 1;
    }
    
    if (LoadAndConvertSound("explosion.wav", &obtained, &cannon) != 0) {
        printf("Unable to load sound.\n");
        return 1;
    }
    
    ClearPlayingSounds();
    SDL_PauseAudio(0);
    return 0;