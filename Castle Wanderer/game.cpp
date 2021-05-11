#include "game.h"
#include "guard_speech.h"
#include <ctime>

Game::Game()
{
    srand(time(0));

    volume = 50;
    Mix_VolumeMusic(volume);

    choseInstruct1 = false;
    choseInstruct2 = false;

    gameTime = 0;
    currentTime = SDL_GetTicks();

    render = initSDL();
    font = initTTF();

    startScreen = new StartScreen;
    instruct1 = loadFromImage("image/instruct1.png");
    instruct2 = loadFromImage("image/instruct2.png");

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
        fire[i] = new Fire(wizard->isToRight(), wizard->getCharPosX(), wizard->getCharPosY(), background->getBGVelocity());
    }

    boss = new FinalBoss;
    showFinalBoss = false;

    defeatBadGuard = 0;
    defeatGoodGuard = 0;
}

Game::~Game()
{
    freeTexture(instruct1);
    freeTexture(instruct2);
    close();
}



bool Game::startGameScreen() {
    if (render == nullptr || font == nullptr) {
        std::cerr << "Failed to initialize!\n";
        return false;
    }
    else {
        //Run start screen
        startScreen->runStartScreen(render, &e);
        startScreen->deleteStartScreen();
        if (startScreen->isQuittingGame()) {
            return false;
        }
        return true;
    }
}

void Game::showInstructScreen() {
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
            switch (e.key.keysym.sym) {
            case SDLK_a:
                choseInstruct1 = true;
                choseInstruct2 = false;
                break;
            case SDLK_d:
                choseInstruct1 = false;
                choseInstruct2 = true;
                break;

            case SDLK_ESCAPE:
                choseInstruct1 = false;
                choseInstruct2 = false;
                break;
            }
        }
        if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
            case SDLK_LEFT:
                if (volume > 0) {
                    volume -= volumeChangeAmount;
                }
                Mix_VolumeMusic(volume);
                break;
            case SDLK_RIGHT:
                if (volume < MIX_MAX_VOLUME) {
                    volume += volumeChangeAmount;
                }
                Mix_VolumeMusic(volume);
                break;
            }
        }

        if (e.type == SDL_QUIT) {
            choseInstruct1 = false;
            choseInstruct2 = false;
            close();
        }
    }

    if (choseInstruct1) {
        SDL_RenderClear(render);
        SDL_RenderCopy(render, instruct1, NULL, NULL);
        SDL_RenderPresent(render);
    }
    else if (choseInstruct2) {
        SDL_RenderClear(render);
        SDL_RenderCopy(render, instruct2, NULL, NULL);
        SDL_RenderPresent(render);
    }
}

bool Game::handlingKeyboardEvents() {
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
            switch (e.key.keysym.sym) {
            case SDLK_f:
                for (int i = 0; i < fireSpellNum; i++)
                {
                    if (fire[i]->isAvailable()) {
                        fire[i]->shoot(wizard->isToRight(), wizard->getCharPosX(), wizard->getCharPosY(), background->getBGVelocity());
                        break;
                    }
                }
                break;
            case SDLK_i:
                choseInstruct1 = true;
                while (choseInstruct1 || choseInstruct2) {
                    showInstructScreen();
                }
                break;
            case SDLK_ESCAPE:
                close();
                return false;
            }
        }
        if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
            case SDLK_LEFT:
                if (volume > 0) {
                    volume -= volumeChangeAmount;
                }
                Mix_VolumeMusic(volume);
                break;
            case SDLK_RIGHT:
                if (volume < MIX_MAX_VOLUME) {
                    volume += volumeChangeAmount;
                }
                Mix_VolumeMusic(volume);
                break;
            }
        }

        if (e.type == SDL_QUIT) {
            return false;
        }
        wizard->handleEvent(e, currentTime);
        background->handledEvent(e, render);
    }
    return true;
}

void Game::setObjectPlusVelocity() {
    if (wizard->isAtEdgeOfScreen()) {
        background->move();
        if (!background->isAtFurthestLeftX() || !background->isAtFurthestRightX()) {
            for (int i = 0; i < guardNum; i++) {
                if (!guard[i]->isDead()) {
                    guard[i]->setPlusVelocity(background->getObjectPlusVelocity());
                }
            }

            for (int i = 0; i < goblinNum; i++) {
                if (!goblin[i]->isDead()) {
                    goblin[i]->setPlusVelocity(background->getObjectPlusVelocity());
                }
            }

            if (defeatBadGuard >= badGuardDieLimit) {

            }
            boss->setPlusVelocity(background->getObjectPlusVelocity());
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

        if (defeatBadGuard >= badGuardDieLimit) {

        }
        boss->setPlusVelocity(0);
    }
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

    setObjectPlusVelocity();

    for (int i = 0; i < guardNum; i++) {
        if (guard[i]->isDead()) {
            if (guard[i]->isGood()) {
                defeatGoodGuard++;
            }
            else {
                defeatBadGuard++;
                wizard->heal(defeatBadGuard);
            }
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

    if (defeatBadGuard >= badGuardDieLimit) {
        
    }
    boss->move(tempCharPosX, tempCharWidth, tempCharPosY + tempCharHeight, currentTime);
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

void Game::checkDefeatedGuards() {
    if (defeatGoodGuard == goodGuardDieLimit) {
        for (int i = 0; i < goblinNum; i++) {
            if (!goblin[i]->isDead()) {
                goblin[i]->getAngry(currentTime);
            }
        }
        defeatGoodGuard = 0;
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

    if (defeatBadGuard >= badGuardDieLimit) {
        
    }
    boss->renderCurrentAction(render, currentTime);

    SDL_RenderPresent(render);
}

void Game::runGameLoop() {
    while (handlingKeyboardEvents()) {
        currentTime = SDL_GetTicks();
        if (currentTime > gameTime + nextFrameTime) {

            moveObjects();

            detectTouchingObjects();
            checkDefeatedGuards();

            updateScreen();

            gameTime = currentTime;
        }
    }
}


void Game::playGame() {
    if (startGameScreen()) {

        gameMusic = loadFromMusic("music/gameMusic.mp3");
        Mix_PlayMusic(gameMusic, -1);

        runGameLoop();
    }
}