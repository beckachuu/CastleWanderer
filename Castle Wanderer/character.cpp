#include "character.h"
#include <SDL_image.h>

myCharacter::myCharacter()
{
    //Initialize
    cTexture = NULL;
    cWidth = 0;
    cHeight = 0;

    //Initialize the offsets
    ground = baseGround;
    cPosX = SCREEN_WIDTH / 2;
    cPosY = baseGround-1;

    //Initialize the velocity
    cVelX = 0;
    cVelY = 0;

    frame = stand;

    frameTime = 0;

    toRight = false;
    //Default attack left
    toLeft = true;
    walking = false;
    jumped = false;

    gotToFar = false;

    health = 120;
}

myCharacter::~myCharacter()
{
    //Deallocate
    free();
}

bool myCharacter::loadFromFile(std::string path, SDL_Renderer* renderer)
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
            cWidth = loadedSurface->w;
            cHeight = loadedSurface->h;
        }

        //Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }

    //Return success
    cTexture = newTexture;

    return (cTexture != NULL);
}

//////////////////////////////////// Character rendering functions /////////////////////////////////////////////

void myCharacter::setSpriteClips() {
    //Set sprite clips

    characterSpriteClips[walkR1].x = 24;
    characterSpriteClips[walkR1].y = 218;
    characterSpriteClips[walkR1].w = 99;
    characterSpriteClips[walkR1].h = 195;

    characterSpriteClips[walkR2].x = 126;
    characterSpriteClips[walkR2].y = 218;
    characterSpriteClips[walkR2].w = 104;
    characterSpriteClips[walkR2].h = 195;

    characterSpriteClips[walkR3].x = 238;
    characterSpriteClips[walkR3].y = 218;
    characterSpriteClips[walkR3].w = 113;
    characterSpriteClips[walkR3].h = 195;

    characterSpriteClips[walkR4].x = 360;
    characterSpriteClips[walkR4].y = 218;
    characterSpriteClips[walkR4].w = 121;
    characterSpriteClips[walkR4].h = 195;

    characterSpriteClips[walkR5].x = 482;
    characterSpriteClips[walkR5].y = 218;
    characterSpriteClips[walkR5].w = 107;
    characterSpriteClips[walkR5].h = 195;

    characterSpriteClips[walkR6].x = 589;
    characterSpriteClips[walkR6].y = 218;
    characterSpriteClips[walkR6].w = 97;
    characterSpriteClips[walkR6].h = 195;

    characterSpriteClips[walkL6].x = 15;
    characterSpriteClips[walkL6].y = 420;
    characterSpriteClips[walkL6].w = 99;
    characterSpriteClips[walkL6].h = 195;

    characterSpriteClips[walkL5].x = 113;
    characterSpriteClips[walkL5].y = 420;
    characterSpriteClips[walkL5].w = 107;
    characterSpriteClips[walkL5].h = 195;

    characterSpriteClips[walkL4].x = 224;
    characterSpriteClips[walkL4].y = 420;
    characterSpriteClips[walkL4].w = 115;
    characterSpriteClips[walkL4].h = 195;

    characterSpriteClips[walkL3].x = 358;
    characterSpriteClips[walkL3].y = 420;
    characterSpriteClips[walkL3].w = 100;
    characterSpriteClips[walkL3].h = 195;

    characterSpriteClips[walkL2].x = 482;
    characterSpriteClips[walkL2].y = 420;
    characterSpriteClips[walkL2].w = 100;
    characterSpriteClips[walkL2].h = 195;

    characterSpriteClips[walkL1].x = 600;
    characterSpriteClips[walkL1].y = 420;
    characterSpriteClips[walkL1].w = 100;
    characterSpriteClips[walkL1].h = 195;

    characterSpriteClips[stand].x = 0;
    characterSpriteClips[stand].y = 0;
    characterSpriteClips[stand].w = 110;
    characterSpriteClips[stand].h = 195;

    characterSpriteClips[attack].x = 280;
    characterSpriteClips[attack].y = 622;
    characterSpriteClips[attack].w = 195;
    characterSpriteClips[attack].h = 195;

}

void myCharacter::render(SDL_Renderer* renderer, SDL_Rect* clip)
{
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { cPosX, cPosY, cWidth, cHeight };

    //Set clip rendering dimensions
    if (clip != NULL)
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    //Render character to screen
    if (frame == attack && toLeft == true) {
        SDL_RenderCopyEx(renderer, cTexture, clip, &renderQuad, 0, NULL, SDL_FLIP_HORIZONTAL);
    }
    else {
        SDL_RenderCopy(renderer, cTexture, clip, &renderQuad);
    }
}

