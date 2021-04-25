#include "guard.h"
#include "guard_speech.h"
#include <SDL_image.h>

Guard::Guard(int* guardNameCount)
{
    while (guardName == "") {
        if (*guardNameCount >= guardNames.size()) {
            *guardNameCount = 0;
        }
        else {
            guardName = guardNames[*guardNameCount];
            *guardNameCount += 1;
        }
    }

    guardTexture = NULL;
    guardWidth = 0;
    guardHeight = 0;

    //Initialize the offsets
    rightmostGuardPos = SCREEN_WIDTH * 4;
    leftmostGuardPos = 0;
    guardPosX = rand() % (rightmostGuardPos - leftmostGuardPos) + leftmostGuardPos;
    guardPosY = rand() % (baseGround - walkLimit) + walkLimit;

    //Initialize the velocity
    guardWalkVelocity = 0;
    guardVelX = 0;
    guardVelY = 0;

    frame = guardWalk0;
    nextOrBackFrame = -1;

    frameTime = 0;

    moveTime = 0;
    nextMoveTime = 0;

    okayToSpeak = false;
    speakTime = 0;
    nextSpeakTime = 0;
    speechLeftToSay = 3;

    toRight = false;
    //Default turning left
    toLeft = true;
    walking = false;

    guardNameRect = { NULL };
    speechBubbleRect = { NULL };

    //Load textures
    guardNameTexture = loadFromText(guardName, &guardNameRect, white);

    guardTexture = loadFromFile("image/guardSheet.png");
    setSpriteClips();

    speechTexture = NULL;
    bubbleSpeechTexture = loadFromFile("image/speechBubble.png");

    healthTexture = loadFromFile("image/health.png");
    healthRect.h = 40;

    healthBarTexture = loadFromFile("image/guard health bar.png");
    healthBarRect.w = 210;
    healthBarRect.h = 40;

    if (rand() % 2 == 0) {
        righteous = false;
    }
    else righteous = true;

    health = rand() % (maxHealth - minHealth) + minHealth;
    damageReceived = 0;
    die = false;

    attackStrength = rand() % (maxAttackStrength - minAttackStrength) + minAttackStrength;
    attackTime = 0;
    attacking = false;
}

Guard::~Guard()
{
    //Deallocate
    free();
}

//////////////////////////////////// Character rendering functions /////////////////////////////////////////////

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
    speechBubbleRect.x = guardPosX - speechBubbleRect.w / 7;
    speechBubbleRect.y = guardPosY - speechBubbleRect.h - 70;

    SDL_Rect bubbleSpeechSpace = { NULL };
    bubbleSpeechSpace.x = speechBubbleRect.x - 15;
    bubbleSpeechSpace.y = speechBubbleRect.y - 20;
    bubbleSpeechSpace.w = speechBubbleRect.w + 35;
    bubbleSpeechSpace.h = speechBubbleRect.h + 75;

    SDL_RenderCopy(renderer, bubbleSpeechTexture, NULL, &bubbleSpeechSpace);
    SDL_RenderCopy(renderer, speechTexture, NULL, &speechBubbleRect);
}

void Guard::renderHealthBar(SDL_Renderer* renderer) {
    healthRect.x = guardPosX - 10;
    healthRect.y = guardPosY - 32;
    healthRect.w = healthBarRect.w - damageReceived * healthBarRect.w / health;
    SDL_RenderCopy(renderer, healthTexture, NULL, &healthRect);

    healthBarRect.x = healthRect.x;
    healthBarRect.y = healthRect.y;
    SDL_RenderCopy(renderer, healthBarTexture, NULL, &healthBarRect);

    guardNameRect.x = healthBarRect.x + healthBarRect.w / 2 - guardNameRect.w / 2;
    guardNameRect.y = healthBarRect.y - 7;
    SDL_RenderCopy(renderer, guardNameTexture, NULL, &guardNameRect);
}

