#include "goblin.h"
#include "goblin_speech.h"
#include <SDL_image.h>

Goblin::Goblin()
{
    //Initialize
    goblinTexture = NULL;
    goblinWidth = 0;
    goblinHeight = 0;

    //Initialize the offsets
    ground = baseGround;
    rightmostGoblinPos = SCREEN_WIDTH * 4;
    leftmostGoblinPos = 0;
    goblinPosX = rand() % (rightmostGoblinPos - leftmostGoblinPos) + leftmostGoblinPos;
    goblinPosY = rand() % (baseGround - walkLimit) + walkLimit;

    //Initialize the velocity
    goblinWalkVelocity = 8;
    goblinJumpVelocity = 32;
    goblinVelX = 0;
    goblinVelY = 0;

    frame = walk0;
    nextOrBackFrame = -1;

    frameTime = 0;
    moveTime = 0;
    nextMoveTime = 0;

    speakTime = 0;
    nextSpeakTime = rand() % maxNextSpeakTime + minNextSpeakTime;
    eraseSpeechTime = 0;

    toRight = false;
    //Default attack left
    toLeft = true;
    walking = false;
    jumped = false;

    renderSpeech = { NULL };
    speechTexture = NULL;

    health = rand() % 20 + 50;

    //Load textures
    goblinTexture = loadFromFile("image/goblinSheet.png");
    setSpriteClips();

    bubbleSpeechTexture = loadFromFile("image/speechBubble.png");
}

Goblin::~Goblin()
{
    //Deallocate
    free();
}

//////////////////////////////////// Character rendering functions /////////////////////////////////////////////

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

    goblinSpriteClips[angry1].x = 331;
    goblinSpriteClips[angry1].y = 263;
    goblinSpriteClips[angry1].w = 119;
    goblinSpriteClips[angry1].h = 195;

    goblinSpriteClips[angry2].x = 462;
    goblinSpriteClips[angry2].y = 263;
    goblinSpriteClips[angry2].w = 119;
    goblinSpriteClips[angry2].h = 195;

    goblinSpriteClips[angry3].x = 462;
    goblinSpriteClips[angry3].y = 263;
    goblinSpriteClips[angry3].w = 119;
    goblinSpriteClips[angry3].h = 195;

    goblinSpriteClips[angry4].x = 462;
    goblinSpriteClips[angry4].y = 263;
    goblinSpriteClips[angry4].w = 119;
    goblinSpriteClips[angry4].h = 195;

    goblinSpriteClips[angry5].x = 462;
    goblinSpriteClips[angry5].y = 263;
    goblinSpriteClips[angry5].w = 119;
    goblinSpriteClips[angry5].h = 195;

    goblinSpriteClips[explode].x = 462;
    goblinSpriteClips[explode].y = 263;
    goblinSpriteClips[explode].w = 119;
    goblinSpriteClips[explode].h = 195;

}

void Goblin::render(SDL_Renderer* renderer, SDL_Rect* clip)
{
    //Set rendering space and render to screen
    SDL_Rect renderGoblin = { goblinPosX, goblinPosY, goblinWidth, goblinHeight };

    //Set clip rendering dimensions
    if (clip != NULL)
    {
        renderGoblin.w = clip->w;
        renderGoblin.h = clip->h;
    }

    //Render character to screen
    if (toLeft == true) {
        SDL_RenderCopyEx(renderer, goblinTexture, clip, &renderGoblin, 0, NULL, SDL_FLIP_HORIZONTAL);
    }
    else {
        SDL_RenderCopy(renderer, goblinTexture, clip, &renderGoblin);
    }

    if (speechTexture != NULL) {
        //Render speech and bubble speech
        renderSpeech.x = goblinPosX - renderSpeech.w / 7;
        renderSpeech.y = goblinPosY - renderSpeech.h - 45;
        SDL_Rect bubbleSpeechSpace = { NULL };
        bubbleSpeechSpace.x = renderSpeech.x - 15;
        bubbleSpeechSpace.y = renderSpeech.y - 20;
        bubbleSpeechSpace.w = renderSpeech.w + 35;
        bubbleSpeechSpace.h = renderSpeech.h + 70;
        SDL_RenderCopy(renderer, bubbleSpeechTexture, NULL, &bubbleSpeechSpace);
        SDL_RenderCopy(renderer, speechTexture, NULL, &renderSpeech);
    }
}

void Goblin::renderCurrentAction(SDL_Renderer* renderer) {
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

    SDL_Rect* currentClip = &goblinSpriteClips[frame];
    render(renderer, currentClip);
}

//////////////////////////////////////////// Auto control NPC character functions /////////////////////////////////////////////////////

void Goblin::randomSpeech() {
    std::string speech = informSpeech[rand() % informSpeech.size()];
    eraseSpeechTime = speech.size() * timeToReadOneCharacter;
    speechTexture = loadFromText(speech, &renderSpeech, black, textWrapLength);
}

void Goblin::moveRandom() {

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

    moveTime = SDL_GetTicks();
}

void Goblin::move()
{
    if (SDL_GetTicks() > moveTime + nextMoveTime) {
        moveRandom();
        speechTexture = NULL;

        moveTime = SDL_GetTicks();
        nextMoveTime = rand() % maxNextMoveTime + minNextMoveTime;
    }

    if (SDL_GetTicks() > speakTime + nextSpeakTime) {
        randomSpeech();
        goblinVelX = 0;
        goblinVelY = 0;

        nextMoveTime += eraseSpeechTime;

        speakTime = SDL_GetTicks();
        nextSpeakTime = rand() % maxNextSpeakTime + minNextSpeakTime;
    }

    //If not walking anywhere
    if (goblinVelX == 0 && goblinVelY == 0) {
        walking = false;
        frame = walk0;
    }
    else walking = true;

    //Move right or left
    goblinPosX += (goblinVelX + plusVelocity);

    //If went too far to the right or left
    if ((goblinPosX < leftmostGoblinPos) || (goblinPosX > rightmostGoblinPos))
    {
        goblinPosX -= goblinVelX;
    }

    //Move up or down
    if (goblinVelY != 0) {
        goblinPosY += goblinVelY;
    }

    //If jumped too far up
    if (jumped && goblinPosY < ground - jumpHeight) {
        goblinVelY += goblinJumpVelocity;
    }
    //If jumped and fell back to where standing
    if (jumped && goblinPosY > ground) {
        goblinVelY = 0;
        jumped = false;
    }

    //If walked too far up
    if (walking && !jumped && goblinPosY < walkLimit) {
        goblinPosY = walkLimit;
    }
    //If got too far down
    if (goblinPosY > baseGround) {
        goblinPosY = baseGround - 1;
    }

}

void Goblin::moveBackX(int vel) {
    if (goblinVelX > 0) {
        goblinPosX -= vel;
    }
    else if (goblinVelX < 0) {
        goblinPosX += vel;
    }
}

void Goblin::moveBackY(int vel) {
    if (goblinVelY > 0) {
        goblinPosY -= vel;
    }
    else if (goblinVelY < 0) {
        goblinPosY += vel;
    }
}

void Goblin::free()
{
    freeTexture(goblinTexture);
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

void Goblin::setVelocity(int bgVelocity) {
    goblinWalkVelocity = bgVelocity + 2;
    goblinJumpVelocity = bgVelocity * 4;
}

int Goblin::getGoblinWidth() {
    return goblinSpriteClips[frame].w;
}

int Goblin::getGoblinHeight() {
    return goblinSpriteClips[frame].h;
}
