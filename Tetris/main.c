#include <SDL2/SDL.h> // for initializing and shutdown functions
#include <SDL2/SDL_image.h> // for rendering images and graphics on screen
#include <SDL2/SDL_timer.h> // for using SDL_Delay() functions
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>

#define WIDTH 600
#define HEIGHT 800

#define CELLWIDTH WIDTH/10
#define MAXCELLX WIDTH/CELLWIDTH
#define MAXCELLY HEIGHT/CELLWIDTH
#define CELL_COUNT MAXCELLX*MAXCELLY

#define maxFPS 30

int fallSpeed = 200; // in milliseconds 

int wPressed = 0; //if piece should rotate
int aPressed = 0; //if piece should go left
int sPressed = 0; //if piece should go down faster
int dPressed = 0; //if piece should go right

SDL_Color backgroundColor = {  0,  0,  0};
SDL_Color frontColor = {255,0,0};

SDL_Rect currentPieceRect[4];
SDL_Rect simplePieceRect;
SDL_Surface* simplePieceSurface;
SDL_Texture* simplePieceTexture;

Uint32 my_callbackfunc(Uint32 interval, void *param) {
    SDL_Event event;
    SDL_UserEvent userevent;

    /* In this example, our callback pushes an SDL_USEREVENT event
    into the queue, and causes our callback to be called again at the
    same interval: */

    userevent.type = SDL_USEREVENT;
    userevent.code = 0;
    userevent.data1 = NULL;
    userevent.data2 = NULL;

    event.type = SDL_USEREVENT;
    event.user = userevent;

    SDL_PushEvent(&event);
    return(interval);
}

SDL_TimerID fallTimer;

int main(int argc, char *argv[]) {
    // returns zero on success else non-zero
    if (SDL_Init(SDL_INIT_EVENTS | SDL_INIT_TIMER | SDL_INIT_VIDEO) != 0) {printf("error initializing SDL: %s\n", SDL_GetError());}

    SDL_Window* window = SDL_CreateWindow("GAME", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,WIDTH, HEIGHT,0 );
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    fallTimer = SDL_AddTimer(fallSpeed, my_callbackfunc, NULL);

    SDL_ShowCursor(SDL_DISABLE);

    simplePieceRect.x = 0;
    simplePieceRect.y = 0;
    simplePieceRect.w = CELLWIDTH;
    simplePieceRect.h = CELLWIDTH;

    int close = 0;
    while (!close) {
        SDL_Event event;
 
        // Events management
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {close = 1;}
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.scancode) {
                case SDL_SCANCODE_W:
                case SDL_SCANCODE_UP:
                    wPressed = 1;
                    if (simplePieceRect.y >= CELLWIDTH) {simplePieceRect.y -= CELLWIDTH;}
                    break;
                case SDL_SCANCODE_S:
                case SDL_SCANCODE_DOWN:
                    sPressed = 1;
                    if (simplePieceRect.y + simplePieceRect.h + 1 <= HEIGHT) {simplePieceRect.y += CELLWIDTH;}
                    break;
                case SDL_SCANCODE_A:
                case SDL_SCANCODE_LEFT:
                    aPressed = 1;
                    if (simplePieceRect.x >= CELLWIDTH) {simplePieceRect.x -= CELLWIDTH;}
                    break;
                case SDL_SCANCODE_D:
                case SDL_SCANCODE_RIGHT:
                dPressed = 1;
                    if (simplePieceRect.x + simplePieceRect.w + 1 <= WIDTH) {simplePieceRect.x += CELLWIDTH;}
                    break;
                }
            } else if (event.type == SDL_KEYUP) {
                switch (event.key.keysym.scancode) {
                case SDL_SCANCODE_W:
                case SDL_SCANCODE_UP:
                    wPressed = 0;
                    break;
                case SDL_SCANCODE_A:
                case SDL_SCANCODE_LEFT:
                    aPressed = 0;
                    break;
                case SDL_SCANCODE_S:
                case SDL_SCANCODE_DOWN:
                    sPressed = 0;
                    break;
                case SDL_SCANCODE_D:
                case SDL_SCANCODE_RIGHT:
                    dPressed = 0;
                    break;
                }
            } else if (event.type == SDL_USEREVENT) {
                if (simplePieceRect.y + simplePieceRect.h + 1 <= HEIGHT) {simplePieceRect.y += CELLWIDTH;}
                
            }
        }

        simplePieceSurface = SDL_CreateRGBSurface(0, CELLWIDTH, CELLWIDTH, 32, 0, 0, 0, 0);
        SDL_FillRect(simplePieceSurface, NULL, SDL_MapRGB(simplePieceSurface->format, 255, 0, 0));
        simplePieceTexture = SDL_CreateTextureFromSurface(renderer, simplePieceSurface);
    
 
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, simplePieceTexture, NULL, &simplePieceRect);
        SDL_RenderPresent(renderer); // triggers the double buffers for multiple rendering
        SDL_Delay(1000 / maxFPS); // calculates to 60 fps
    }
 
    SDL_FreeSurface(simplePieceSurface);
    SDL_DestroyRenderer(renderer); // destroy renderer
    SDL_DestroyWindow(window); // destroy window
    SDL_Quit(); // close SDL
 
    return 0;
}