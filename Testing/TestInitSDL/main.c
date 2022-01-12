
#include <stdio.h>
#include <SDL2/SDL.h> // for initializing and shutdown functions
#include <SDL2/SDL_image.h> // for rendering images and graphics on screen
#include <SDL2/SDL_timer.h> // for using SDL_Delay() functions
 
int main(int argc, char *argv[]) {
    // returns zero on success else non-zero
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0) {printf("error initializing SDL: %s\n", SDL_GetError());}

    // creates a window
    SDL_Window* window = SDL_CreateWindow("Window", 50,50, 500, 500, 0);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {printf("Error in renderer\n");}

    SDL_Surface* surface = IMG_Load("ghost.gif");
    if (!surface) {printf("Error in surface\n");}

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {printf("Error in texture\n");}

    SDL_FreeSurface(surface);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);

    int close = 0;

    while (!close) {
        SDL_Event event;
 
        // Events management
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {close = 1;}
        }
    }
    
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window); // destroy window
    SDL_Quit(); // close SDL

    return 0;
}