void Guard::renderCurrentAction(SDL_Renderer* renderer) {
    //Set guard current frame
    if (SDL_GetTicks() > frameTime + nextFrameTime) {
        if (guardVelX == 0 && guardVelY == 0 && !attacking) {
            frame = guardWalk0;
        }
        else if (walking && !attacking) {
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

        frameTime = SDL_GetTicks();
    }
    
    
    SDL_Rect* currentClip = &guardSpriteClips[frame];

    renderGuard(renderer, currentClip);

    if (speechTexture != NULL) {
        renderSpeechBubble(renderer);
    }

    renderHealthBar(renderer);
}

//////////////////////////////////////////// Auto control NPC character functions /////////////////////////////////////////////////////

void Guard::randomSpeech() {

    speechLeftToSay--;

    std::string speech = "";

    if (righteous) {
        if (rand() % 2 == 0) {
            speech = goodCall[rand() % goodCall.size()];
        }

        if (rand() % 2 == 0) {
            speech += goodSentence[rand() % goodSentence.size()];
        }
        else {
            speech += goodQuestion[rand() % goodQuestion.size()];
        }
    }
    else {
        if (rand() % 2 == 0) {
            speech = notgoodCall[rand() % notgoodCall.size()];
        }

        if (rand() % 2 == 0) {
            speech += notgoodSentence[rand() % notgoodSentence.size()];
        }
        else {
            speech += notgoodQuestion[rand() % notgoodQuestion.size()];
        }
    }
    speechTexture = loadFromText(speech, &speechBubbleRect, black, textWrapLength);
    nextSpeakTime = speech.size() * timeToReadOneCharacter;
}

void Guard::moveRandom() {

    speechTexture = NULL;

    //Random right or left or neither (stand still)
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

    //Random up or down or neither
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

    moveTime = SDL_GetTicks();
    nextMoveTime = rand() % (maxNextMovetime - minNextMovetime) + minNextMovetime;
}

void Guard::move(int targetPosX, int targetPosY, int targetWidth)
{
    if (okayToSpeak && speechLeftToSay > 0 && damageReceived <= 0 && SDL_GetTicks() > speakTime + nextSpeakTime) {
        randomSpeech();
        guardVelX = 0;
        guardVelY = 0;
        moveTime = SDL_GetTicks() + nextSpeakTime;
        speakTime = SDL_GetTicks();
        attacking = false;
    }
    else if (speechLeftToSay <= 0 || damageReceived > 0) {
        guardWalkVelocity = rand() % (maxVelocity - minVelocity) + minVelocity;
        chaseTarget(targetPosX, targetPosY, targetWidth);
        attacking = true;
    }
    else if (SDL_GetTicks() > moveTime + nextMoveTime) {
        guardWalkVelocity = rand() % (maxVelocity - minVelocity) + minVelocity;
        moveRandom();
        attacking = false;
    }

    //Move
    guardPosX += (guardVelX + plusVelocity);

    guardPosY += guardVelY;

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

void Guard::receiveAttack(int damage) {
    damageReceived += damage;
    if (damageReceived >= health) {
        die = true;
    }

    std::string speech = angrySpeech[rand() % angrySpeech.size()];
    speechTexture = loadFromText(speech, &speechBubbleRect, black, textWrapLength);

    nextSpeakTime = speech.size() * timeToReadOneCharacter;
}

bool Guard::isDead() {
    return die;
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

    if (guardPosY < targetPosY + approxDistant) {
        guardVelY = guardWalkVelocity;
    }
    else if (guardPosY > targetPosY - approxDistant) {
        guardVelY = -guardWalkVelocity;
    }
    else {
        guardVelY = 0;
    }
}

bool Guard::isAttacking() {
    return attacking;
}

int Guard::getAttackDamage() {
    if (SDL_GetTicks() > attackTime + nextAttackTime) {
        attackStrength = rand() % (maxAttackStrength - minAttackStrength) + minAttackStrength;
    }
    else {
        attackStrength = 0;
    }
    return attackStrength;
}

void Guard::free()
{
    freeTexture(guardNameTexture);
    freeTexture(guardTexture);
    freeTexture(speechTexture);
    freeTexture(bubbleSpeechTexture);
    freeTexture(healthTexture);
    freeTexture(healthBarTexture);
    guardName = { NULL };
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

