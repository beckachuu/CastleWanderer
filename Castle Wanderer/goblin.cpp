#include "goblin.h"
#include "goblin_speech.h"
#include <SDL_image.h>

Goblin::Goblin()
{
    frameTime = 0;

    moveTime = 0;
    nextMoveTime = 0;

    speakTime = 0;
    nextSpeakTime = rand() % maxNextSpeakTime + minNextSpeakTime;
    eraseSpeechTime = 0;


    goblinTexture = loadFromImage("image/Sprite sheets/goblinSheet.png");
    setSpriteClips();

    bubbleSpeechTexture = loadFromImage("image/speechBubble.png");
    bubbleSpeechRect = { NULL };

    speechTexture = NULL;
    speechRect = { NULL };


    rightmostGoblinPos = SCREEN_WIDTH * 4;
    leftmostGoblinPos = 0;

    reviveGoblin();

    goblinWalkVelocity = 5;
    plusVelocity = 0;

    goblinWidth = 0;
    goblinHeight = 0;

    toRight = false;
    toLeft = true;
    walking = false;

}

void Goblin::reviveGoblin() {
    frame = walk0;
    nextOrBackFrame = 1;

    do {
        goblinPosX = rand() % (rightmostGoblinPos - leftmostGoblinPos) + leftmostGoblinPos;
    } while (goblinPosX > -150 && goblinPosX < SCREEN_WIDTH);

    goblinPosY = rand() % (baseGround - walkLimit) + walkLimit;

    goblinVelX = 0;
    goblinVelY = 0;

    angry = false;
    explodeDamage = 0;
    exploded = false;
}

Goblin::~Goblin()
{
    free();
}



void Goblin::setSpriteClips() {
    //Set sprite clips
    goblinSpriteClips[walk0].x = 20;
    goblinSpriteClips[walk0].y = 6;
    goblinSpriteClips[walk0].w = 65;
    goblinSpriteClips[walk0].h = 125;

    goblinSpriteClips[walk1].x = 125;
    goblinSpriteClips[walk1].y = 6;
    goblinSpriteClips[walk1].w = 62;
    goblinSpriteClips[walk1].h = 125;

    goblinSpriteClips[walk2].x = 238;
    goblinSpriteClips[walk2].y = 6;
    goblinSpriteClips[walk2].w = 76;
    goblinSpriteClips[walk2].h = 125;

    goblinSpriteClips[walk3].x = 352;
    goblinSpriteClips[walk3].y = 6;
    goblinSpriteClips[walk3].w = 70;
    goblinSpriteClips[walk3].h = 125;

    goblinSpriteClips[walk4].x = 455;
    goblinSpriteClips[walk4].y = 6;
    goblinSpriteClips[walk4].w = 78;
    goblinSpriteClips[walk4].h = 125;

    goblinSpriteClips[angry1].x = 133;
    goblinSpriteClips[angry1].y = 306;
    goblinSpriteClips[angry1].w = 67;
    goblinSpriteClips[angry1].h = 127;

    goblinSpriteClips[angry2].x = 250;
    goblinSpriteClips[angry2].y = 306;
    goblinSpriteClips[angry2].w = 67;
    goblinSpriteClips[angry2].h = 127;

    goblinSpriteClips[angry3].x = 361;
    goblinSpriteClips[angry3].y = 306;
    goblinSpriteClips[angry3].w = 67;
    goblinSpriteClips[angry3].h = 127;

    goblinSpriteClips[angry4].x = 35;
    goblinSpriteClips[angry4].y = 476;
    goblinSpriteClips[angry4].w = 65;
    goblinSpriteClips[angry4].h = 116;

    goblinSpriteClips[angry5].x = 141;
    goblinSpriteClips[angry5].y = 476;
    goblinSpriteClips[angry5].w = 65;
    goblinSpriteClips[angry5].h = 116;

    goblinSpriteClips[explode].x = 241;
    goblinSpriteClips[explode].y = 472;
    goblinSpriteClips[explode].w = 112;
    goblinSpriteClips[explode].h = 124;

}

