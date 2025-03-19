/*
 * MIT License
 *
 * Copyright (c) 2025 Lauri Räisänen
 *
 * Permission is hereby granted, free of charge...
*/


#include <stdio.h>
#include <SDL.h>
#include <math.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define COLOR_WHITE 0xffffffff
#define COLOR_TONED_YELLOW 0xFFFFE600
#define COLOR_BLACK 0x00000000
#define AMOUNT_OF_RAYS 200


// creating a struct for our circles for easier time to pass argument into functions
struct Circle {
    double x;
    double y;
    double radius;
};

// creating a struct for our rays for easier time to pass argument into functions
struct Ray {
    double angle;
    double x_start;
    double y_start;
};

// to draw a circle
// we pass arguments as follows: surface for the screen we are going to draw on
// struct circle for all values of circle (x,y,radius)
// and finally color to color the drawn pixels
// all calculations are done without sqrt just to save computing power and since the exact values arent important
// we could do it by calculating the actual distance using sqrt on the values. (sqrt is very expensive to use)
// we can just compare the value of distance to radius when both are in power of 2
// for loops iterate through "imaginary" rectangle around the circle just so we dont have to check the whole screen
// finally if distance is smaller than radius, we fill that pixel
// also the values could be int's since the pixel's value (coordinate) is always int
// but ive read somewhere that that can cause problems, so we are using doubles instead
// SDL uses coordinates for pixels
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

// we need to generate angles for the rays depending on the position of the circle
// we get the circle as argument to know where it is
// we get the rays as an array with the structure of the rays
// then we loop over the rays in the array
// i / amount of rays to generate the angles between 360
// 2*pi to convert the fraction number to radians. since 360 = 2pi rad
// then we create a new ray using the structure
// and finally store the ray
void generate_rays(struct Circle circle, struct Ray rays[AMOUNT_OF_RAYS]) {
    for (int i = 0; i < AMOUNT_OF_RAYS; i++) {
        double angle = ((double) i / AMOUNT_OF_RAYS) * 2 * M_PI;
        struct Ray ray = { angle, circle.x, circle.y };
        rays[i] = ray;
    }
}

// to draw the actual rays
// we pass surface to get the "surface" we are drawing on
// we pass the array of rays where each ray has starting position and the angle
// and finally the color we want the rays to b
// then we loop over the array
// then we initialize the variables
// end of screen flag to indicate when ray has moved beyond the screen
// hit object flag to check if ray has struck an object
// step is the definition of how much the ray is moving
// 1 step = ray advances 1 pixel at a time
// while loop checks if the ray is passed the screen and if it has touched the object
// then we increment the position
// draw_x_pixel += step*cos(ray.angle); this advances the x coordinate based on cosine of rays angle
// draw_y_pixel += step*sin(ray.angle); this advances the y coordinate based on sine of the rays angle
// then we just draw the given pixel
// then we check if the rays are out of bounds
// since the origin of sdl coordinate is 0,0 and thats at the top left corner
// draw_x_pixel < 0 // this will check the left side of the screen
// draw_x_pixel > SCREEN_WIDTH // this will check the right side of the screen
// same for the y coordinate
void draw_rays(SDL_Surface *surface, struct Ray rays[AMOUNT_OF_RAYS], Uint32 color, struct Circle object_circle) {

    double radius_squared = object_circle.radius * object_circle.radius;
    for (int i = 0; i < AMOUNT_OF_RAYS; i++) {
        struct Ray ray = rays[i];
        int end_of_screen = 0;
        int hit_object = 0;
        double step = 1;
        double draw_x_pixel = ray.x_start;
        double draw_y_pixel = ray.y_start;

        while (!end_of_screen && !hit_object) {
            draw_x_pixel += step*cos(ray.angle);
            draw_y_pixel += step*sin(ray.angle);

            double distance_from_object_center_squared = pow(draw_x_pixel - object_circle.x, 2) + pow(draw_y_pixel - object_circle.y, 2);

            SDL_Rect pixel = (SDL_Rect) {draw_x_pixel,draw_y_pixel,1,1};
            SDL_FillRect(surface, &pixel, color);

            if (draw_x_pixel < 0 || draw_x_pixel > SCREEN_WIDTH) {
                end_of_screen = 1;
            }
            if (draw_y_pixel < 0 || draw_y_pixel > SCREEN_HEIGHT) {
                end_of_screen = 1;
            }
            // to check that the ray has hit an object
            // we calculate the distance from current ray's position to the object's center
            // we calculate the radius of the object
            // and if the distance is smaller than radius, the ray is inside the circle
            // then we set hit_object to 1 to stop drawing the ray any further
            // we calculate in squared to save computing power
            if (distance_from_object_center_squared < radius_squared) {
                hit_object = 1;
            }
        }
    }
}

// n. 1.27.30
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

    struct Circle light_source = {200, 200, 50}; // creating the "light" source circle with the struct
    struct Circle our_object = {900, 350, 100}; // creating our "object" circle with same struct
    struct Ray rays[AMOUNT_OF_RAYS];
    // Creating a rectangle which is the size of the screen. Whilst this rect will fill the screen,
    // it will be "erasing" what's on the screen
    SDL_Rect erasing_rectangle = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

    double ray_length = sqrt(SCREEN_WIDTH * SCREEN_WIDTH + SCREEN_HEIGHT * SCREEN_HEIGHT);

    int is_running = 1;
    SDL_Event event;
    while (is_running) { // this will be the main loop running the program
        while (SDL_PollEvent(&event)) { // this loop whill check on all the events
            if (event.type == SDL_QUIT) { // if the top right x is clicked
                is_running = 0; // we close the program
            }
            // if mouse is moving and the mouse button is pressed
            if (event.type == SDL_MOUSEMOTION && event.motion.state != 0) {
                light_source.x = event.motion.x; // we move the x and y coordinate of the circle according to the mouse's coordinates
                light_source.y = event.motion.y;
                generate_rays(light_source, rays);
            }
        }

        // we will be looping over and over these
        SDL_FillRect(screen, &erasing_rectangle, COLOR_BLACK); // every loop will start on drawing the black screen,
        drawCircle(screen, light_source, COLOR_TONED_YELLOW); // drawing the light source
        drawCircle(screen, our_object, COLOR_WHITE); // drawing the object

        generate_rays(light_source, rays);
        draw_rays(screen, rays, COLOR_TONED_YELLOW, our_object);
        SDL_UpdateWindowSurface(window); // while they've been drawn, they are just being drawn in memory, we have to
        // update the screen in order to see the drawn results
        SDL_Delay(10); // around 100 fps depending on performance

    }
    return 0;
}