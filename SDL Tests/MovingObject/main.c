
#include <SDL2/SDL.h> // for initializing and shutdown functions
#include <SDL2/SDL_image.h> // for rendering images and graphics on screen
#include <SDL2/SDL_timer.h> // for using SDL_Delay() functions

#define WIDTH 600
#define HEIGHT 800
#define maxFPS 30



typedef struct Player {
    double x, y, vx, vy, w, h, a; //a = accerelation
} Player;

 
int main(int argc, char *argv[]) {
    // returns zero on success else non-zero
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {printf("error initializing SDL: %s\n", SDL_GetError());}

    // creates a window
    SDL_Window* window = SDL_CreateWindow("GAME", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,WIDTH, HEIGHT, 0);
 
    // creates a renderer to render our images
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
 
    // creates a surface to load an image into the main memory
    SDL_Surface* surface = IMG_Load("./res/ghost.gif");
 
    // loads image to our graphics hardware memory.
    SDL_Texture* pic = SDL_CreateTextureFromSurface(renderer, surface);
 
    // clears main-memory
    SDL_FreeSurface(surface);
 
    //create our player
    Player player;
    player.x = WIDTH/2;
    player.y = HEIGHT/2;
    player.vx = 0;
    player.vy = 0;
    player.w = 50;
    player.h = 50;
    player.a = 0.5;

    // let us control our image position
    // so that we can move it with our keyboard.
    SDL_Rect playerRect;

    // connects our texture with dot to control position & set some standard variables
    SDL_QueryTexture(pic, NULL, NULL, &playerRect.w, &playerRect.h);
    playerRect.w = player.w;
    playerRect.h = player.h;
    playerRect.x = player.x;
    playerRect.y = player.y;
 
    // controls annimation loop
    int close = 0;

    int up = 0;
    int down = 0;
    int left = 0;
    int right = 0;
 
    // annimation loop
    while (!close) {
        SDL_Event event;
 
        // Events management
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                close = 1;
                break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.scancode) {
                case SDL_SCANCODE_A:
                case SDL_SCANCODE_LEFT:
                    left = 1;
                    break;
                case SDL_SCANCODE_D:
                case SDL_SCANCODE_RIGHT:
                    right = 1;
                    break;
                case SDL_SCANCODE_W:
                case SDL_SCANCODE_UP:
                    up = 1;
                    break;
                case SDL_SCANCODE_S:
                case SDL_SCANCODE_DOWN:
                    down = 1;
                    break;
                case SDL_SCANCODE_ESCAPE:
                    close = 1;
                    break;
                case SDL_SCANCODE_UNKNOWN:
                    break;
                }
                break;
            case SDL_KEYUP:
                switch(event.key.keysym.scancode) {
                case SDL_SCANCODE_A:
                case SDL_SCANCODE_LEFT:
                    left = 0;
                    break;
                case SDL_SCANCODE_D:
                case SDL_SCANCODE_RIGHT:
                    right = 0;
                    break;
                case SDL_SCANCODE_W:
                case SDL_SCANCODE_UP:
                    up = 0;
                    break;
                case SDL_SCANCODE_S:
                case SDL_SCANCODE_DOWN:
                    down = 0;
                    break;
                }
                break;
            }
        }

        if ( up   && !down ) {player.vy -= player.a;}
        if (!up   &&  down ) {player.vy += player.a;}
        if ( left && !right) {player.vx -= player.a;}
        if (!left &&  right) {player.vx += player.a;}

        if (0 < player.x+player.vx && player.x+player.w+player.vx < WIDTH ) {player.x += player.vx;}
        else {player.vx *= -1;}
        if (0 < player.y+player.vy && player.y+player.h+player.vy < HEIGHT) {player.y += player.vy;}
        else {player.vy *= -1;}

        playerRect.x = player.x;
        playerRect.y = player.y;

        player.vx *= 0.99;
        player.vy *= 0.99;
 
        // clears the screen
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, pic, NULL, &playerRect);
 
        SDL_RenderPresent(renderer); // triggers the double buffers for multiple rendering
        SDL_Delay(1000 / maxFPS); // calculates to 60 fps
    }
 
    
    SDL_DestroyTexture(pic); // destroy texture
    SDL_DestroyRenderer(renderer); // destroy renderer
    SDL_DestroyWindow(window); // destroy window
    SDL_Quit(); // close SDL
 
    return 0;
}