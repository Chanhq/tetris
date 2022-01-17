#include <SDL2/SDL.h> // for initializing and shutdown functions
#include <SDL2/SDL_image.h> // for rendering images and graphics on screen
#include <SDL2/SDL_timer.h> // for using SDL_Delay() functions
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>

#define WIDTH 300
#define HEIGHT 600
#define maxFPS 20

#define MAXCELLX 10 //has to be a dividor of WIDTH
#define MAXCELLY 20 ////has to be a dividor of HEIGHT

#define CELLWIDTH 30 //has to be a dividor of WIDTH and HEIGHT

int fallSpeed = 300; // in milliseconds (lower is faster)

int wPressed = 0; //if piece should rotate
int aPressed = 0; //if piece should go left
int sPressed = 0; //if piece should go down faster
int dPressed = 0; //if piece should go right

int field[MAXCELLY][MAXCELLX] = {
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0},
};


SDL_Color backgroundColor = {  0,  0,  0};
SDL_Color frontColor = {255,0,0};

//SDL_Rect currentPieceRect[4];
SDL_Rect currentPieceRect;
SDL_Surface* currentPieceSurface;
SDL_Texture* currentPieceTexture;

SDL_Rect blockRect;
SDL_Surface* blockSurface;
SDL_Texture* blockTexture;



Uint32 generateEventAfterDelay(Uint32 interval, void *param) {
    SDL_Event event;
    event.type = SDL_USEREVENT;

    SDL_PushEvent(&event);
    return(interval);
}

SDL_TimerID fallTimer;

int main(int argc, char *argv[]) {
    // returns zero on success else non-zero
    if (SDL_Init(SDL_INIT_EVENTS | SDL_INIT_TIMER | SDL_INIT_VIDEO) != 0) {printf("error initializing SDL: %s\n", SDL_GetError());}

    SDL_Window* window = SDL_CreateWindow("GAME", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,WIDTH, HEIGHT,0 );
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    fallTimer = SDL_AddTimer(fallSpeed, generateEventAfterDelay, NULL);

    SDL_ShowCursor(SDL_DISABLE);

    currentPieceRect.x = 0;
    currentPieceRect.y = 0;
    currentPieceRect.w = CELLWIDTH;
    currentPieceRect.h = CELLWIDTH;

    blockRect.w = CELLWIDTH;
    blockRect.h = CELLWIDTH;

    printf("CELLWIDTH = %d\n",CELLWIDTH);
    printf("MAXCELLX = %d\n",MAXCELLX);
    printf("MAXCELLY = %d\n",MAXCELLY);

    int close = 0;
    while (!close) {
        SDL_Event event;
 
        // Events management
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {close = 1;}
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.scancode) {
                case SDL_SCANCODE_W:
                case SDL_SCANCODE_UP:
                    wPressed = 1;
                    //Stück drehen
                    break;
                case SDL_SCANCODE_S:
                case SDL_SCANCODE_DOWN:
                    sPressed = 1;
                    //Stück schneller nach unten bewegen
                    if (currentPieceRect.y/CELLWIDTH < MAXCELLY-1 && field[currentPieceRect.y/CELLWIDTH+1][currentPieceRect.x/CELLWIDTH] == 0) {currentPieceRect.y += CELLWIDTH;}
                    break;
                case SDL_SCANCODE_A:
                case SDL_SCANCODE_LEFT:
                    aPressed = 1;
                    if (currentPieceRect.x/CELLWIDTH > 0 && field[currentPieceRect.y/CELLWIDTH][currentPieceRect.x/CELLWIDTH-1] == 0) {currentPieceRect.x -= CELLWIDTH;}
                    break;
                case SDL_SCANCODE_D:
                case SDL_SCANCODE_RIGHT:
                    dPressed = 1;
                    if (currentPieceRect.x/CELLWIDTH < MAXCELLX-1 && field[currentPieceRect.y/CELLWIDTH][currentPieceRect.x/CELLWIDTH+1] == 0) {currentPieceRect.x += CELLWIDTH;}
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

                int yPos = currentPieceRect.y/CELLWIDTH;
                int xPos = currentPieceRect.x/CELLWIDTH;

                int collide = 0;

                if (yPos +1 == MAXCELLY) {collide = 1;}
                else {
                    for (int x = 0; x<MAXCELLX; x++) {
                        if (field[yPos + 1][x]!=0 && xPos == x) {
                            collide = 1;
                            printf("collision on %d,%d \n",x,yPos);
                        }
                    }
                }

                if (collide) {
                    field[(currentPieceRect.y/CELLWIDTH)][currentPieceRect.x/CELLWIDTH] = 1;
                    int lineComplete = 1; //true
                    for (int x = 0; x<MAXCELLX; x++) {
                        if (field[yPos][x]==0) {lineComplete = 0;} //line isnt complete
                    }
                    if (lineComplete) {
                        printf("Line is complete! Gets deleted.\n");
                        for (int y = yPos; y>0; y--) {
                            for (int x = 0; x<MAXCELLX; x++) {
                                field[y][x] = field[y-1][x];
                            }
                        }
                        for (int x = 0; x<MAXCELLX; x++) {
                            field[0][x] = 0;
                        }
                    }
                    currentPieceRect.x = (MAXCELLX/2)*CELLWIDTH;
                    currentPieceRect.y = 0;
                } else {
                    currentPieceRect.y += CELLWIDTH;
                }
                
                
                
                
            }
        }


        SDL_RenderClear(renderer);

        for (int y = 0; y<MAXCELLY; y++) {
            for (int x = 0; x<MAXCELLX; x++) {
                

                if (field[y][x] != 0) {      
                    blockRect.x = x*CELLWIDTH;
                    blockRect.y = y*CELLWIDTH;
                    blockSurface = SDL_CreateRGBSurface(0, CELLWIDTH, CELLWIDTH, 32, 0, 0, 0, 0);
                    SDL_FillRect(blockSurface, NULL, SDL_MapRGB(blockSurface->format, 0, 0, 255));
                    blockTexture = SDL_CreateTextureFromSurface(renderer, blockSurface);
                    SDL_RenderCopy(renderer, currentPieceTexture, NULL, &blockRect);
                }
            }
        }

        currentPieceSurface = SDL_CreateRGBSurface(0, CELLWIDTH, CELLWIDTH, 32, 0, 0, 0, 0);
        SDL_FillRect(currentPieceSurface, NULL, SDL_MapRGB(currentPieceSurface->format, 255, 0, 0));
        currentPieceTexture = SDL_CreateTextureFromSurface(renderer, currentPieceSurface);
        
        SDL_RenderCopy(renderer, currentPieceTexture, NULL, &currentPieceRect);
        SDL_RenderPresent(renderer); // triggers the double buffers for multiple rendering
        SDL_Delay(1000 / maxFPS); // calculates to 60 fps
    }
 
    SDL_FreeSurface(currentPieceSurface);
    SDL_DestroyRenderer(renderer); // destroy renderer
    SDL_DestroyWindow(window); // destroy window
    SDL_Quit(); // close SDL
 
    return 0;
}