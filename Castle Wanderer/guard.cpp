#include "guard.h"
#include "guard_speech.h"
#include <SDL_image.h>

Guard::Guard(int* guardNameCount)
{
    rightmostGuardPos = SCREEN_WIDTH * 4 - 190; //Leave enough space to render whole bubble speech
    leftmostGuardPos = 0;

    reviveGuard(guardNameCount);

    frameTime = 0;

    moveTime = 0;
    nextMoveTime = 0;

    speakTime = 0;
    nextSpeakTime = 0;

    guardTexture = loadFromImage("image/Sprite sheets/guardSheet.png");
    setSpriteClips();

    bubbleSpeechTexture = loadFromImage("image/speechBubble.png");
    bubbleSpeechRect = { NULL };

    healthTexture = loadFromImage("image/health.png");
    healthRect.h = 10;

    healthBarTexture = loadFromImage("image/guard health bar.png");
    healthBarRect.w = 210;
    healthBarRect.h = 40;

    guardWalkVelocity = 0;

    guardWidth = 0;
    guardHeight = 0;

    toRight = false;
    toLeft = true;
    walking = false;

    attackStrength = 0;
    attackTime = 0;
}

void Guard::reviveGuard(int* guardNameCount) {

    while (guardName == "") {
        if (*guardNameCount >= guardNames.size()) {
            *guardNameCount = 0;
        }
        else {
            guardName = guardNames[*guardNameCount];
            *guardNameCount += 1;
        }
    }
    guardNameTexture = loadFromText(guardName, &guardNameRect, white);

    speechTexture = NULL;
    speechRect = { NULL };

    frame = guardWalk0;
    nextOrBackFrame = -1;

    okayToSpeak = false;
    speechLeftToSay = 4;

    do {
        guardPosX = rand() % (rightmostGuardPos - leftmostGuardPos) + leftmostGuardPos;
    } while (guardPosX > -150 && guardPosX < SCREEN_WIDTH);

    guardPosY = rand() % (baseGround - walkLimit) + walkLimit;

    guardVelX = 0;
    guardVelY = 0;

    if (rand() % 2 == 0) {
        righteous = false;
    }
    else righteous = true;

    health = rand() % (maxHealth - minHealth) + minHealth;
    damageReceived = 0;
    die = false;

    attacking = false;
}

Guard::~Guard()
{
    //Deallocate
    free();
}


void Guard::setSpriteClips() {
    //Set sprite clips
    guardSpriteClips[guardWalk0].x = 4;
    guardSpriteClips[guardWalk0].y = 47;
    guardSpriteClips[guardWalk0].w = 85;
    guardSpriteClips[guardWalk0].h = 195;

    guardSpriteClips[guardWalk1].x = 112;
    guardSpriteClips[guardWalk1].y = 47;
    guardSpriteClips[guardWalk1].w = 85;
    guardSpriteClips[guardWalk1].h = 195;

    guardSpriteClips[guardWalk2].x = 229;
    guardSpriteClips[guardWalk2].y = 47;
    guardSpriteClips[guardWalk2].w = 101;
    guardSpriteClips[guardWalk2].h = 195;

    guardSpriteClips[guardWalk3].x = 347;
    guardSpriteClips[guardWalk3].y = 47;
    guardSpriteClips[guardWalk3].w = 86;
    guardSpriteClips[guardWalk3].h = 195;

    guardSpriteClips[guardWalk4].x = 456;
    guardSpriteClips[guardWalk4].y = 47;
    guardSpriteClips[guardWalk4].w = 102;
    guardSpriteClips[guardWalk4].h = 195;

    guardSpriteClips[attack1].x = 331;
    guardSpriteClips[attack1].y = 263;
    guardSpriteClips[attack1].w = 119;
    guardSpriteClips[attack1].h = 195;

    guardSpriteClips[attack2].x = 462;
    guardSpriteClips[attack2].y = 263;
    guardSpriteClips[attack2].w = 119;
    guardSpriteClips[attack2].h = 195;
}

