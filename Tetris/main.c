#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#include <SDL2/SDL.h> // for initializing and shutdown functions
#include <SDL2/SDL_image.h> // for rendering images and graphics on screen
#include <SDL2/SDL_timer.h> // for using SDL_Delay() functions
//#include <SDL_mixer.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_ttf.h>

/*
    IMPORTANT: Coordinates with an "s" before it mean window-coordinates (such as "sx" or "sy")
    Variables of coords without anything in front of the name store ingame coords!
*/

#define SWIDTH 320 //screen coord
#define SHEIGHT 620
#define maxFPS 20
#define OFFSETSX 10
#define OFFSETSY 10

#define WIDTH 10 //ingame coord; has to be a dividor of SWIDTH
#define HEIGHT 20 ////has to be a dividor of SHEIGHT

#define SBLOCKWIDTH 30 //has to be a dividor of WIDTH and HEIGHT

typedef int8_t i8;  //mostly used for ingame coords
typedef int32_t i32; //mostly used for screen coords

i8 tetrinoColor = 1; //has to be smaller than 8
i8 backgroundBlockColor = 0;

i32 fallSpeed = 600; 
i32 fasterFallSpeed = 100;
// if key s is not pressed: in double milliseconds (lower is faster)
// if key s is     pressed: in        milliseconds (lower is faster)

//const i8 tSpawnPosX = WIDTH/2-2;
//const i8 tSpawnPosY = -3;

const i8 tSpawnPosX = WIDTH/2-1;
const i8 tSpawnPosY = 0;

//important variables to save information about the current, falling tetrino (t for tetrino)
i8 tForm; //type of tetrino (0-6), defined as the index in the TETRINOS-list
i8 tRot;  //rotation of tetrino (0-3), defined as the second index in the Tetrino list
i8 tColor;  //rotation of tetrino (0-3), defined as the second index in the Tetrino list
i8 tPosX;
i8 tPosY;

SDL_TimerID fallTimer;
SDL_TimerID fasterFallTimer; //if key s is pressed
SDL_Window* window;
SDL_Renderer* renderer;
SDL_Color black = {  0,  0,  0};
SDL_Color white = {255,255,255};
TTF_Font* font;

i8 getRandom(int min, int max) {
    return rand()%(max-min + 1)+min;
}

i32 score = 0;

char scoreString[200];
//sprintf(scoreString, "%d", score);

i8 fasterTimer = 0;

SDL_Color backgroundColor = {  0,  0,  0};
SDL_Color frontColor = {255,0,0};

struct Color {i32 r; i32 g; i32 b; i32 a;};

struct Color diyBackgroundColor = {
//  R     G     B     A (Alpha)
    0x00, 0x00, 0x00, 0xFF,
};

struct Color textColor = {
//  R     G     B     A (Alpha)
    0xFF, 0xFF, 0xFF, 0xFF,
};

const struct Color BASE_COLORS[] = {
    { 0x28, 0x28, 0x28, 0xFF }, 
    { 0x2D, 0x99, 0x99, 0xFF },
    { 0x99, 0x99, 0x2D, 0xFF },
    { 0x99, 0x2D, 0x99, 0xFF },
    { 0x2D, 0x99, 0x51, 0xFF },
    { 0x99, 0x2D, 0x2D, 0xFF },
    { 0x2D, 0x63, 0x99, 0xFF },
    { 0x99, 0x63, 0x2D, 0xFF }
};

const struct Color LIGHT_COLORS[] = {
    { 0x28, 0x28, 0x28, 0xFF },
    { 0x44, 0xE5, 0xE5, 0xFF },
    { 0xE5, 0xE5, 0x44, 0xFF },
    { 0xE5, 0x44, 0xE5, 0xFF },
    { 0x44, 0xE5, 0x7A, 0xFF },
    { 0xE5, 0x44, 0x44, 0xFF },
    { 0x44, 0x95, 0xE5, 0xFF },
    { 0xE5, 0x95, 0x44, 0xFF }
};

const struct Color DARK_COLORS[] = {
    { 0x28, 0x28, 0x28, 0xFF },
    { 0x1E, 0x66, 0x66, 0xFF },
    { 0x66, 0x66, 0x1E, 0xFF },
    { 0x66, 0x1E, 0x66, 0xFF },
    { 0x1E, 0x66, 0x36, 0xFF },
    { 0x66, 0x1E, 0x1E, 0xFF },
    { 0x1E, 0x42, 0x66, 0xFF },
    { 0x66, 0x42, 0x1E, 0xFF }
};

