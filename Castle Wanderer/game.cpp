#include "game.h"
#include "guard_speech.h"
#include <ctime>

Game::Game()
{

    srand(time(0));

    gameTime = 0;
    currentTime = SDL_GetTicks();

    render = initSDL();
    font = initTTF();

    startScreen = new StartScreen;


    background = new Background();

    guardNameCount = rand() % guardNames.size();
    for (int i = 0; i < guardNum; i++) {
        guard[i] = new Guard(&guardNameCount);
    }

    for (int i = 0; i < goblinNum; i++) {
        goblin[i] = new Goblin;
    }

    wizard = new MyCharacter;

    for (int i = 0; i < fireSpellNum; i++) {
        fire[i] = new Fire(wizard->isToRight(), wizard->getCharPosX(), wizard->getCharPosY(), background->getBGspeed());
    }
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
        //Run start screen
        startScreen->runStartScreen(render,&e);
        startScreen->deleteStartScreen();
        if (startScreen->isQuittingGame()) {
            return;
        }

        //Game
        gameMusic = loadFromMusic("music/gameMusic.mp3");
        Mix_PlayMusic(gameMusic, -1);

        while (handlingKeyboardEvents()) {
            currentTime = SDL_GetTicks();
            if (currentTime > gameTime + nextFrameTime) {
                
                moveObjects();

                detectTouchingObjects();
                
                updateScreen();

                gameTime = currentTime;
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
            case SDLK_f:
                for (int i = 0; i < fireSpellNum; i++)
                {
                    if (fire[i]->isAvailable()) {
                        fire[i]->shoot(wizard->isToRight(), wizard->getCharPosX(), wizard->getCharPosY(), background->getBGspeed());
                        break;
                    }
                }
            }
        }
        else if (e.type == SDL_QUIT) {
            return false;
        }
        wizard->handleEvent(e, currentTime);
        background->handledEvent(e, render);
    }
    return true;
}

void Game::moveObjects() {

    if (background->isAtFurthestLeftX()) {
        wizard->setFurthestPoints(characterFurthestLeft, SCREEN_WIDTH - 100);
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
                if (!guard[i]->isDead()) {
                    guard[i]->setPlusVelocity(background->getBGVelX());
                }
            }
            for (int i = 0; i < goblinNum; i++) {
                if (!goblin[i]->isDead()) {
                    goblin[i]->setPlusVelocity(background->getBGVelX());
                }
            }
        }
    }
    if (!wizard->isAtEdgeOfScreen() || background->isAtFurthestLeftX() || background->isAtFurthestRightX()) {
        for (int i = 0; i < guardNum; i++) {
            if (!guard[i]->isDead()) {
                guard[i]->setPlusVelocity(0);
            }
        }
        for (int i = 0; i < goblinNum; i++) {
            if (!goblin[i]->isDead()) {
                goblin[i]->setPlusVelocity(0);
            }
        }
    }

    for (int i = 0; i < guardNum; i++) {
        if (guard[i]->isDead()) {
            guard[i]->reviveGuard(&guardNameCount);
        }
        guard[i]->move(tempCharPosX, tempCharPosY, tempCharWidth, currentTime);

        tempGuardPosX[i] = guard[i]->getGuardPosX();
        tempGuardPosY[i] = guard[i]->getGuardPosY();
        tempGuardWidth[i] = guard[i]->getGuardWidth();
        tempGuardHeight[i] = guard[i]->getGuardHeight();
    }
    for (int i = 0; i < goblinNum; i++) {
        if (goblin[i]->isDead()) {
            goblin[i]->reviveGoblin();
        }
        goblin[i]->move(tempCharPosX, tempCharPosY, tempCharWidth, tempCharHeight, currentTime);

        tempGoblinPosX[i] = goblin[i]->getGoblinPosX();
        tempGoblinPosY[i] = goblin[i]->getGoblinPosY();
        tempGoblinWidth[i] = goblin[i]->getGoblinWidth();
        tempGoblinHeight[i] = goblin[i]->getGoblinHeight();
    }

    for (int i = 0; i < fireSpellNum; i++)
    {
        if (!fire[i]->isAvailable() && !fire[i]->outOfRange()) {
            fire[i]->move();

            tempFirePosX[i] = fire[i]->getPosX();
            tempFirePosY[i] = fire[i]->getPosY();
            tempFireWidth[i] = fire[i]->getWidth();
            tempFireHeight[i] = fire[i]->getHeight();
        }
    }
}