void Guard::renderGuard(SDL_Renderer* renderer, SDL_Rect* clip)
{
    if (clip != NULL)
    {
        guardWidth = clip->w;
        guardHeight = clip->h;
    }

    SDL_Rect renderGuard = { guardPosX, guardPosY, guardWidth, guardHeight };

    if (toLeft == true) {
        SDL_RenderCopyEx(renderer, guardTexture, clip, &renderGuard, 0, NULL, SDL_FLIP_HORIZONTAL);
    }
    else {
        SDL_RenderCopy(renderer, guardTexture, clip, &renderGuard);
    }    
}

void Guard::renderSpeechBubble(SDL_Renderer* renderer) {

    speechRect.x = guardPosX - 20;
    speechRect.y = guardPosY - speechRect.h - 70;

    bubbleSpeechRect.x = speechRect.x - 20;
    bubbleSpeechRect.y = speechRect.y - 17;
    bubbleSpeechRect.w = speechRect.w + 50;
    bubbleSpeechRect.h = (speechRect.h + 40) * 6 / 5;

    SDL_RenderCopy(renderer, bubbleSpeechTexture, NULL, &bubbleSpeechRect);
    SDL_RenderCopy(renderer, speechTexture, NULL, &speechRect);
}

void Guard::renderHealthBar(SDL_Renderer* renderer) {
    healthRect.x = guardPosX - 7;
    healthRect.y = guardPosY - 12;
    healthRect.w = 188 - damageReceived * 188 / health;
    SDL_RenderCopy(renderer, healthTexture, NULL, &healthRect);

    healthBarRect.x = healthRect.x - 17;
    healthBarRect.y = healthRect.y - 21;
    SDL_RenderCopy(renderer, healthBarTexture, NULL, &healthBarRect);

    guardNameRect.x = healthBarRect.x + healthBarRect.w / 2 - guardNameRect.w / 2;
    guardNameRect.y = healthBarRect.y - 7;
    SDL_RenderCopy(renderer, guardNameTexture, NULL, &guardNameRect);
}

void Guard::renderCurrentAction(SDL_Renderer* renderer, unsigned int currentTime) {
    if (currentTime > frameTime + nextFrameTime) {
        if (!attacking && guardVelX == 0 && guardVelY == 0) {
            frame = guardWalk0;
        }
        else if (!attacking && walking) {
            if (frame >= guardWalk4)
            {
                nextOrBackFrame = -1;
            }
            else if (frame <= guardWalk0) {
                nextOrBackFrame = 1;
            }
            frame += nextOrBackFrame;
        }
        else if (attacking) {
            frame++;
            if (frame > attack2 || frame < attack1) {
                frame = attack1;
            }
        }

        frameTime = currentTime;
    }
    
    
    SDL_Rect* currentClip = &guardSpriteClips[frame];

    renderGuard(renderer, currentClip);

    if (speechTexture != NULL) {
        renderSpeechBubble(renderer);
    }

    renderHealthBar(renderer);
}



void Guard::randomSpeech() {

    speechLeftToSay--;

    std::string speech = "";

    if (righteous) {
        if (rand() % 4 == 0) {
            speech = goodQuestion[rand() % goodQuestion.size()];
        }
        else {
            speech = goodSentence[rand() % goodSentence.size()];
        }
    }
    else {
        if (rand() % 4 == 0) {
            speech = notgoodQuestion[rand() % notgoodQuestion.size()];
        }
        else {
            speech = notgoodSentence[rand() % notgoodSentence.size()];
        }
    }
    speechTexture = loadFromText(speech, &speechRect, black, textWrapLength);
    nextSpeakTime = speech.size() * timeToReadOneCharacter;
}

void Guard::moveRandom(unsigned int currentTime) {

    guardWalkVelocity = rand() % (maxVelocity - minVelocity) + minVelocity;

    speechTexture = NULL;

    int moveX = rand() % 3;
    if (moveX == 0) {
        guardVelX = guardWalkVelocity;
        toLeft = false;
        toRight = true;
        walking = true;
    }
    else if (moveX == 1) {
        guardVelX = -guardWalkVelocity;
        toLeft = true;
        toRight = false;
        walking = true;
    }
    else {
        guardVelX = 0;
    }

    int moveY = rand() % 3;
    if (moveY == 0) {
        guardVelY = guardWalkVelocity;
        walking = true;
    }
    else if (moveY == 1) {
        guardVelY = -guardWalkVelocity;
        walking = true;
    }
    else {
        guardVelY = 0;
    }

    if (guardVelX == 0 && guardVelY == 0) {
        walking = false;
    }

    moveTime = currentTime;
    nextMoveTime = rand() % (maxNextMovetime - minNextMovetime) + minNextMovetime;
}

