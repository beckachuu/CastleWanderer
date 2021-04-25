#include "game.h"
#include "guard_speech.h"
#include <ctime>

Game::Game()
{
    srand(time(0));

    gameTime = 0;

    render = initSDL();
    font = initTTF();

    background = new Background();

    guardNameCount = rand() % guardNames.size();

    wizard = new MyCharacter();
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
                for (int i = 0; i < fireSpellNum; i++)
                {
                    if (fire[i] == nullptr) {
                        fire[i] = new Fire(wizard->isToRight(), wizard->getCharPosX(), wizard->getCharPosY(), background->getBGspeed());
                        break;
                    }
                }
            }
        }
        else if (e.type == SDL_QUIT) {
            return false;
        }
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
    tempCharPosX = wizard->getCharPosX();
    tempCharPosY = wizard->getCharPosY();
    tempCharWidth = wizard->getWidth();
    tempCharHeight = wizard->getHeight();

    if (wizard->isAtEdgeOfScreen()) {
        background->move();
        if (!background->isAtFurthestLeftX() || !background->isAtFurthestRightX()) {
            for (int i = 0; i < guardNum; i++) {
                if (guard[i] != nullptr) {
                    guard[i]->setPlusVelocity(background->getBGVelX());
                }
            }
            for (int i = 0; i < goblinNum; i++) {
                if (goblin[i] != nullptr) {
                    goblin[i]->setPlusVelocity(background->getBGVelX());
                }
            }
        }
    }
    if (!wizard->isAtEdgeOfScreen() || background->isAtFurthestLeftX() || background->isAtFurthestRightX()) {
        for (int i = 0; i < guardNum; i++) {
            if (guard[i] != nullptr) {
                guard[i]->setPlusVelocity(0);
            }
        }
        for (int i = 0; i < goblinNum; i++) {
            if (goblin[i] != nullptr) {
                goblin[i]->setPlusVelocity(0);
            }
        }
    }

    for (int i = 0; i < guardNum; i++) {
        if (guard[i] == nullptr) {
            guard[i] = new Guard(&guardNameCount);
        }
        guard[i]->move(tempCharPosX, tempCharPosY, tempCharWidth);

        tempGuardPosX[i] = guard[i]->getGuardPosX();
        tempGuardPosY[i] = guard[i]->getGuardPosY();
        tempGuardWidth[i] = guard[i]->getGuardWidth();
        tempGuardHeight[i] = guard[i]->getGuardHeight();
    }
    for (int i = 0; i < goblinNum; i++) {
        if (goblin[i] == nullptr) {
            goblin[i] = new Goblin();
        }
        goblin[i]->move();
    }

    for (int i = 0; i < fireSpellNum; i++)
    {
        if (fire[i] != nullptr && fire[i]->outOfRange()) {
            fire[i]->free();
            fire[i] = nullptr;
        }
        else if (fire[i] != nullptr) {
            fire[i]->move();

            tempFirePosX[i] = fire[i]->getPosX();
            tempFirePosY[i] = fire[i]->getPosY();
            tempFireWidth[i] = fire[i]->getWidth();
            tempFireHeight[i] = fire[i]->getHeight();

        }
    }
}

void Game::detectTouchingObjects() {

    //Check character position VS guards position
    for (int i = 0; i < guardNum; i++) {
        if (guard[i] != nullptr) {

            //Guards vs wizard
            if (tempCharPosX + tempCharWidth > tempGuardPosX[i] &&
                tempCharPosX < tempGuardPosX[i] + tempGuardWidth[i] &&
                tempCharPosY + tempCharHeight > tempGuardPosY[i] + tempGuardHeight[i] - approxDistant &&
                tempCharPosY + tempCharHeight < tempGuardPosY[i] + tempGuardHeight[i] + approxDistant) {

                guard[i]->okayToSpeak = true;
                if (guard[i]->isAttacking()) {
                    wizard->receiveDamage(guard[i]->getAttackDamage());
                }

            }
            else {
                guard[i]->okayToSpeak = false;
            }

            //Guards vs Fire
            for (int j = 0; j < fireSpellNum; j++)
            {
                if (fire[j] != nullptr) {

                    if (tempFirePosX[j] + tempFireWidth[j] > tempGuardPosX[i] &&
                        tempFirePosX[j] < tempGuardPosX[j] + tempGuardWidth[i] &&
                        tempFirePosY[j] + tempFireHeight[j] /2 > tempGuardPosY[i] + tempGuardHeight[i]/2 - approxDistant &&
                        tempFirePosY[j] + tempFireHeight[j] /2 < tempGuardPosY[i] + tempGuardHeight[i]/2 + approxDistant) {

                        guard[i]->receiveAttack(fire[j]->getFireDamage());
                        fire[j]->free();
                        fire[j] = nullptr;

                        if (guard[i]->isDead()) {
                            guard[i]->free();
                            guard[i] = nullptr;
                        }
                    }
                }
            }
        }
    }


    //Check goblins position VS 7 fire position

}

void Game::updateScreen() {

    SDL_RenderClear(render);

    background->renderCurrentFrame(render);

    wizard->renderCurrentAction(render);

    for (int i = 0; i < goblinNum; i++)
    {
        if (goblin[i] != nullptr) {
            goblin[i]->renderCurrentAction(render);
        }
    }

    for (int i = 0; i < guardNum; i++) {
        if (guard[i] != nullptr) {
            guard[i]->renderCurrentAction(render);
        }
    }

    for (int i = 0; i < fireSpellNum; i++)
    {
        if (fire[i] != nullptr) {
            fire[i]->renderSpellPosition(render);
        }
    }

    SDL_RenderPresent(render);

}