void Game::detectTouchingObjects() {
    //Guards vs wizard
    for (int i = 0; i < guardNum; i++) {
        if (!guard[i]->isDead()) {

            if (tempCharPosX + tempCharWidth > tempGuardPosX[i] &&
                tempCharPosX < tempGuardPosX[i] + tempGuardWidth[i] &&
                tempCharPosY + tempCharHeight > tempGuardPosY[i] + tempGuardHeight[i] - approxDistant &&
                tempCharPosY + tempCharHeight < tempGuardPosY[i] + tempGuardHeight[i] + approxDistant) {

                guard[i]->okayToSpeak = true;
                if (guard[i]->isAttacking()) {
                    wizard->receiveDamage(guard[i]->getAttackDamage(currentTime));
                }

            }
            else {
                guard[i]->okayToSpeak = false;
            }
        }
    }

    //Fires vs goblins, guards
    for (int j = 0; j < fireSpellNum; j++)
    {
        if (!fire[j]->isAvailable()) {
            for (int i = 0; i < goblinNum; i++) {
                if (!goblin[i]->isDead()) {

                    if (tempFirePosX[j] + tempFireWidth[j] > tempGoblinPosX[i] &&
                        tempFirePosX[j] < tempGoblinPosX[i] + tempGoblinWidth[i] &&
                        tempFirePosY[j] > tempGoblinPosY[i] &&
                        tempFirePosY[j] + tempFireHeight[j] < tempGoblinPosY[i] + tempGoblinHeight[i] / 2 + approxDistant) {

                        goblin[i]->getAngry(currentTime);
                        fire[j]->reload();
                        break;
                    }
                }
            }
        }

        if (!fire[j]->isAvailable()) {
            for (int i = 0; i < guardNum; i++) {
                if (!guard[i]->isDead()) {

                    if (tempFirePosX[j] + tempFireWidth[j] > tempGuardPosX[i] &&
                        tempFirePosX[j] < tempGuardPosX[i] + tempGuardWidth[i] &&
                        tempFirePosY[j] + tempFireHeight[j] / 2 > tempGuardPosY[i] + tempGuardHeight[i] / 2 - approxDistant &&
                        tempFirePosY[j] + tempFireHeight[j] / 2 < tempGuardPosY[i] + tempGuardHeight[i] / 2 + approxDistant) {

                        guard[i]->receiveAttack(fire[j]->getFireDamage(),currentTime);
                        fire[j]->reload();
                        break;
                    }
                }
            }
        }
    }

    //Goblins vs wizard
    for (int i = 0; i < goblinNum; i++) {
        if (!goblin[i]->isDead() && goblin[i]->isAngry()) {
            if (tempGoblinPosX[i] < tempCharPosX + tempCharWidth &&
                tempGoblinPosX[i] + tempGoblinWidth[i] > tempCharPosX &&
                tempGoblinPosY[i] + tempGoblinHeight[i] > tempCharPosY + tempCharHeight - approxDistant &&
                tempGoblinPosY[i] + tempGoblinHeight[i] < tempCharPosY + tempCharHeight + approxDistant) {
                wizard->receiveDamage(goblin[i]->getExplodeDamage());
            }
        }
    }
}

void Game::updateScreen() {

    SDL_RenderClear(render);

    background->renderCurrentFrame(render);

    wizard->renderCurrentAction(render, currentTime);

    for (int i = 0; i < goblinNum; i++)
    {
        if (!goblin[i]->isDead()) {
            goblin[i]->renderCurrentAction(render, currentTime);
        }
    }

    for (int i = 0; i < guardNum; i++) {
        if (!guard[i]->isDead()) {
            guard[i]->renderCurrentAction(render, currentTime);
        }
    }

    for (int i = 0; i < fireSpellNum; i++)
    {
        if (!fire[i]->isAvailable()) {
            fire[i]->renderSpellPosition(render);
        }
    }

    SDL_RenderPresent(render);
}