void Guard::move(int targetPosX, int targetPosY, int targetWidth, unsigned int currentTime)
{
    if (okayToSpeak && speechLeftToSay > 0 && damageReceived <= 0 && currentTime > speakTime + nextSpeakTime) {
        randomSpeech();
        guardVelX = 0;
        guardVelY = 0;
        moveTime = currentTime + nextSpeakTime;
        speakTime = currentTime;
        attacking = false;
    }
    else if (speechLeftToSay <= 0 || damageReceived > 0) {
        chaseTarget(targetPosX, targetPosY, targetWidth);
        attacking = true;
    }
    else if (currentTime > moveTime + nextMoveTime) {
        moveRandom(currentTime);
        attacking = false;
    }

    guardPosX += (guardVelX + plusVelocity);
    if (guardVelY != 0) {
        guardPosY += guardVelY;
    }

    checkGuardLimits();
}

void Guard::checkGuardLimits() {

    //If went too far to the right or left
    if ((guardPosX < leftmostGuardPos) || (guardPosX > rightmostGuardPos))
    {
        guardPosX -= guardVelX;
    }

    //If walked too far up
    if (walking && guardPosY < walkLimit) {
        guardPosY = walkLimit;
    }

    //If got too far down
    if (guardPosY > baseGround) {
        guardPosY = baseGround - 1;
    }
}



void Guard::receiveAttack(int damage, int currentTime) {
    damageReceived += damage;
    if (damageReceived >= health) {
        die = true;
    }

    guardPosX -= guardVelX;

    std::string speech = angrySpeech[rand() % angrySpeech.size()];
    speechTexture = loadFromText(speech, &speechRect, black, textWrapLength);
}

void Guard::chaseTarget(int targetPosX, int targetPosY, int targetWidth) {
    if (guardPosX + guardWidth < targetPosX + approxDistant) {
        guardVelX = guardWalkVelocity;
        toLeft = false;
        toRight = true;
    }
    else if (guardPosX > targetPosX + targetWidth - approxDistant) {
        guardVelX = -guardWalkVelocity;
        toLeft = true;
        toRight = false;
    }
    else {
        guardVelX = 0;
    }

    if (guardPosY < targetPosY - approxDistant) {
        guardVelY = guardWalkVelocity;
    }
    else if (guardPosY > targetPosY + approxDistant) {
        guardVelY = -guardWalkVelocity;
    }
    else {
        guardVelY = 0;
    }
}

bool Guard::isAttacking() {
    return attacking;
}

int Guard::getAttackDamage(unsigned int currentTime) {
    if (currentTime > attackTime + nextAttackTime) {
        attackStrength = rand() % (maxAttackStrength - minAttackStrength) + minAttackStrength;
        attackTime = currentTime;
    }
    else {
        attackStrength = 0;
    }
    return attackStrength;
}

bool Guard::isDead() {
    return die;
}

bool Guard::isGood() {
    return righteous;
}

int Guard::getGuardPosX() {
    return guardPosX;
}
int Guard::getGuardPosY() {
    return guardPosY;
}

void Guard::setPlusVelocity(int bgVelocity) {
    plusVelocity = bgVelocity;
    leftmostGuardPos += plusVelocity;
    rightmostGuardPos += plusVelocity;
}

int Guard::getGuardVelX() {
    return guardVelX;
}
int Guard::getGuardVelY() {
    return guardVelY;
}


int Guard::getGuardWidth() {
    return guardSpriteClips[frame].w;
}
int Guard::getGuardHeight() {
    return guardSpriteClips[frame].h;
}



void Guard::free()
{
    freeTexture(guardNameTexture);
    freeTexture(guardTexture);
    freeTexture(speechTexture);
    freeTexture(bubbleSpeechTexture);
    freeTexture(healthTexture);
    freeTexture(healthBarTexture);
}