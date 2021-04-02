#include "guard.h"
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
    guardPosX = rand() % (rightmostGuardPos - leftMostGuardPos) + leftMostGuardPos;
    guardPosY = rand() % (baseGround - walkLimit) + walkLimit;

    //Initialize the velocity
    guardVelX = 0;
    guardVelY = 0;

    frame = walk0;
    frameTime = 0;
    nextOrBackFrame = -1;
    moveTime = 0;
    tempTime = 0;

    toRight = false;
    //Default attack left
    toLeft = true;
    walking = false;
    jumped = false;

    if (rand() % 2 == 0) {
        righteous = false;
    }
    else righteous = true;

    loadText(speechTexture, "string here");

    health = rand() % 20 + 50;

    if (!loadFromFile("image/guardSheet.png", renderer)) {
        std::cout << "Failed to load guard." << std::endl;
    }
    else setSpriteClips();
}

Guard::~Guard()
{
    //Deallocate
    free();
}

bool Guard::loadFromFile(std::string path, SDL_Renderer* renderer)
{
    //Get rid of preexisting texture
    free();

    //The final texture
    SDL_Texture* newTexture = NULL;

    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL)
    {
        std::cerr << "Unable to load image! SDL_image Error:" << IMG_GetError() << std::endl;
    }
    else
    {
        //Color key image
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 77, 79, 86));

        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if (newTexture == NULL)
        {
            std::cerr << "Unable to create texture! SDL Error: " << SDL_GetError() << std::endl;
        }
        else
        {
            //Get image dimensions
            gWidth = loadedSurface->w;
            gHeight = loadedSurface->h;
        }

        //Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }

    //Return success
    guardTexture = newTexture;

    return (guardTexture != NULL);
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
    SDL_Rect renderQuad = { guardPosX, guardPosY, gWidth, gHeight };

    //Set clip rendering dimensions
    if (clip != NULL)
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    //Render character to screen
    if (toLeft == true) {
        SDL_RenderCopyEx(renderer, guardTexture, clip, &renderQuad, 0, NULL, SDL_FLIP_HORIZONTAL);
    }
    else {
        SDL_RenderCopy(renderer, guardTexture, clip, &renderQuad);
    }

    //Render speech
    SDL_Rect renderSpeech = { guardPosX, guardPosY - 50, 150, 50 };
    SDL_RenderCopy(renderer, speechTexture, NULL, NULL);
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

//////////////////////////////////// Auto control NPC character functions /////////////////////////////////////////////

void Guard::randomSpeech() {
    std::string speech = "Heyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy";
    loadText(speechTexture, speech);
    if (speechTexture == nullptr) {
        std::cerr << "Dude the speechTexture is NULL" << std::endl;
    }
}

void Guard::moveRandom() {
    if (SDL_GetTicks() > moveTime + rand() % maxNextMovetime + minNextMovetime) {
        
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
}

void Guard::move()
{
    moveRandom();

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

    std::cout << leftMostGuardPos << " " << rightmostGuardPos << std::endl;
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
