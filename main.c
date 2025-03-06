#include <stdio.h>
#include <SDL.h>
#include <math.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define COLOR_WHITE 0xffffffff
#define COLOR_TONED_YELLOW 0xFFFFE600






struct Circle {
    double x;
    double y;
    double radius;
};

// to draw a circle
// we pass arguments as follows: surface for the screen we are going to draw on
// struct circle for all values of circle (x,y,r)
// and finally color to color the drawn pixels
// all calculations are done without sqrt just to save computing power and since the exact values arent important
// we could do it by calculating the actual distance using sqrt on the values.
// we can just compare the value of distance to radius when both are in power of 2
// for loops iterate through "imaginary" rectangle around the circle just so we dont have to check the whole screen
// finally if distance is smaller than radius, we fill that pixel
void drawCircle(SDL_Surface *surface, struct Circle circle, Uint32 color) {
    double radius_squared = pow(circle.radius, 2);
    for (double x=circle.x-circle.radius; x<=circle.x+circle.radius; x++) {
        for (double y=circle.y-circle.radius; y<=circle.y+circle.radius; y++) {
            double distance_squared = pow(x-circle.x,2) + pow(y-circle.y,2);
            if (distance_squared < radius_squared) {
                SDL_Rect drawn_pixel = (SDL_Rect) {x,y,1,1};
                SDL_FillRect(surface, &drawn_pixel, color);
            }
        }
    }
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

    struct Circle light_source = {200, 200, 50};
    struct Circle our_object = {900, 350, 50};

    drawCircle(screen, light_source, COLOR_TONED_YELLOW);
    drawCircle(screen, our_object, COLOR_WHITE);
    SDL_UpdateWindowSurface(window);

    SDL_Delay(5000); //just for testing the screen to be up
    return 0;
}