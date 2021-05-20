#include "game.h"
#include "guard_speech.h"
#include <ctime>

Game::Game()
{
    srand(time(0));

    win = false;
    lose = false;
    quitGame = false;

    volume = initialVolume;
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

    youWin = loadFromImage("image/you won.png");
    youLose = loadFromImage("image/you lost.png");

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

bool Game::endGameScreen() {
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE && e.key.repeat == 0) {
            quitGame = true;
            return false;
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
            case SDLK_r:
                quitGame = false;
                return false;
            }
        }

        if (e.type == SDL_QUIT) {
            quitGame = true;
            return false;
        }
    }

    if (win) {
        SDL_RenderClear(render);
        SDL_RenderCopy(render, youWin, NULL, NULL);
        SDL_RenderPresent(render);
    }
    else if (lose) {
        SDL_RenderClear(render);
        SDL_RenderCopy(render, youLose, NULL, NULL);
        SDL_RenderPresent(render);
    }
    return true;
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
                        tempFireFeetPoint[i] = fire[i]->getPosY() + fireDistantFromGround;
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

            if (showFinalBoss) {
                boss->setPlusVelocity(background->getObjectPlusVelocity());
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

        if (showFinalBoss) {
            boss->setPlusVelocity(0);
        }
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
    tempCharLeft = wizard->getCharPosX();
    tempCharRight = wizard->getCharPosX() + wizard->getWidth();
    tempCharFeetPoint = wizard->getCharFeetPoint();

    setObjectPlusVelocity();

    for (int i = 0; i < guardNum; i++) {
        if (guard[i]->isDead()) {
            if (guard[i]->isGood()) {
                defeatGoodGuard++;
            }
            else {
                defeatBadGuard++;
                if (defeatBadGuard == badGuardDieLimit) {
                    showFinalBoss = true;
                }
                wizard->heal(i);
            }
            guard[i]->reviveGuard(&guardNameCount);
        }
        guard[i]->move(tempCharLeft, tempCharRight, tempCharFeetPoint, currentTime);

        tempGuardLeft[i] = guard[i]->getGuardPosX();
        tempGuardRight[i] = guard[i]->getGuardPosX() + guard[i]->getGuardWidth();
        tempGuardFeetPoint[i] = guard[i]->getGuardFeetPoint();
    }
    for (int i = 0; i < goblinNum; i++) {
        if (goblin[i]->isDead()) {
            goblin[i]->reviveGoblin();
        }
        goblin[i]->move(tempCharLeft, tempCharRight, tempCharFeetPoint, currentTime);

        tempGoblinLeft[i] = goblin[i]->getGoblinPosX();
        tempGoblinRight[i] = goblin[i]->getGoblinPosX() + goblin[i]->getGoblinWidth();
        tempGoblinFeetPoint[i] = goblin[i]->getGoblinFeetPoint();
    }

    for (int i = 0; i < fireSpellNum; i++)
    {
        if (!fire[i]->isAvailable() && !fire[i]->outOfRange()) {
            fire[i]->move();

            tempFireLeft[i] = fire[i]->getPosX();
            tempFireRight[i] = fire[i]->getPosX() + fire[i]->getWidth();
        }
    }

    if (showFinalBoss) {
        boss->move(tempCharLeft, tempCharRight, tempCharFeetPoint, currentTime);
        tempBossLeft = boss->getBossPosX();
        tempBossRight = boss->getBossPosX() + boss->getBossWidth();
        tempBossFeetPoint = boss->getBossFeetPoint();
    }
}

