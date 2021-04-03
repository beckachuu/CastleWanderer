#include "guard.h"
#include "speech_data.h"
#include <SDL_image.h>
#include <ctime>

Guard::Guard(SDL_Renderer* renderer)
{
    srand(time(0));
    //Initialize
    guardTexture = NULL;
    gWidth = 0;
    gHeight = 0;

    //Initialize the offsets
    ground = baseGround;
    rightmostGuardPos = SCREEN_WIDTH;
    guardPosX = rand() % (rightmostGuardPos - leftMostGuardPos) + leftMostGuardPos;
    guardPosY = rand() % (baseGround - walkLimit) + walkLimit;

    //Initialize the velocity
    guardVelX = 0;
    guardVelY = 0;

    frame = walk0;
    nextOrBackFrame = -1;

    frameTime = 0;
    moveTime = 0;
    speakTime = 0;

    toRight = false;
    //Default attack left
    toLeft = true;
    walking = false;
    jumped = false;

    if (rand() % 2 == 0) {
        righteous = false;
    }
    else righteous = true;

    //Assume righteous
    righteous = true;

    renderSpeech = { NULL };
    speechTexture = NULL;

    health = rand() % 20 + 50;

    //Load textures
    guardTexture = loadFromFile("image/guardSheet.png", renderer);
    setSpriteClips();

    bubbleSpeechTexture = loadFromFile("image/speechBubble.jpg", renderer);
}

Guard::~Guard()
{
    //Deallocate
    free();
}

//////////////////////////////////// Character rendering functions /////////////////////////////////////////////

void Guard::setSpriteClips() {
    //Set sprite clips
    guardSpriteClips[walk0].x = 4;
    guardSpriteClips[walk0].y = 47;
    guardSpriteClips[walk0].w = 85;
    guardSpriteClips[walk0].h = 195;

    guardSpriteClips[walk1].x = 112;
    guardSpriteClips[walk1].y = 47;
    guardSpriteClips[walk1].w = 85;
    guardSpriteClips[walk1].h = 195;

    guardSpriteClips[walk2].x = 229;
    guardSpriteClips[walk2].y = 47;
    guardSpriteClips[walk2].w = 101;
    guardSpriteClips[walk2].h = 195;

    guardSpriteClips[walk3].x = 347;
    guardSpriteClips[walk3].y = 47;
    guardSpriteClips[walk3].w = 86;
    guardSpriteClips[walk3].h = 195;

    guardSpriteClips[walk4].x = 456;
    guardSpriteClips[walk4].y = 47;
    guardSpriteClips[walk4].w = 102;
    guardSpriteClips[walk4].h = 195;

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
    SDL_Rect renderGuard = { guardPosX, guardPosY, gWidth, gHeight };

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

    //Render speech
    renderSpeech.x = guardPosX + renderGuard.w / 2 - renderSpeech.w / 2;
    renderSpeech.y = guardPosY - renderSpeech.h - speechOffset;
    SDL_RenderCopy(renderer, speechTexture, NULL, &renderSpeech);
}

void Guard::renderCurrentAction(SDL_Renderer* renderer) {
    if (walking) {
        if (SDL_GetTicks() > frameTime + nextFrameTime) {
            if (frame >= walk4)
            {
                nextOrBackFrame = -1;
            }
            else if (frame <= walk0) {
                nextOrBackFrame = 1;
            }
            frame += nextOrBackFrame;
            frameTime = SDL_GetTicks();
        }
    }
    else {
        frame = walk0;
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
            speech += good_call[rand() % speechCount];
        }
        if (rand() % 2 == 0) {
            speech += (good_s13[rand() % speechCount] + good_v13[rand() % speechCount] + good_o13[rand() % speechCount]);
        }
        else {
            speech += (good_s2[rand() % speechCount] + good_v2[rand() % speechCount] + good_o2[rand() % speechCount]);
        }
    }
    

    speechTexture = loadFromRenderedText(speech, &renderSpeech, textWrapLength);
}

void Guard::moveRandom() {
    
    //Random right or left or neither (stand still)
    int moveX = rand() % 4; //Guard has 50% to stand still on X-axis
    if (moveX == 0) {
        guardVelX = gVelocity;
        toLeft = false;
        toRight = true;
    }
    else if (moveX == 1) {
        guardVelX = -gVelocity;
        toLeft = true;
        toRight = false;
    }
    else {
        guardVelX = 0;
    }

    //Random up or down or neither
    int moveY = rand() % 4; //Guard has 50% to stand still on Y-axis
    if (moveY == 0) {
        guardVelY = gVelocity;
    }
    else if (moveY == 1) {
        guardVelY = -gVelocity;
    }
    else {
        guardVelY = 0;
    }

    moveTime = SDL_GetTicks();
}

void Guard::move()
{
    if (SDL_GetTicks() > speakTime + nextSpeakTime) {
        randomSpeech();
        speakTime = SDL_GetTicks();
    }
    if (SDL_GetTicks() > moveTime + rand() % maxNextMovetime + minNextMovetime) {
        //moveRandom();
    }

    //Move right or left
    guardPosX += (guardVelX + plusVelocity);
    walking = true;

    //If went too far to the right or left
    if ((guardPosX < leftMostGuardPos) || (guardPosX > rightmostGuardPos))
    {
        guardPosX -= guardVelX;
    }

    //Move up or down
    if (guardVelY != 0) {
        guardPosY += guardVelY;
    }

    //If jumped too far up
    if (jumped && guardPosY < ground - jumpHeight) {
        guardVelY += gVelocityJump;
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

    //If not walking anywhere
    if (guardVelX == 0 && guardVelY == 0) {
        walking = false;
        frame = walk0;
    }

}


void Guard::free()
{
    freeTexture(guardTexture);
}

int Guard::getguardPosX() {
    return guardPosX;
}

int Guard::getguardPosY() {
    return guardPosY;
}

void Guard::setPlusVelocity(int bgVelocity) {
    plusVelocity = bgVelocity;
}

void Guard::setRightLimit(int bgLeftMostPos) {
    rightmostGuardPos = -bgLeftMostPos;
}

int Guard::getguardVelX() {
    return guardVelX;
}

int Guard::getguardVelY() {
    return guardVelY;
}

int Guard::getgWidth() {
    return gWidth;
}

int Guard::getgHeight() {
    return gHeight;
}
