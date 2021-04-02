#include "game.h"
#include "character.h"
#include "background.h"
#include "guard.h"
#include <ctime>
#include <SDL_ttf.h>

Game::Game()
{
    std::cout << "Init game...\n";
    gameTime = 0;

    render = initSDL();
    font = initTTF();
}

Game::~Game()
{
    std::cout << "Finished. :>";
}

void Game::run() {
    srand(time(0));

    //Init background
    Background background(render);

    //Init guards
    Guard g(render);
    g.setRightLimit(background.getFurthestLeftPoint());

    //Init character
    myCharacter wizard;
    wizard.setSpriteClips();
    //Load wizard
    if (!wizard.loadFromFile("image/wizardSheet.png", render)) {
        std::cerr << "Load wizard sheet error!\n";
        return;
    }

    if (render == nullptr || font == nullptr) {
        std::cerr << "Failed to initialize!\n";
        return;
    }
    else {
        bool quit = false;
        while (!quit) {
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
                //Control character by keyboard events
                wizard.handleEvent(e, render);
                background.handledEvent(e, render);
            }

            if (SDL_GetTicks() > gameTime + nextFrameTime) {
                wizard.move();
                if (wizard.gotToFar) {
                    background.move();
                    if (!background.toFurthestLeftX() || !background.toFurthestRightX()) {
                        g.setPlusVelocity(background.getBGVelX());
                    }
                }
                if (background.toFurthestLeftX() || background.toFurthestRightX() || !wizard.gotToFar) {
                    g.setPlusVelocity(0);
                }
                g.move();

                SDL_RenderClear(render);

                background.renderCurrentFrame(render);
                g.renderCurrentAction(render);
                wizard.renderCurrentAction(render);

                SDL_RenderPresent(render);

                gameTime = SDL_GetTicks();
            }

        }
    }
}