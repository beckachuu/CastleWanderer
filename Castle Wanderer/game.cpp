#include "game.h"

Game::Game() {
    render = initSDL();
    if (render == nullptr) {
        std::cerr << "Failed to initialize!\n";
    }
    else run();
}

Game::~Game() {
    free();
}

void Game::free() {
    wizard->free();

    SDL_DestroyRenderer(render);
    render = NULL;

    //Quit SDL subsystems
    IMG_Quit();

    close();
}

void Game::run() {
    bool bg = loadBackground();
    bool ch = loadCharacter();
    if (!bg) {
        std::cerr << "Failed to load media!\n";
    }
    else {
        int frame = 0;

        bool quit = false;

        while (!quit) {
            SDL_RenderClear(render);

            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_KEYDOWN) {
                    switch (e.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        free();
                    }
                }
                else if (e.type == SDL_QUIT) {
                    quit = true;
                }

                wizard->handleEvent(e);
            }

            if (wizard->move() % 70 == 0 && wizard->getcPosX() != 0) {

                frame++;
                if (frame >= 4)
                {
                    frame = 0;
                }
            }
            else if (wizard->getcVelX() == 0) {
                frame = stand;
            }

            wizard->renderCurrentAction(frame, render);
            SDL_RenderPresent(render);
        }
    }
}



bool Game::loadBackground() {
    SDL_Texture* background = loadTexture("image/jungle background.png");
    SDL_RenderClear(render);
    SDL_RenderCopy(render, background, NULL, NULL);
    if (background == NULL || render == NULL) {
        std::cerr << "Load background error! \n";
        return false;
    }
    else return true;
}

bool Game::loadCharacter() {

    if (!wizard->loadFromFile("image/wizardSheet.png", render)) {
        std::cerr << "Load wizard sheet error! \n";
        return false;
    }
    else wizard->setSpriteClips();
    return true;
}