void drawRect(i32 sx, i32 sy, i32 sw, i32 sh, struct Color color, i8 fill){
    SDL_Rect rect = {0};
    rect.x = sx;
    rect.y = sy;
    rect.w = sw;
    rect.h = sh;
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    if (fill) {
        SDL_RenderFillRect(renderer, &rect);
    } else {
        SDL_RenderDrawRect(renderer, &rect);
    }
}

void drawBackground(struct Color color) {
    drawRect(0,0,SWIDTH,SHEIGHT,color,1);
};

void drawText(const char *text,i32 sx, i32 sy,i8 alignment,struct Color color){
    SDL_Color sdl_color = { color.r, color.g, color.b, color.a };
    SDL_Surface *surface = TTF_RenderText_Solid(font, text, sdl_color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect rect;

    switch (alignment) {
    case -1: //Align left
        rect.x = sx;
        break;
    case 0: //Align center
        rect.x = sx - surface->w / 2;
        break;
    case 1: //Align right
        rect.x = sx - surface->w;
        break;
    }
    rect.y = sy;
    rect.w = surface->w;
    rect.h = surface->h;

    SDL_RenderCopy(renderer, texture, 0, &rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void drawBlock(i8 x, i8 y, i8 color){
    struct Color base_color = BASE_COLORS[color];
    struct Color light_color = LIGHT_COLORS[color];
    struct Color dark_color = DARK_COLORS[color];

    i32 borderThickness = SBLOCKWIDTH / 10;

    i32 sx = x * SBLOCKWIDTH + OFFSETSX;
    i32 sy = y * SBLOCKWIDTH + OFFSETSY;

    //drawRect(x, y, SBLOCKWIDTH, SBLOCKWIDTH, base_color, 1);
    drawRect(sx, sy, SBLOCKWIDTH, SBLOCKWIDTH, dark_color,1);
    drawRect(sx + borderThickness, sy, SBLOCKWIDTH - borderThickness, SBLOCKWIDTH - borderThickness, light_color,1);
    drawRect(sx + borderThickness, sy + borderThickness, SBLOCKWIDTH - borderThickness * 2, SBLOCKWIDTH - borderThickness * 2, base_color,1);
}

int field[WIDTH*HEIGHT] = {
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0
};

Uint32 generateUserEvent(Uint32 interval, void *param) {
    SDL_Event event;
    SDL_UserEvent userevent;

    userevent.type = SDL_USEREVENT;
    userevent.code = 0;
    userevent.data1 = NULL;
    userevent.data2 = NULL;

    event.type = SDL_USEREVENT;
    event.user = userevent;

    SDL_PushEvent(&event);
    return(interval);
}

i8 getBlock(i8 x, i8 y) {
    if (x < 0 || x >= WIDTH || y >= HEIGHT) {return -1;}
    else if (y<0) {return -2;}
    else {return field[y*WIDTH + x];}
}

const i8 tForms[7][4][16] = {
    { //0. tetrino: I
        {0, 0, 0, 0,
         0, 0, 0, 0,
         1, 1, 1, 1,
         0, 0, 0, 0},

        {0, 0, 1, 0,
         0, 0, 1, 0,
         0, 0, 1, 0,
         0, 0, 1, 0},
        
        {0, 0, 0, 0,
         0, 0, 0, 0,
         1, 1, 1, 1,
         0, 0, 0, 0},

        {0, 0, 1, 0,
         0, 0, 1, 0,
         0, 0, 1, 0,
         0, 0, 1, 0}
    },
    { //1. tetrino: O
        {1, 1,
         1, 1,},

        {1, 1,
         1, 1,},

        {1, 1,
         1, 1,},
        
        {1, 1,
         1, 1,},
    },
    { //2. tetrino: T
        {0, 0, 0,
         1, 1, 1,
         0, 1, 0},
    
        {0, 1, 0,
         1, 1, 0,
         0, 1, 0},
        
        {0, 1, 0,
         1, 1, 1,
         0, 0, 0},
    
        {0, 1, 0,
         0, 1, 1,
         0, 1, 0}
    },
    { //3. tetrion: S
        {0, 0, 0,
         0, 1, 1,
         1, 1, 0},

        {0, 1, 0,
         0, 1, 1,
         0, 0, 1},

        {0, 0, 0,
         0, 1, 1,
         1, 1, 0},

        {0, 1, 0,
         0, 1, 1,
         0, 0, 1}
    },
    { //4. tetrino: Z
        {0, 0, 0,
         1, 1, 0,
         0, 1, 1},

        {0, 0, 1,
         0, 1, 1,
         0, 1, 0},

        {0, 0, 0,
         1, 1, 0,
         0, 1, 1},

        {0, 0, 1,
         0, 1, 1,
         0, 1, 0}
    },
    { //5. tetrino: J
        {0, 0, 0,
         1, 1, 1,
         0, 0, 1},

        {0, 1, 0,
         0, 1, 0,
         1, 1, 0},

        {1, 0, 0,
         1, 1, 1,
         0, 0, 0},

        {0, 1, 1,
         0, 1, 0,
         0, 1, 0},
    },
    { //6. tetrino: L
        {0, 0, 0,
         1, 1, 1,
         1, 0, 0},

        {1, 1, 0,
         0, 1, 0,
         0, 1, 0},

        {0, 0, 1,
         1, 1, 1,
         0, 0, 0},

        {0, 1, 0,
         0, 1, 0,
         0, 1, 1}
    }
};

const i8 tFormWidth[7] = {4,2,3,3,3,3,3};

void newTetrino() {
    tColor = getRandom(1,7);
    tRot = 0; //getRandom(0,3);
    tForm = getRandom(0,6);
    if (tForm == 0) {
        tPosX = tSpawnPosX-2;
    } else if (tForm == 1) {
        tPosX = tSpawnPosX+1;
    } else {
        tPosX = tSpawnPosX;
    }
    tPosY = tSpawnPosY;
}

void drawTetrino() {
    for (i8 y = 0; y<tFormWidth[tForm]; y++) {
        for (i8 x = 0; x<tFormWidth[tForm]; x++) {
            if (tForms[tForm][tRot][y * tFormWidth[tForm] + x] != 0) {
                drawBlock(tPosX+x,tPosY+y,tForms[tForm][tRot][y * tFormWidth[tForm] + x] * tColor);
            }
        }
    }
}

i8 collide(i8 tXTemp, i8 tYTemp, i8 tFormTemp, i8 tRotTemp) {
    //returns 1 if tetrino collides, 0 if it doesn't collide
    i8 doescollide = 0;

    for (i8 y = 0; y<tFormWidth[tFormTemp]; y++) {
        for (i8 x = 0; x<tFormWidth[tFormTemp];x++) {
            if (tForms[tFormTemp][tRotTemp][y * tFormWidth[tFormTemp] + x] == 1 && tPosY>=0 && getBlock(tXTemp+x,tYTemp+y)!=0) {
                return 1;
            }
        }
    }
    return 0;
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    // returns zero on success else non-zero
    if (SDL_Init(SDL_INIT_EVENTS | SDL_INIT_TIMER | SDL_INIT_VIDEO) != 0) {printf("error initializing SDL: %s\n", SDL_GetError());}
    TTF_Init();
    font = TTF_OpenFont("./font/Sans.ttf", 24);

    window = SDL_CreateWindow("Tetris", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,SWIDTH, SHEIGHT,0 );
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    fallTimer = SDL_AddTimer(fallSpeed, generateUserEvent, NULL);
    //SDL_RemoveTimer(fallTimer);
    //fallTimer = SDL_AddTimer(fasterFallSpeed, generateUserEvent, NULL);

    //disable cursor
    //SDL_ShowCursor(SDL_DISABLE);

    newTetrino();

    int close = 0;
    while (!close) {
        SDL_Event event;
 
        // Event management
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {close = 1;}
            if (event.type == SDL_KEYDOWN) {

                switch (event.key.keysym.scancode) {
                case SDL_SCANCODE_W:
                case SDL_SCANCODE_UP:
                    //Rotate tetrino
                    if (!collide(tPosX, tPosY, tForm, tRot+1)) {
                        tRot++;
                        if (tRot>3) {tRot = 0;}
                    }
                    break;
                case SDL_SCANCODE_S:
                case SDL_SCANCODE_DOWN:
                    //Move tetrino faster down
                    if (fasterTimer == 0) {
                        fasterTimer = 1;
                        SDL_RemoveTimer(fallTimer);
                        fallTimer = SDL_AddTimer(fasterFallSpeed, generateUserEvent, NULL);
                    }
                    break;
                case SDL_SCANCODE_A:
                case SDL_SCANCODE_LEFT:
                    //Move tetrino left
                    if (!collide(tPosX-1, tPosY, tForm, tRot)) {
                        tPosX -= 1;
                    }
                    break;
                case SDL_SCANCODE_D:
                case SDL_SCANCODE_RIGHT:
                    //Move tetrino right
                    if (!collide(tPosX+1, tPosY, tForm, tRot)) {
                        tPosX += 1;
                    }
                    break;
                }
            } else if (event.type == SDL_KEYUP) { 
                switch (event.key.keysym.scancode) {
                case SDL_SCANCODE_W:
                case SDL_SCANCODE_UP:
                    break;
                case SDL_SCANCODE_A:
                case SDL_SCANCODE_LEFT:
                    break;
                case SDL_SCANCODE_S:
                case SDL_SCANCODE_DOWN:
                    if (fasterTimer == 1) {
                        fasterTimer = 0;
                        SDL_RemoveTimer(fallTimer);
                        fallTimer = SDL_AddTimer(fallSpeed, generateUserEvent, NULL);
                    }
                    break;
                case SDL_SCANCODE_D:
                case SDL_SCANCODE_RIGHT:
                    break;
                }
            } else if (event.type == SDL_USEREVENT) {

                if (collide(tPosX, tPosY+1, tForm, tRot)) {
                    //Put tetrino into field as solid part
                    for (i8 y = 0; y<tFormWidth[tForm]; y++) {
                        for (i8 x = 0; x<tFormWidth[tForm]; x++) {
                            if (tForms[tForm][tRot][y * tFormWidth[tForm] + x] != 0) {
                                field[(tPosY+y) *WIDTH + (tPosX+x)] = tColor;
                            }
                        }
                    }
                    
                    //Delete line if line is complete (check every row)
                    for (i8 y = 0; y < HEIGHT; y++) {
                        i8 lineComplete = 1;
                        for (i8 x = 0; x<WIDTH; x++) {
                           if (getBlock(x,y)==0) {lineComplete = 0;} //line isnt complete
                        }

                        if (lineComplete) { 
                            for (i8 yToShift = y; yToShift>0; yToShift--) {
                                for (i8 x = 0; x<WIDTH; x++) {
                                    field[yToShift*WIDTH + x] = field[(yToShift-1)*WIDTH + x];
                                }
                            }
                            for (i8 x = 0; x<WIDTH; x++) {
                                field[0*WIDTH + x] = 0;
                            }
                            score += 10;
                            //itoa(score, scoreString, 20);
                            //snprintf(scoreString,20, "%d", score);
                            SDL_RemoveTimer(fallTimer);
                            if (fallSpeed>50) {
                                fallSpeed -= 10;
                                if (fasterFallSpeed>20) {fallSpeed -=10;}
                            }
                            fallTimer = SDL_AddTimer(fallSpeed, generateUserEvent, NULL);
                        }
                    }

                    newTetrino();
                    if (collide(tPosX, tPosY, tForm, tRot)) {
                        close = 1;
                        printf("Score: %d\n", score);
                    } //Game Over!
                } else {
                    tPosY += 1;
                }
            }
        }

        SDL_RenderClear(renderer); //make screen black
        drawBackground(diyBackgroundColor);

        
        for (int y = 0; y<HEIGHT; y++) {
            for (int x = 0; x<WIDTH; x++) {
                drawBlock(x,y,getBlock(x,y)); //field blocks
            }
        }
        
        drawTetrino();

        //drawText(scoreString,SWIDTH-100, 40,-1,textColor);

        SDL_RenderPresent(renderer); // triggers the double buffers for multiple rendering
        SDL_Delay(1000 / maxFPS); // calculates to maxFPS
    }
 
    //tidy up
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
 
    return 0;
}

/*
u8 getRotTetrino(const struct TetrinoType *tetrinoType, u8 x, u8 y, u8 rot){
    u8 w = tetrinoType->w; //width
    switch (rot) {
    case 0: return tetrinoType->data[(y    ) * w +  x];
    case 1: return tetrinoType->data[(w-x-1) * w +  y];
    case 2: return tetrinoType->data[(w-y-1) * w + (w-x-1)];
    case 3: return tetrinoType->data[(x    ) * w + (w-y-1)];
    }
    return 0;
}
*/