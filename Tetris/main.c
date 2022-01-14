#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#include <SDL2/SDL.h> // for initializing and shutdown functions
#include <SDL2/SDL_image.h> // for rendering images and graphics on screen
#include <SDL2/SDL_timer.h> // for using SDL_Delay() functions
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

typedef uint8_t u8;  //mostly used for ingame coords
typedef int32_t s32; //mostly used for screen coords

u8 tetrinoColor = 1; //has to be smaller than 8
u8 backgroundBlockColor = 0;

const u8 defaultXPos = WIDTH/2-1; //in which x-coordinate above the game spawns

s32 fallSpeed = 150; 
// if key s is not pressed: in double milliseconds (lower is faster)
// if key s is     pressed: in        milliseconds (lower is faster)

u8 wPressed = 0; //if piece should rotate
u8 sPressed = 0; //if piece should go down faster
u8 aPressed = 0; //if piece should go left
u8 dPressed = 0; //if piece should go right

SDL_TimerID fallTimer;
SDL_TimerID fasterFallTimer; //if key s is pressed
SDL_Window* window;
SDL_Renderer* renderer;
SDL_Color black = {  0,  0,  0};
SDL_Color white = {255,255,255};
TTF_Font* font;

struct TetrinoType {
    const u8 *data;
    const u8 w; //width
};

const u8 TETRINO_1[] = {
    0, 0, 0, 0,
    1, 1, 1, 1,
    0, 0, 0, 0,
    0, 0, 0, 0
};
const u8 TETRINO_2[] = {
    2, 2,
    2, 2
};
const u8 TETRINO_3[] = {
    0, 0, 0,
    3, 3, 3,
    0, 3, 0
};
const u8 TETRINO_4[] = {
    0, 4, 4,
    4, 4, 0,
    0, 0, 0
};
const u8 TETRINO_5[] = {
    5, 5, 0,
    0, 5, 5,
    0, 0, 0
};
const u8 TETRINO_6[] = {
    6, 0, 0,
    6, 6, 6,
    0, 0, 0
};
const u8 TETRINO_7[] = {
    0, 0, 7,
    7, 7, 7,
    0, 0, 0
};

const struct TetrinoType TETRINOS[] = {
    {TETRINO_1, 4},
    {TETRINO_2, 2},
    {TETRINO_3, 3},
    {TETRINO_4, 3},
    {TETRINO_5, 3},
    {TETRINO_6, 3},
    {TETRINO_7, 3}
};

struct Color {u8 r; u8 g; u8 b; u8 a;};

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

struct Tetrino {
    u8 type; //type of block, defined as the index in the TETRINOS-list
    u8 offsetX;
    u8 offsetY;
    u8 rot;
};

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

void drawRect(s32 sx, s32 sy, s32 sw, s32 sh, struct Color color, u8 fill){
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

void drawText(const char *text,s32 sx, s32 sy,u8 alignment,struct Color color){
    SDL_Color sdl_color = { color.r, color.g, color.b, color.a };
    SDL_Surface *surface = TTF_RenderText_Solid(font, text, sdl_color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect rect;

    switch (alignment) {
    case 0: //Align left
        rect.x = sx;
        break;
    case 1: //Align center
        rect.x = sx - surface->w / 2;
        break;
    case 2: //Align right
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

void drawBlock(u8 x, u8 y, u8 color){
    struct Color base_color = BASE_COLORS[color];
    struct Color light_color = LIGHT_COLORS[color];
    struct Color dark_color = DARK_COLORS[color];

    s32 borderThickness = SBLOCKWIDTH / 10;

    s32 sx = x * SBLOCKWIDTH + OFFSETSX;
    s32 sy = y * SBLOCKWIDTH + OFFSETSY;

    //drawRect(x, y, SBLOCKWIDTH, SBLOCKWIDTH, base_color, 1);
    drawRect(sx, sy, SBLOCKWIDTH, SBLOCKWIDTH, dark_color,1);
    drawRect(sx + borderThickness, sy, SBLOCKWIDTH - borderThickness, SBLOCKWIDTH - borderThickness, light_color,1);
    drawRect(sx + borderThickness, sy + borderThickness, SBLOCKWIDTH - borderThickness * 2, SBLOCKWIDTH - borderThickness * 2, base_color,1);
}

void drawTetrino(const struct Tetrino *tetrino, u8 outline){
    const struct TetrinoType *tetrinoType = TETRINOS + tetrino->type;
    for (u8 x = 0; x < tetrinoType->w; x++){
        for (u8 y = 0; y < tetrinoType->w; y++){
            u8 value = getRotTetrino(tetrinoType, x, y, tetrino->rot);
            if (value){
                drawBlock(x + tetrino->offsetX, y + tetrino->offsetY, value);
            }
        }
    }
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

SDL_Color backgroundColor = {  0,  0,  0};
SDL_Color frontColor = {255,0,0};

u8 xPos = defaultXPos;
u8 yPos = 0;

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

int getBlock(u8 x, u8 y) {
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {return -1;}
    else {return field[y*WIDTH + x];}
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

    SDL_ShowCursor(SDL_DISABLE);


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
                    if (getBlock(xPos, yPos+1)==0) {yPos += 1;}
                    break;
                case SDL_SCANCODE_A:
                case SDL_SCANCODE_LEFT:
                    aPressed = 1;
                    if (getBlock(xPos-1, yPos)==0) {xPos -= 1;}
                    break;
                case SDL_SCANCODE_D:
                case SDL_SCANCODE_RIGHT:
                    dPressed = 1;
                    if (getBlock(xPos+1,yPos)==0) {xPos += 1;}
                    break;
                }
            } else if (event.type == SDL_KEYUP) { 
                switch (event.key.keysym.scancode) {
                case SDL_SCANCODE_W:
                case SDL_SCANCODE_UP:
                    wPressed = -1;
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

                u8 collide = 0;

                if (yPos + 1 == HEIGHT) {collide = 1;}
                else {
                    for (int x = 0; x<WIDTH; x++) {
                        if (field[(yPos + 1)*WIDTH + x]!=0 && xPos == x) {collide = 1;}
                    }
                }

                if (collide) {
                    field[(yPos) *WIDTH + xPos] = tetrinoColor;
                    int lineComplete = 1;
                    for (int x = 0; x<WIDTH; x++) {
                        if (getBlock(x,yPos)==0) {lineComplete = 0;} //line isnt complete
                    }
                    if (lineComplete) { //delete line
                        for (int y = yPos; y>0; y--) {
                            for (int x = 0; x<WIDTH; x++) {
                                field[y*WIDTH + x] = field[(y-1)*WIDTH + x];
                            }
                        }
                        for (int x = 0; x<WIDTH; x++) {
                            field[0*WIDTH + x] = 0;
                        }
                    }
                    tetrinoColor = rand()%7;
                    xPos = defaultXPos;
                    yPos = 0;
                } else {
                    yPos += 1;
                }
            }
        }

        SDL_RenderClear(renderer); //make screen black

        
        for (int y = 0; y<HEIGHT; y++) {
            for (int x = 0; x<WIDTH; x++) {
                drawBlock(x,y,getBlock(x,y)); 
            }
        }

        drawBlock(xPos,yPos,tetrinoColor);
        SDL_RenderPresent(renderer); // triggers the double buffers for multiple rendering
        SDL_Delay(1000 / maxFPS); // calculates to maxFPS
    }
 
    //tidy up
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
 
    return 0;
}