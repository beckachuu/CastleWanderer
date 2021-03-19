#include "game.h"
#include "character.h"

Game::Game()
{
    std::cout << "init game...\n";
}

Game::~Game()
{
    std::cout << "finished. :>";
}

void Game::run() {
    SDL_Renderer* render = initSDL();
    SDL_Texture* background = loadTexture("image/jungle background.png");

    myCharacter wizard;
    wizard.setSpriteClips();

    //The application timer
    Timer timer;
    timer.start();

    if (!wizard.loadFromFile("image/wizardSheet.png", render)) {
        std::cerr << "Load wizard sheet error!\n";
        return;
    }

    if (render == nullptr) {
        std::cerr << "Failed to initialize!\n";
    }
    else {
        bool quit = false;
        while (!quit) {

            SDL_RenderClear(render);
            SDL_RenderCopy(render, background, NULL, NULL);

            while (SDL_PollEvent(&e) != 0) {

                if (e.type == SDL_KEYDOWN) {
                    switch (e.key.keysym.sym) {
                    case SDLK_ESCAPE:                        
                        close();
                        return;
                    }
                }
                else if (e.type == SDL_QUIT) {
                    quit = true;
                }

                wizard.handleEvent(e);
            }

            wizard.renderCurrentAction(render, e);
            SDL_RenderPresent(render);
        }
    }

    std::cout << "SDL_GetTicks(): " << SDL_GetTicks() << std::endl;
    wizard.free();
}