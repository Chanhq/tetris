#include <SDL2/SDL.h> // for initializing and shutdown functions
#include <SDL2/SDL_image.h> // for rendering images and graphics on screen
#include <SDL2/SDL_timer.h> // for using SDL_Delay() functions
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>

int width = 600;
int height = 800;

#define maxFPS 30

int r = 0;
int g = 0;
int b = 0;

int h = 0;
int s = 100;
int v = 100;

char rString[3]; 
char gString[3]; 
char bString[3];
char label[11]; 

SDL_Color black = {  0,  0,  0};
SDL_Color white = {255,255,255};

SDL_Renderer* renderer;
TTF_Font*     pixelFont;
SDL_Window*   window;
SDL_Surface*  labelSurface;
SDL_Surface*  rectSurface;

SDL_Rect windowRect;
SDL_Rect labelRect;

void HSVtoRGB(float H, float S,float V, int *rOut, int *gOut, int *bOut){
    if(H>360 || H<0 || S>100 || S<0 || V>100 || V<0){
        printf("The givem HSV values are not in valid range\n");
        return;
    }
    float s = S/100;
    float v = V/100;
    float C = s*v;
    float X = C*(1-fabs(fmod(H/60.0, 2)-1));
    float m = v-C;
    float r,g,b;
    if(H >= 0 && H < 60){r = C,g = X,b = 0;}
    else if(H >=  60 && H < 120){r = X,g = C,b = 0;}
    else if(H >= 120 && H < 180){r = 0,g = C,b = X;}
    else if(H >= 180 && H < 240){r = 0,g = X,b = C;}
    else if(H >= 240 && H < 300){r = X,g = 0,b = C;}
    else{r = C,g = 0,b = X;}

    *rOut = (r+m)*255;
    *gOut = (g+m)*255;
    *bOut = (b+m)*255;
}

int main(int argc, char *argv[]) {
    // returns zero on success else non-zero
    if (SDL_Init(SDL_INIT_EVENTS | SDL_INIT_TIMER | SDL_INIT_VIDEO) != 0) {printf("error initializing SDL: %s\n", SDL_GetError());}
    if (!TTF_Init()) {printf("Unable to initialize SDL_ttf: %s \n", TTF_GetError());}
    pixelFont = TTF_OpenFont("./font/PixelFJVerdana12pt", 24);

    window = SDL_CreateWindow("GAME", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,width, height,0 ); // SDL_WINDOW_FULLSCREEN_DESKTOP);
    SDL_GetWindowSize(window, &width, &height);
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    rectSurface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
    printf("Before rendertextsolid\n");
    labelSurface = TTF_RenderText_Shaded(pixelFont, "placeholer", black, white); 
    printf("After rendertextsolid\n");

    sprintf(rString, "%d", r);
    sprintf(gString, "%d", g);
    sprintf(bString, "%d", b);

    SDL_ShowCursor(SDL_DISABLE);

    windowRect.x = 0;
    windowRect.y = 0;
    windowRect.w = width;
    windowRect.h = height;

    labelRect.w = width/10;
    labelRect.h = height/10;
    labelRect.x = width/2-labelRect.w;
    labelRect.y = height/2-labelRect.h;

    // controls annimation loop
    int close = 0;

    // annimation loop
    while (!close) {
        h++;
        if(h>360) {h=0;}

        SDL_Event event;
 
        // Events management
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {close = 1;}
            else if (event.type == SDL_KEYDOWN) {close = 1;}
        }

        HSVtoRGB(h,s,v,&r,&g,&b);
        SDL_FillRect(rectSurface, NULL, SDL_MapRGB(rectSurface->format, r, g, b));
        labelSurface = TTF_RenderText_Solid(pixelFont, "placeholer", black); 
 
        // clears the screen
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, SDL_CreateTextureFromSurface(renderer, rectSurface), NULL, &windowRect);
        SDL_RenderCopy(renderer, SDL_CreateTextureFromSurface(renderer, labelSurface), NULL, &labelRect);
 
        SDL_RenderPresent(renderer); // triggers the double buffers for multiple rendering
        SDL_Delay(1000 / maxFPS); // calculates to 60 fps
    }
 
    SDL_FreeSurface(rectSurface);
    SDL_FreeSurface(labelSurface);
    TTF_CloseFont(pixelFont);
    SDL_DestroyRenderer(renderer); // destroy renderer
    SDL_DestroyWindow(window); // destroy window
    SDL_Quit(); // close SDL
 
    return 0;
}