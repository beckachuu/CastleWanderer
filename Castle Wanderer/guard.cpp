#include "guard.h"
#include "guard_speech.h"
#include <SDL_image.h>

Guard::Guard()
{
    //Initialize
    guardTexture = NULL;
    guardWidth = 0;
    guardHeight = 0;

    //Initialize the offsets
    ground = baseGround;
    rightmostGuardPos = SCREEN_WIDTH * 4;
    leftmostGuardPos = 0;
    guardPosX = rand() % (rightmostGuardPos - leftmostGuardPos) + leftmostGuardPos;
    guardPosY = rand() % (baseGround - walkLimit) + walkLimit;

    //Initialize the velocity
    guardWalkVelocity = 7;
    guardJumpVelocity = 28;
    guardVelX = 0;
    guardVelY = 0;

    frame = guardWalk0;
    nextOrBackFrame = -1;

    frameTime = 0;
    moveTime = 0;
    nextMoveTime = rand() % maxNextMovetime + minNextMovetime;

    speakTime = 0;
    okayToSpeak = false;

    toRight = false;
    //Default attack left
    toLeft = true;
    walking = false;
    jumped = false;

    if (rand() % 2 == 0) {
        righteous = false;
    }
    else righteous = true;

    renderSpeech = { NULL };
    speechTexture = NULL;

    health = rand() % 20 + 50;

    //Load textures
    guardTexture = loadFromFile("image/guardSheet.png");
    setSpriteClips();

    bubbleSpeechTexture = loadFromFile("image/speechBubble.png");
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

void Guard::render(SDL_Renderer* renderer, SDL_Rect* clip)
{
    //Set rendering space and render to screen
    SDL_Rect renderGuard = { guardPosX, guardPosY, guardWidth, guardHeight };

    //Set clip rendering dimensions
    if (clip != NULL)
    {
        renderGuard.w = clip->w;
        renderGuard.h = clip->h;
    }

    //Render character to screen
    if (toLeft == true) {
        SDL_RenderCopyEx(renderer, guardTexture, clip, &renderGuard, 0, NULL, SDL_FLIP_HORIZONTAL);
    }
    else {
        SDL_RenderCopy(renderer, guardTexture, clip, &renderGuard);
    }

    if (speechTexture != NULL) {
        //Render speech and bubble speech
        renderSpeech.x = guardPosX - renderSpeech.w / 7;
        renderSpeech.y = guardPosY - renderSpeech.h - 45;
        SDL_Rect bubbleSpeechSpace = { NULL };
        bubbleSpeechSpace.x = renderSpeech.x - 15;
        bubbleSpeechSpace.y = renderSpeech.y - 20;
        bubbleSpeechSpace.w = renderSpeech.w + 35;
        bubbleSpeechSpace.h = renderSpeech.h + 70;
        SDL_RenderCopy(renderer, bubbleSpeechTexture, NULL, &bubbleSpeechSpace);
        SDL_RenderCopy(renderer, speechTexture, NULL, &renderSpeech);
    }
}

void Guard::renderCurrentAction(SDL_Renderer* renderer) {
    if (walking) {
        if (SDL_GetTicks() > frameTime + nextFrameTime) {
            if (frame >= guardWalk4)
            {
                nextOrBackFrame = -1;
            }
            else if (frame <= guardWalk0) {
                nextOrBackFrame = 1;
            }
            frame += nextOrBackFrame;
            frameTime = SDL_GetTicks();
        }
    }
    else {
        frame = guardWalk0;
        frameTime = SDL_GetTicks();
    }

    SDL_Rect* currentClip = &guardSpriteClips[frame];
    render(renderer, currentClip);
}

//////////////////////////////////////////// Auto control NPC character functions /////////////////////////////////////////////////////

void Guard::randomSpeech() {
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
    speechTexture = loadFromText(speech, &renderSpeech, textWrapLength);
}

void Guard::moveRandom() {
    
    //Random right or left or neither (stand still)
    int moveX = rand() % 3;
    if (moveX == 0) {
        guardVelX = guardWalkVelocity;
        toLeft = false;
        toRight = true;
    }
    else if (moveX == 1) {
        guardVelX = -guardWalkVelocity;
        toLeft = true;
        toRight = false;
    }
    else {
        guardVelX = 0;
    }

    //Random up or down or neither
    int moveY = rand() % 3;
    if (moveY == 0) {
        guardVelY = guardWalkVelocity;
    }
    else if (moveY == 1) {
        guardVelY = -guardWalkVelocity;
    }
    else {
        guardVelY = 0;
    }

    moveTime = SDL_GetTicks();
}

void Guard::move()
{
    if (okayToSpeak && SDL_GetTicks() > speakTime + nextSpeakTime) {
        randomSpeech();
        guardVelX = 0;
        guardVelY = 0;
        moveTime = SDL_GetTicks() + nextSpeakTime;
        speakTime = SDL_GetTicks();
    }
    
    if (SDL_GetTicks() > moveTime + nextMoveTime) {
        moveRandom();
        speechTexture = NULL;
        nextMoveTime = rand() % maxNextMovetime + minNextMovetime;
    }

    //If not walking anywhere
    if (guardVelX == 0 && guardVelY == 0) {
        walking = false;
        frame = guardWalk0;
    }
    else walking = true;

    //Move right or left
    guardPosX += (guardVelX + plusVelocity);

    //If went too far to the right or left
    if ((guardPosX < leftmostGuardPos) || (guardPosX > rightmostGuardPos))
    {
        guardPosX -= guardVelX;
    }

    //Move up or down
    if (guardVelY != 0) {
        guardPosY += guardVelY;
    }

    //If jumped too far up
    if (jumped && guardPosY < ground - jumpHeight) {
        guardVelY += guardJumpVelocity;
    }
    //If jumped and fell back to where standing
    if (jumped && guardPosY > ground) {
        guardVelY = 0;
        jumped = false;
    }

    //If walked too far up
    if (walking && !jumped && guardPosY < walkLimit) {
        guardPosY = walkLimit;
    }
    //If got too far down
    if (guardPosY > baseGround) {
        guardPosY = baseGround - 1;
    }

}

void Guard::moveBackX(int vel) {
    if (guardVelX > 0) {
        guardPosX -= vel;
    }
    else if (guardVelX < 0) {
        guardPosX += vel;
    }
}

void Guard::moveBackY(int vel) {
    if (guardVelY > 0) {
        guardPosY -= vel;
    }
    else if (guardVelY < 0) {
        guardPosY += vel;
    }
}

void Guard::free()
{
    freeTexture(guardTexture);
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

void Guard::setRightLimit(int bgLeftMostPos) {
    rightmostGuardPos = -bgLeftMostPos;
    //leftmostGuardPos=
}

int Guard::getGuardVelX() {
    return guardVelX;
}

int Guard::getGuardVelY() {
    return guardVelY;
}

void Guard::setVelocity(int BguardWalkVelocity) {
    guardWalkVelocity = BguardWalkVelocity + 1;
    guardJumpVelocity = guardWalkVelocity * 4;
}

int Guard::getGuardWidth() {
    return guardSpriteClips[frame].w;
}

int Guard::getGuardHeight() {
    return guardSpriteClips[frame].h;
}
