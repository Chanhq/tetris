#include <stdlib.h>
#include <stdio.h>
#include "SDL.h"


int main(void) {
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0) {
        fprintf(stderr, "\nUnable to initialize SDL: Â %s\n", SDL_GetError());
        return 1;
    }
    atexit(SDL_Quit);


    return 0;
} 