void myCharacter::renderCurrentAction(SDL_Renderer* renderer) {
    if (walking && toRight) {
        if (SDL_GetTicks() > frameTime + nextFrame) {
            frame++;
            frameTime = SDL_GetTicks();
            if (frame > walkR6)
            {
                frame = walkR1;
            }
        }
    }
    else if (walking && toLeft) {
        if (SDL_GetTicks() > frameTime + nextFrame) {
            frame++;
            frameTime = SDL_GetTicks();
            if (frame<walkL1 || frame>walkL6)
            {
                frame = walkL1;
            }
        }
    }
    else if (!walking && toLeft) {
        if (SDL_GetTicks() > frameTime + nextFrame) {
            frame = walkL3;
            frameTime = SDL_GetTicks();
        }
    }
    else if (!walking && toRight) {
        if (SDL_GetTicks() > frameTime + nextFrame) {
            frame = walkR3;
            frameTime = SDL_GetTicks();
        }
    }

    SDL_Rect* currentClip = &characterSpriteClips[frame];
    render(renderer, currentClip);

    //Render spell
    for (int i = 0; i < max_fire_spell; i++)
    {
        if (fire[i] != nullptr) {
            fire[i]->renderSpellPosition(renderer);
        }
    }
}


//////////////////////////////////// Press and move functions /////////////////////////////////////////////////////

void myCharacter::handleEvent(SDL_Event& e, SDL_Renderer* render)
{
    //If a key was pressed
    if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
    {
        //Adjust the velocity
        switch (e.key.keysym.sym) {

        case SDLK_d:
            toRight = true;
            toLeft = false;
            cVelX += cVelocity;
            break;

        case SDLK_a:
            toRight = false;
            toLeft = true;
            cVelX -= cVelocity;
            break;

        case SDLK_w:
            cVelY -= cVelocity;
            break;
        case SDLK_s:
            cVelY += cVelocity;
            break;

        case SDLK_SPACE:
            if (!jumped) {
                cVelY -= cVelocityJump;
                ground = cPosY;
                jumped = true;
            }
            break;

        case SDLK_f:
            //Init fire attack
            for (int i = 0; i < max_fire_spell; i++)
            {
                if (fire[i] == nullptr) {
                    fire[i] = new Fire(toRight, cPosX, cPosY);
                    fire[i]->setSpriteClips();
                    if (!fire[i]->loadFromFile("image/wizardSheet.png", render)) {
                        std::cout << "Failed to load bullet" << std::endl;
                    }
                    break;
                }
                else continue;
            }

            //Change to attack frame
            frame = attack;
            frameTime = SDL_GetTicks() + nextFrame;

            //Move back a bit because of the spell's rebound
            if (toRight) {
                cPosX -=3;
            }
            else if (toLeft) {
                cPosX +=3;
            }
            break;
        }
    }

    //If a key was released
    else if (e.type == SDL_KEYUP && e.key.repeat == 0)
    {
        //Adjust the velocity
        switch (e.key.keysym.sym) {

        case SDLK_d:
            cVelX -= cVelocity;
            break;

        case SDLK_a:
            cVelX += cVelocity;
            break;

        case SDLK_w:
            cVelY += cVelocity;
            break;

        case SDLK_s:
            cVelY -= cVelocity;
            break;

        case SDLK_f:
            //Move back a bit because of the spell's rebound
            if (toRight) {
                cPosX ++;
            }
            else if (toLeft) {
                cPosX --;
            }
            break;
        }
    }
}

void myCharacter::move()
{
    //Move right or left
    if (cVelX != 0) {
        cPosX += cVelX;
        walking = true;
    }
    //If went too far to the right or left
    if (cPosX < leftmostCharacterPos)
    {
        cPosX += cVelocity;
        gotToFar = true;
    }
    else if (cPosX > rightmostCharacterPos) {
        cPosX -= cVelocity;
        gotToFar = true;
    }
    else gotToFar = false;

    //Move up or down
    if (cVelY != 0) {
        cPosY += cVelY;
        walking = true;
    }

    //If jumped too far up
    if (jumped && cPosY < ground - jumpHeight) {
        cVelY += cVelocityJump;
    }
    //If jumped and fell back to where standing
    if (jumped && cPosY > ground) {
        cVelY = 0;
        jumped = false;
    }

    //If walked too far up
    if (walking && !jumped && cPosY < walkLimit) {
        cPosY = walkLimit;
    }
    //If got too far down
    if (cPosY > baseGround) {
        cPosY = baseGround-1;
    }

    //If not walking anywhere
    if (cVelX == 0 && cVelY != cVelocity) {
        walking = false;
    }

    //Delete spell if out of range
    for (int i = 0; i < max_fire_spell; i++)
    {
        if (fire[i] != nullptr && fire[i]->outOfRange()) {
            fire[i]->free();
            fire[i] = nullptr;
        }
        else if (fire[i] != nullptr) {
            fire[i]->move();
        }
    }
}


void myCharacter::free()
{
    //Free texture if it exists
    if (cTexture != NULL)
    {
        SDL_DestroyTexture(cTexture);
        cTexture = NULL;
        cWidth = 0;
        cHeight = 0;
    }
}

int myCharacter::getcPosX() {
    return cPosX;
}

int myCharacter::getcPosY() {
    return cPosY;
}

int myCharacter::getcVelX() {
    return cVelX;
}

int myCharacter::getcVelY() {
    return cVelY;
}

int myCharacter::getWidth() {
    return cWidth;
}

int myCharacter::getHeight() {
    return cHeight;
}
