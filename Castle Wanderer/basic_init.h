#ifndef BASIC_INIT_H
#define BASIC_INIT_H

#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

const std::string title = "Castle Wanderer";
const int SCREEN_HEIGHT = 700;
const int SCREEN_WIDTH = 1280;

const std::string fontName = "fonts/MaliMedium.ttf";
const int fontSize = 20;
const Uint32 textWrapLength = 250;
const int timeToReadOneCharacter = 45;

const SDL_Color white = { 255,255,255 };
const SDL_Color black = { 0, 0, 0 };

const int approxDistant = 20;

//init
SDL_Renderer* initSDL();
TTF_Font* initTTF();

SDL_Texture* loadFromImage(std::string path);
SDL_Texture* loadFromText(std::string textureText, SDL_Rect* renderText, SDL_Color textColor, Uint32 textWrapLength = 0);
Mix_Music* loadFromMusic(std::string path);

void logError(std::ostream& out, const std::string& ms, bool fatal);

void freeTexture(SDL_Texture* texture);
void freeMusic(Mix_Music* music);

void close();

#endif
