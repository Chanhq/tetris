#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h> // for initializing and shutdown functions
#include <SDL2/SDL_image.h> // for rendering images and graphics on screen
#include <SDL2/SDL_timer.h> // for using SDL_Delay() functions
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>
#include <stdbool.h>

#define GRID_SIZE 30
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

typedef uint8_t u8;
typedef int32_t s32;

struct Tetrino {
    const u8 *data;
    const s32 width;
};

static const u8 TETRINO_1[] = {
    0, 0, 0, 0,
    1, 1, 1, 1,
    0, 0, 0, 0,
    0, 0, 0, 0
};
static const u8 TETRINO_2[] = {
    2, 2,
    2, 2
};
static const u8 TETRINO_3[] = {
    0, 0, 0,
    3, 3, 3,
    0, 3, 0
};
static const u8 TETRINO_4[] = {
    0, 4, 4,
    4, 4, 0,
    0, 0, 0
};
static const u8 TETRINO_5[] = {
    5, 5, 0,
    0, 5, 5,
    0, 0, 0
};
static const u8 TETRINO_6[] = {
    6, 0, 0,
    6, 6, 6,
    0, 0, 0
};
static const u8 TETRINO_7[] = {
    0, 0, 7,
    7, 7, 7,
    0, 0, 0
};

static const struct Tetrino TETRINOS[] = {
    {TETRINO_1, 4},
    {TETRINO_2, 2},
    {TETRINO_3, 3},
    {TETRINO_4, 3},
    {TETRINO_5, 3},
    {TETRINO_6, 3},
    {TETRINO_7, 3}
};

struct Piece_State {
    u8 tetrino_index;
    s32 offset_row;
    s32 offset_col;
    s32 rotation;
};

static u8 tetrino_get(const struct Tetrino *tetrino, s32 row, s32 col, s32 rotation){
    s32 width = tetrino->width;
    switch (rotation)
    {
    case 0:
        return tetrino->data[row * width + col];
    case 1:
        return tetrino->data[(width - col - 1) * width + row];
    case 2:
        return tetrino->data[(width - row - 1) * width + (width - col - 1)];
    case 3:
        return tetrino->data[col * width + (width - row - 1)];
    }
    return 0;
}
struct Color
{
    u8 r;
    u8 g;
    u8 b;
    u8 a;
};

static const struct Color BASE_COLORS[] = {
    { 0x28, 0x28, 0x28, 0xFF },
    { 0x2D, 0x99, 0x99, 0xFF },
    { 0x99, 0x99, 0x2D, 0xFF },
    { 0x99, 0x2D, 0x99, 0xFF },
    { 0x2D, 0x99, 0x51, 0xFF },
    { 0x99, 0x2D, 0x2D, 0xFF },
    { 0x2D, 0x63, 0x99, 0xFF },
    { 0x99, 0x63, 0x2D, 0xFF }
};

static const struct Color LIGHT_COLORS[] = {
    { 0x28, 0x28, 0x28, 0xFF },
    { 0x44, 0xE5, 0xE5, 0xFF },
    { 0xE5, 0xE5, 0x44, 0xFF },
    { 0xE5, 0x44, 0xE5, 0xFF },
    { 0x44, 0xE5, 0x7A, 0xFF },
    { 0xE5, 0x44, 0x44, 0xFF },
    { 0x44, 0x95, 0xE5, 0xFF },
    { 0xE5, 0x95, 0x44, 0xFF }
};

static const struct Color DARK_COLORS[] = {
    { 0x28, 0x28, 0x28, 0xFF },
    { 0x1E, 0x66, 0x66, 0xFF },
    { 0x66, 0x66, 0x1E, 0xFF },
    { 0x66, 0x1E, 0x66, 0xFF },
    { 0x1E, 0x66, 0x36, 0xFF },
    { 0x66, 0x1E, 0x1E, 0xFF },
    { 0x1E, 0x42, 0x66, 0xFF },
    { 0x66, 0x42, 0x1E, 0xFF }
};

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