void Goblin::renderGoblin(SDL_Renderer* renderer, SDL_Rect* clip)
{
    if (clip != NULL)
    {
        goblinWidth = clip->w;
        goblinHeight = clip->h;
    }
    SDL_Rect renderGoblin = { goblinPosX, goblinPosY, goblinWidth, goblinHeight };

    if (toLeft == true) {
        SDL_RenderCopyEx(renderer, goblinTexture, clip, &renderGoblin, 0, NULL, SDL_FLIP_HORIZONTAL);
    }
    else {
        SDL_RenderCopy(renderer, goblinTexture, clip, &renderGoblin);
    }
}

void Goblin::renderGoblinSpeech(SDL_Renderer* renderer) {
    if (speechTexture != NULL) {

        speechRect.x = goblinPosX - 20;
        speechRect.y = goblinPosY - speechRect.h - 50;

        bubbleSpeechRect.x = speechRect.x - 17;
        bubbleSpeechRect.y = speechRect.y - 20;
        bubbleSpeechRect.w = speechRect.w + 50;
        bubbleSpeechRect.h = (speechRect.h + 40) * 6 / 5;

        SDL_RenderCopy(renderer, bubbleSpeechTexture, NULL, &bubbleSpeechRect);
        SDL_RenderCopy(renderer, speechTexture, NULL, &speechRect);
    }
}

void Goblin::renderCurrentAction(SDL_Renderer* renderer, unsigned int currentTime) {
    if (walking && frame < angry1) {
        if (currentTime > frameTime + nextFrameTime) {
            if (frame >= walk4)
            {
                nextOrBackFrame = -1;
            }
            else if (frame <= walk0) {
                nextOrBackFrame = 1;
            }
            frame += nextOrBackFrame;
            frameTime = currentTime;
        }
    }
    else if (!walking && frame < angry1) {
        frame = walk0;
        frameTime = currentTime;
    }
    else if (frame >= angry1 && frame != explode) {
        if (currentTime > frameTime + nextFrameTime * 3) {
            frame++;
            frameTime = currentTime;

            if (frame > angry5) {
                angry = true;
                frame = walk0;
                frameTime = currentTime;
            }
        }
    }
    else if (frame == explode) {
        if (currentTime > frameTime + nextFrameTime * 2) {
            exploded = true;
        }
    }

    SDL_Rect* currentClip = &goblinSpriteClips[frame];
    renderGoblin(renderer, currentClip);

    renderGoblinSpeech(renderer);
}



void Goblin::randomSpeech() {
    std::string speech = informSpeech[rand() % informSpeech.size()];
    eraseSpeechTime = speech.size() * timeToReadOneCharacter;
    speechTexture = loadFromText(speech, &speechRect, black, textWrapLength);
}

void Goblin::moveRandom(unsigned int currentTime) {

    //Random right or left or neither (stand still)
    int moveX = rand() % 3;
    if (moveX == 0) {
        goblinVelX = goblinWalkVelocity;
        toLeft = false;
        toRight = true;
    }
    else if (moveX == 1) {
        goblinVelX = -goblinWalkVelocity;
        toLeft = true;
        toRight = false;
    }
    else {
        goblinVelX = 0;
    }

    //Random up or down or neither
    int moveY = rand() % 3;
    if (moveY == 0) {
        goblinVelY = goblinWalkVelocity;
    }
    else if (moveY == 1) {
        goblinVelY = -goblinWalkVelocity;
    }
    else {
        goblinVelY = 0;
    }

    moveTime = currentTime;
}

