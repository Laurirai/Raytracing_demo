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

struct Ray {
    double x;
    double y;
    double angle;
    double x_start;
    double y_start;
    double x_end;
    double y_end;
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

void generate_rays(struct Circle circle, struct Ray rays[AMOUNT_OF_RAYS], double ray_length) {
    for (int i = 0; i < AMOUNT_OF_RAYS; i++) {
        double angle = (2 * M_PI * i) / AMOUNT_OF_RAYS;  // Convert index to radians

        rays[i].angle = angle;
        rays[i].x_start = circle.x;
        rays[i].y_start = circle.y;

        // Calculate the end point using trigonometry
        rays[i].x_end = circle.x + cos(angle) * ray_length;
        rays[i].y_end = circle.y + sin(angle) * ray_length;
    }
}
// n. 1,06,35
void draw_rays(SDL_Renderer *renderer,struct Circle circle, struct Ray rays[AMOUNT_OF_RAYS]) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);  // Yellow for light

    for (int i = 0; i < AMOUNT_OF_RAYS; i++) {
        SDL_RenderDrawLine(renderer, rays[i].x_start, rays[i].y_start, rays[i].x_end, rays[i].y_end);
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

    struct Circle light_source = {200, 200, 50}; // creating the "light" source circle with the struct
    struct Circle our_object = {900, 350, 100}; // creating our "object" circle with same struct
    struct Ray rays[AMOUNT_OF_RAYS];
    // Creating a rectangle which is the size of the screen. Whilst this rect will fill the screen,
    // it will be "erasing" what's on the screen
    SDL_Rect erasing_rectangle = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
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
            }
        }

        // we will be looping over and over these
        SDL_FillRect(screen, &erasing_rectangle, COLOR_BLACK); // every loop will start on drawing the black screen,
        drawCircle(screen, light_source, COLOR_TONED_YELLOW); // drawing the light source
        drawCircle(screen, our_object, COLOR_WHITE); // drawing the object

        generate_rays(light_source, rays, ray_length);
        draw_rays(renderer, our_object, rays);
        SDL_UpdateWindowSurface(window); // while they've been drawn, they are just being drawn in memory, we have to
        // update the screen in order to see the drawn results
        SDL_Delay(10); // around 100 fps depending on performance

    }
    return 0;
}