void Game::detectFireAttack() {
    for (int j = 0; j < fireSpellNum; j++) {
        if (!fire[j]->isAvailable()) {
            for (int i = 0; i < goblinNum; i++) {
                if (!goblin[i]->isDead()) {
                    if (tempFireRight[j] > tempGoblinLeft[i] &&
                        tempFireLeft[j] < tempGoblinRight[i] &&
                        tempFireFeetPoint[j] > tempGoblinFeetPoint[i] - approxDistant &&
                        tempFireFeetPoint[j] < tempGoblinFeetPoint[i] + approxDistant) {

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
                    if (tempFireRight[j] > tempGuardLeft[i] &&
                        tempFireLeft[j] < tempGuardRight[i] &&
                        tempFireFeetPoint[j] > tempGuardFeetPoint[i] - approxDistant &&
                        tempFireFeetPoint[j] < tempGuardFeetPoint[i] + approxDistant) {

                        guard[i]->receiveAttack(fire[j]->getFireDamage(), currentTime);
                        fire[j]->reload();
                        break;
                    }
                }
            }
        }
        if (!fire[j]->isAvailable() && showFinalBoss) {
            if (tempFireRight[j] > tempBossLeft &&
                tempFireLeft[j] < tempBossRight &&
                tempFireFeetPoint[j] > tempBossFeetPoint - approxDistant &&
                tempFireFeetPoint[j] < tempBossFeetPoint + approxDistant) {

                boss->receiveAttack(fire[j]->getFireDamage(), currentTime);
                if (boss->isDead()) {
                    win = true;
                }
                fire[j]->reload();
                continue;
            }
        }
    }
}

void Game::detectTouchingObjects() {
    detectFireAttack();
    //Guards vs wizard
    for (int i = 0; i < guardNum; i++) {
        if (!guard[i]->isDead()) {

            if (tempCharRight > tempGuardLeft[i] &&
                tempCharLeft < tempGuardRight[i] &&
                tempCharFeetPoint > tempGuardFeetPoint[i] - approxDistant &&
                tempCharFeetPoint < tempGuardFeetPoint[i] + approxDistant) {

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

    //Goblins vs wizard
    for (int i = 0; i < goblinNum; i++) {
        if (!goblin[i]->isDead() && goblin[i]->isAngry()) {
            if (tempGoblinLeft[i] < tempCharRight &&
                tempGoblinRight[i] > tempCharLeft &&
                tempGoblinFeetPoint[i] > tempCharFeetPoint - approxDistant &&
                tempGoblinFeetPoint[i] < tempCharFeetPoint + approxDistant) {
                wizard->receiveDamage(goblin[i]->getExplodeDamage());
            }
        }
    }

    //Boss vs wizard
    if (showFinalBoss) {
        if (tempCharRight > tempBossLeft &&
            tempCharLeft < tempBossRight &&
            tempCharFeetPoint > tempBossFeetPoint - approxDistant &&
            tempCharFeetPoint < tempBossFeetPoint + approxDistant)
        {
            wizard->receiveDamage(boss->getAttackDamage(currentTime));
        }
    }
    lose = wizard->isDefeated();
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

void Game::renderWizard(int i) {
    wizard->renderCurrentAction(render);
}
void Game::renderGuard(int i) {
    guard[i]->renderCurrentAction(render);
}
void Game::renderGoblin(int i) {
    goblin[i]->renderCurrentAction(render);
}
void Game::renderFinalBoss(int i) {
    guard[i]->renderCurrentAction(render);
}

void Game::updateScreen() {

    SDL_RenderClear(render);

    background->renderCurrentFrame(render);

    wizard->setCurrentFrame(currentTime);
    wizard->renderCurrentAction(render);

    for (int i = 0; i < goblinNum; i++)
    {
        if (!goblin[i]->isDead()) {
            goblin[i]->setCurrentFrame(currentTime);
            if (tempGoblinRight > 0 && tempGoblinLeft[i] < SCREEN_WIDTH) {
                goblin[i]->renderCurrentAction(render);
            }
        }
    }

    for (int i = 0; i < guardNum; i++) {
        if (!guard[i]->isDead()) {
            guard[i]->setCurrentFrame(currentTime);
            if (tempGuardRight[i] > 0 && tempGuardLeft[i] < SCREEN_WIDTH) {
                guard[i]->renderCurrentAction(render);
            }
        }
    }

    for (int i = 0; i < fireSpellNum; i++)
    {
        if (!fire[i]->isAvailable()) {
            fire[i]->renderSpellPosition(render);
        }
    }

    if (showFinalBoss) {
        boss->setCurrentFrame(currentTime);
        boss->renderCurrentAction(render);
    }

    SDL_RenderPresent(render);
}

void Game::restartGame() {
    win = false;
    lose = false;
    quitGame = false;

    currentTime = SDL_GetTicks();

    background->resetBackGround();

    win = false;
    lose = false;
    quitGame = false;

    for (int i = 0; i < guardNum; i++) {
        guard[i]->reviveGuard(&guardNameCount);
    }

    for (int i = 0; i < goblinNum; i++) {
        goblin[i]->reviveGoblin();
    }

    wizard->resetCharacter();

    for (int i = 0; i < fireSpellNum; i++) {
        fire[i]->reload();
    }

    boss->resetBoss();
    showFinalBoss = false;

    defeatBadGuard = 0;
    defeatGoodGuard = 0;
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
        if (win || lose) {
            while (endGameScreen()) {}
            if (quitGame == true) {
                break;
            }
            else restartGame();
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

