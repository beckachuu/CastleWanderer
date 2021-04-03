#ifndef BASIC_INIT_H
#define BASIC_INIT_H

#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>

const std::string title = "Castle Wanderer";
const int SCREEN_HEIGHT = 700;
const int SCREEN_WIDTH = 1280;

const int fontSize = 18;
const std::string fontName = "fonts/Inkfree.ttf";

//init
SDL_Renderer* initSDL();
TTF_Font* initTTF();

SDL_Texture* loadFromFile(std::string path, SDL_Renderer* renderer);
SDL_Texture* loadFromRenderedText(std::string textureText, SDL_Rect* renderText, Uint32 textWrapLength);

void logError(std::ostream& out, const std::string& ms, bool fatal);

void freeTexture(SDL_Texture* texture);
void close();

#endif
