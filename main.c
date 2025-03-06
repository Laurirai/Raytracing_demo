#include <stdio.h>
#include <SDL.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define COLOR_WHITE 0xffffffff

struct Circle {
    double x;
    double y;
    double radius;
};

void drawCircle(SDL_Surface *surface, struct Circle circle) {
    
}

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window = SDL_CreateWindow( //Creating our "screen"
        "Raytracer Demo", // name in top of the screen
        SDL_WINDOWPOS_CENTERED, // x and y coordinate where the screen will b
        SDL_WINDOWPOS_CENTERED, // but we call it to center on our own screen
        SCREEN_WIDTH, //width
        SCREEN_HEIGHT, // and height of the screen, but we've defined them already
        SDL_WINDOW_SHOWN); // just to enable the screen to show


    SDL_Surface *screen = SDL_GetWindowSurface(window);// retrieving the surface and saving it to a pointer
    SDL_Rect rectangle = (SDL_Rect){200, 200, 100, 100};
    SDL_FillRect(screen, &rectangle, COLOR_WHITE);
    SDL_UpdateWindowSurface(window);

    SDL_Delay(5000); // testing for screen to b up
    return 0; // returning 0
}