Uint32 my_callbackfunc(Uint32 interval, void *param) {
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

SDL_TimerID fallTimer;

int main(int argc, char *argv[]) {
    // returns zero on success else non-zero
    if (SDL_Init(SDL_INIT_EVENTS | SDL_INIT_TIMER | SDL_INIT_VIDEO) != 0) {printf("error initializing SDL: %s\n", SDL_GetError());}

    SDL_Window* window = SDL_CreateWindow("GAME", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,WIDTH, HEIGHT,0 );
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    fallTimer = SDL_AddTimer(fallSpeed, my_callbackfunc, NULL);

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
                    if (currentPieceRect.y/CELLWIDTH > 0 && field[currentPieceRect.y/CELLWIDTH][currentPieceRect.x/CELLWIDTH-1] == 0) {currentPieceRect.x -= CELLWIDTH;}
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

static void fill_rect(SDL_Renderer *renderer, s32 x, s32 y, s32 width, s32 height, struct Color color){
    SDL_Rect rect = {0};
    rect.x = x;
    rect.y = y;
    rect.w = width;
    rect.h = height;
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect);
}


static void draw_rect(SDL_Renderer *renderer, s32 x, s32 y, s32 width, s32 height, struct Color color){
    SDL_Rect rect = {0};
    rect.x = x;
    rect.y = y;
    rect.w = width;
    rect.h = height;
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawRect(renderer, &rect);
}

static void draw_string(SDL_Renderer *renderer,TTF_Font *font,const char *text,s32 x, s32 y,enum Text_Align alignment,struct Color color){
    SDL_Color sdl_color = { color.r, color.g, color.b, color.a };
    SDL_Surface *surface = TTF_RenderText_Solid(font, text, sdl_color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect rect;
    rect.y = y;
    rect.w = surface->w;
    rect.h = surface->h;

    switch (alignment)
    {
    case TEXT_ALIGN_LEFT:
        rect.x = x;
        break;
    case TEXT_ALIGN_CENTER:
        rect.x = x - surface->w / 2;
        break;
    case TEXT_ALIGN_RIGHT:
        rect.x = x - surface->w;
        break;
    }

    SDL_RenderCopy(renderer, texture, 0, &rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

static void draw_cell(SDL_Renderer *renderer,s32 row, s32 col, u8 value,s32 offset_x, s32 offset_y,bool outline){
        struct Color base_color = BASE_COLORS[value];
        struct Color light_color = LIGHT_COLORS[value];
        struct Color dark_color = DARK_COLORS[value];


        s32 edge = GRID_SIZE / 8;

        s32 x = col * GRID_SIZE + offset_x;
        s32 y = row * GRID_SIZE + offset_y;

    if (outline){
        draw_rect(renderer, x, y, GRID_SIZE, GRID_SIZE, base_color);
        return;
    }

    fill_rect(renderer, x, y, GRID_SIZE, GRID_SIZE, dark_color);
    fill_rect(renderer, x + edge, y,
              GRID_SIZE - edge, GRID_SIZE - edge, light_color);
    fill_rect(renderer, x + edge, y + edge,
              GRID_SIZE - edge * 2, GRID_SIZE - edge * 2, base_color);
}

static void draw_piece(SDL_Renderer *renderer, const struct Piece_State *piece, s32 offset_x, s32 offset_y, bool outline){

        const struct Tetrino *tetrino = TETRINOS + piece->tetrino_index;
        for (s32 row = 0; row < tetrino->width; ++row){
            for (s32 col = 0; col < tetrino->width; ++col){
                u8 value = tetrino_get(tetrino, row, col, piece->rotation);
                if (value){
                    draw_cell(renderer, row + piece->offset_row, col + piece->offset_col, value, offset_x, offset_y, outline);
                }
            }
        }
}

    return 0;
}


