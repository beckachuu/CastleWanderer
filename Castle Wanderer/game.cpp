#include "game.h"
#include <ctime>

Game::Game()
{
    srand(time(0));

    gameTime = 0;

    render = initSDL();
    font = initTTF();

    background = new Background();

    for (int i = 0; i < guardNum; i++) {
        guard[i] = new Guard();
        guard[i]->setRightLimit(background->getFurthestLeftPoint());
    }

    for (int i = 0; i < goblinNum; i++) {
        goblin[i] = new Goblin();
        goblin[i]->setRightLimit(background->getFurthestLeftPoint());
    }

    wizard = new MyCharacter();

    touchingX = false;
    touchingY = false;
}

Game::~Game()
{
    close();
}

void Game::run() {

    wizard->setVelocity(background->getBGspeed());

    if (render == nullptr || font == nullptr) {
        std::cerr << "Failed to initialize!\n";
        return;
    }
    else {
        while (handlingKeyboardEvents()) {

            if (SDL_GetTicks() > gameTime + nextFrameTime) {
                
                moveObjects();

                detectTouchingObjects();
                
                updateScreen();

                gameTime = SDL_GetTicks();
            }
        }
    }
}


bool Game::handlingKeyboardEvents() {
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
            case SDLK_ESCAPE:
                close();
                return false;
                /*case SDLK_UP:
                    background->speedUp();
                    wizard->setVelocity(background->getBGspeed());
                    for (int i = 0; i < guardNum; i++) {
                        if (guard[i] == nullptr) {
                            guard[i] = new Guard(render);
                        }
                        else guard[i]->setVelocity(background->getBGspeed());
                    }
                    break;
                case SDLK_DOWN:
                    background->speedDown();
                    wizard->setVelocity(background->getBGspeed());
                    for (int i = 0; i < guardNum; i++) {
                        if (guard[i] == nullptr) {
                            guard[i] = new Guard(render);
                        }
                        else guard[i]->setVelocity(background->getBGspeed());
                    }
                    break;*/
            case SDLK_f:
                //Init fire attack
                for (int i = 0; i < max_fire_spell; i++)
                {
                    if (fire[i] == nullptr) {
                        fire[i] = new Fire(wizard->isToRight(), wizard->getcharPosX(), wizard->getcharPosY(), background->getBGspeed());
                        break;
                    }
                    else continue;
                }
            }
        }
        else if (e.type == SDL_QUIT) {
            return false;
        }
        //Control character by keyboard events
        wizard->handleEvent(e, render);
        background->handledEvent(e, render);
    }
    return true;
}

void Game::moveObjects() {

    if (background->isAtFurthestLeftX()) {
        wizard->setFurthestPoints(characterFurthestLeft, SCREEN_WIDTH);
    }
    else if (background->isAtFurthestRightX()) {
        wizard->setFurthestPoints(0, characterFurthestRight);
    }
    else {
        wizard->setFurthestPoints(characterFurthestLeft, characterFurthestRight);
    }

    wizard->move();

    if (wizard->isAtEdgeOfScreen()) {
        background->move();
        if (!background->isAtFurthestLeftX() || !background->isAtFurthestRightX()) {
            for (int i = 0; i < guardNum; i++) {
                if (guard[i] == nullptr) {
                    guard[i] = new Guard();
                }
                guard[i]->setPlusVelocity(background->getBGVelX());
            }
            for (int i = 0; i < goblinNum; i++) {
                if (goblin[i] == nullptr) {
                    goblin[i] = new Goblin();
                }
                goblin[i]->setPlusVelocity(background->getBGVelX());
            }
        }
    }
    if (!wizard->isAtEdgeOfScreen() || background->isAtFurthestLeftX() || background->isAtFurthestRightX()) {
        for (int i = 0; i < guardNum; i++) {
            if (guard[i] == nullptr) {
                guard[i] = new Guard();
            }
            guard[i]->setPlusVelocity(0);
        }
        for (int i = 0; i < goblinNum; i++) {
            if (goblin[i] == nullptr) {
                goblin[i] = new Goblin();
            }
            goblin[i]->setPlusVelocity(0);
        }
    }

    for (int i = 0; i < guardNum; i++) {
        if (guard[i] == nullptr) {
            guard[i] = new Guard();
        }
        
        guard[i]->move();
    }
    for (int i = 0; i < goblinNum; i++) {
        if (goblin[i] == nullptr) {
            goblin[i] = new Goblin();
        }

        goblin[i]->move();
    }

    for (int i = 0; i < max_fire_spell; i++)
    {
        if (fire[i] != nullptr && fire[i]->outOfRange()) {
            fire[i]->free();
            fire[i] = nullptr;
        }
        else if (fire[i] != nullptr) {
            fire[i]->move();
        }
    }
}

void Game::detectTouchingObjects() {
    int xDistant = 0;
    int yDistant = 0;

    //Check character position VS guards position
    for (int i = 0; i < guardNum; i++) {
        if (guard[i] == nullptr) {
            guard[i] = new Guard();
        }

        if (wizard->getcharPosX() + wizard->getWidth() > guard[i]->getGuardPosX() &&
            wizard->getcharPosX() < guard[i]->getGuardPosX() + guard[i]->getGuardWidth()) {
            touchingX = true;
        }
        else {
            touchingX = false;
        }
        if (wizard->getcharPosY() + wizard->getHeight() > guard[i]->getGuardPosY() + guard[i]->getGuardHeight() - minDistantY &&
            wizard->getcharPosY() + wizard->getHeight() < guard[i]->getGuardPosY() + guard[i]->getGuardHeight() + minDistantY) {
            touchingY = true;
        }
        else {
            touchingY = false;
        }
        
        if (touchingX && touchingY) {
            guard[i]->okayToSpeak = true;

            wizard->moveBackX(background->getBGspeed());
            wizard->moveBackY(background->getBGspeed());

            guard[i]->moveBackX(background->getBGspeed());
            guard[i]->moveBackY(background->getBGspeed());

            std::cout << "Touched!" << std::endl;
        }
        else {
            guard[i]->okayToSpeak = false;
        }
    }

    //ATTACKS

    //Check guards position VS ... goblins position

    //Check guards position VS 7 fire position

    //Check goblins position VS 7 fire position
}

void Game::updateScreen() {

    SDL_RenderClear(render);

    background->renderCurrentFrame(render);

    wizard->renderCurrentAction(render);

    for (int i = 0; i < goblinNum; i++)
    {
        if (goblin[i] == nullptr) {
            goblin[i] = new Goblin();
        }

        goblin[i]->renderCurrentAction(render);
    }

    for (int i = 0; i < guardNum; i++) {
        if (guard[i] == nullptr) {
            guard[i] = new Guard();
        }

        guard[i]->renderCurrentAction(render);

        if (wizard->getcharPosY() + wizard->getHeight() >= guard[i]->getGuardPosY() + guard[i]->getGuardHeight()) {
            wizard->renderCurrentAction(render);
        }
    }

    for (int i = 0; i < max_fire_spell; i++)
    {
        if (fire[i] != nullptr) {
            fire[i]->renderSpellPosition(render);
        }
    }

    SDL_RenderPresent(render);

}

