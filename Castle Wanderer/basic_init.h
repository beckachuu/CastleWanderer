#ifndef BASIC_INIT_H
#define BASIC_INIT_H

#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_image.h>

const std::string title = "Castle Wanderer";
const int SCREEN_HEIGHT = 700;
const int SCREEN_WIDTH = 1500;


//init
SDL_Renderer* initSDL();
void logError(std::ostream& out, const std::string& ms, bool fatal);

void waitUntilKeyPressed();

void close();
SDL_Texture* loadTexture(std::string path);



#endif
