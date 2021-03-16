#pragma once
#include "basic_init.h"
#include "character.h"


class Game {
public:
    Game();
    ~Game();

    void run();

    bool loadBackground();
    bool loadCharacter();

    void free();

private:
    //SDL_Window* window = NULL;
    SDL_Renderer* render = NULL;

    SDL_Event e;
    
    myCharacter* wizard = NULL;
};