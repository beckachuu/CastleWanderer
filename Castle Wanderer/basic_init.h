#ifndef BASIC_INIT_H
#define BASIC_INIT_H


#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <string>
using namespace std;


const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 600;

//The window we'll be rendering to
SDL_Window* window = NULL;

//The surface contained by the window
SDL_Surface* screenSurface = NULL;

//test surface
SDL_Surface* CurrentSurface = NULL;

SDL_Renderer* renderer;

const string WINDOW_TITLE = "Castle Wanderer";



void logSDLError(ostream& os, const string& msg, bool fatal = false);

void initSDL(SDL_Window*& window, SDL_Renderer*& renderer);

void waitUntilKeyPressed();

void quitSDL(SDL_Window* window, SDL_Renderer* renderer);


/// ////////////////////////////////////////////////////////////////////


void logSDLError(ostream& os, const string& msg, bool fatal)
{
    os << msg << " Error: " << SDL_GetError() << endl;
    if (fatal) {
        SDL_Quit();
        exit(1);
    }
}

void initSDL(SDL_Window*& window, SDL_Renderer*& renderer)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        logSDLError(cout, "SDL_Init", true);

    window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED, 
            SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if (window == NULL) {
        logSDLError(cout, "CreateWindow", true);
    }

    //Create renderer for window (chay vs may o nha)
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |
        SDL_RENDERER_PRESENTVSYNC);

    if (renderer == NULL) {
        logSDLError(cout, "CreateRenderer", true);
    }

    //Initialize renderer color
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

    //Set texture filtering to linear
    if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear")) // or "1"
    {
        printf("Warning: Linear texture filtering not enabled!");
    }

    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

    screenSurface = SDL_GetWindowSurface(window);
}

void waitUntilKeyPressed()
{
    SDL_Event e;
    while (true) {
        if (SDL_WaitEvent(&e) != 0 &&
            (e.type == SDL_KEYDOWN || e.type == SDL_QUIT))
            return;
        SDL_Delay(200);
    }
}

void quitSDL(SDL_Window* window, SDL_Renderer* renderer)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    window = NULL;
    renderer = NULL;

    IMG_Quit();
    SDL_Quit();
}

#endif