void Goblin::move(int targetPosX, int targetPosY, int targetWidth, int targetHeight, unsigned int currentTime)
{
    if (!angry && frame < angry1) {
        if (currentTime > moveTime + nextMoveTime) {
            moveRandom(currentTime);
            moveTime = currentTime;
            nextMoveTime = rand() % maxNextMoveTime + minNextMoveTime;
        }

        if (currentTime > speakTime + nextSpeakTime) {
            randomSpeech();
            goblinVelX = 0;
            goblinVelY = 0;

            nextMoveTime += eraseSpeechTime;

            speakTime = currentTime;
            nextSpeakTime = rand() % maxNextSpeakTime + minNextSpeakTime;
        }
    }
    else if (angry) {
        chaseTarget(targetPosX, targetPosY, targetWidth, targetHeight);
    }

    if (currentTime > speakTime + eraseSpeechTime) {
        speechTexture = NULL;
    }

    if (goblinVelX == 0 && goblinVelY == 0 && frame < angry1) {
        walking = false;
        frame = walk0;
    }
    else walking = true;

    goblinPosX += (goblinVelX + plusVelocity);

    if (goblinVelY != 0) {
        goblinPosY += goblinVelY;
    }

    checkGoblinLimits();
}

void Goblin::checkGoblinLimits() {
    //If went too far to the right or left
    if ((goblinPosX < leftmostGoblinPos) || (goblinPosX > rightmostGoblinPos))
    {
        goblinPosX -= goblinVelX;
    }

    //If walked too far up
    if (walking && goblinPosY < walkLimit) {
        goblinPosY = walkLimit;
    }
    //If got too far down
    if (goblinPosY > baseGround) {
        goblinPosY = baseGround - 1;
    }
}

void Goblin::getAngry(unsigned int currentTime) {
    if (frame < angry1) {
        frame = angry1;
        frameTime = currentTime;

        goblinVelX = 0;
        goblinVelY = 0;
    }

    std::string speech = angrySpeech[rand() % angrySpeech.size()];
    speechTexture = loadFromText(speech, &speechRect, black, textWrapLength);

    speakTime = currentTime;
    eraseSpeechTime = speech.size() * timeToReadOneCharacter;
}

bool Goblin::isAngry() {
    return angry;
}

void Goblin::chaseTarget(int targetPosX, int targetPosY, int targetWidth, int targetHeight) {
    if (goblinPosX + goblinWidth < targetPosX + approxDistant) {
        goblinVelX = goblinWalkVelocity;
        toLeft = false;
        toRight = true;
    }
    else if (goblinPosX > targetPosX + targetWidth - approxDistant) {
        goblinVelX = -goblinWalkVelocity;
        toLeft = true;
        toRight = false;
    }
    else {
        goblinVelX = 0;
    }

    if (goblinPosY + goblinHeight < targetPosY + targetHeight - approxDistant) {
        goblinVelY = goblinWalkVelocity;
    }
    else if (goblinPosY + goblinHeight > targetPosY + targetHeight + approxDistant) {
        goblinVelY = -goblinWalkVelocity;
    }
    else {
        goblinVelY = 0;
    }

    if (goblinVelX == 0 && goblinVelY == 0 && frame != explode) {
        frame = explode;
        explodeDamage = rand() % (maxExplodeDamage - minExplodeDamage) + minExplodeDamage;
    }
}

int Goblin::getExplodeDamage() {
    int tempExpDamage = explodeDamage;
    explodeDamage = 0;
    return tempExpDamage;
}



bool Goblin::isDead() {
    return exploded;
}



int Goblin::getGoblinPosX() {
    return goblinPosX;
}
int Goblin::getGoblinPosY() {
    return goblinPosY;
}

void Goblin::setPlusVelocity(int bgVelocity) {
    plusVelocity = bgVelocity;
    leftmostGoblinPos += plusVelocity;
    rightmostGoblinPos += plusVelocity;
}

int Goblin::getGoblinVelX() {
    return goblinVelX;
}
int Goblin::getGoblinVelY() {
    return goblinVelY;
}

int Goblin::getGoblinWidth() {
    return goblinWidth;
}
int Goblin::getGoblinHeight() {
    return goblinHeight;
}

void Goblin::free()
{
    freeTexture(goblinTexture);
    freeTexture(speechTexture);
    freeTexture(bubbleSpeechTexture);
}
