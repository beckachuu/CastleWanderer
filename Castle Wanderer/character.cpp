#include "character.h"

myCharacter::myCharacter()
{
    //Initialize
    cTexture = NULL;
    cWidth = 0;
    cHeight = 0;

    //Initialize the offsets
    cPosX = SCREEN_WIDTH/2;
    cPosY = 0;
    ground = (SCREEN_HEIGHT - 190) * 19 / 20;

    //Initialize the velocity
    cVelX = 0;
    cVelY = 0;

    frame = stand;

    frameTime = 0;
    moveTime = 0;

}

myCharacter::~myCharacter()
{
    //Deallocate
    free();
}


void myCharacter::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
    //Modulate texture rgb
    SDL_SetTextureColorMod(cTexture, red, green, blue);
}
void myCharacter::setBlendMode(SDL_BlendMode blending)
{
    //Set blending function
    SDL_SetTextureBlendMode(cTexture, blending);
}
void myCharacter::setAlpha(Uint8 alpha)
{
    //Modulate texture alpha
    SDL_SetTextureAlphaMod(cTexture, alpha);
}

bool myCharacter::loadFromFile(std::string path, SDL_Renderer* renderer)
{
    //Get rid of preexisting texture
    //free();

    //The final texture
    SDL_Texture* newTexture = NULL;

    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL)
    {
        std::cerr << "Unable to load image! SDL_image Error:" << IMG_GetError()<<std::endl;
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
    characterSpriteClips[walkR1].h = 190;

    characterSpriteClips[walkR2].x = 126;
    characterSpriteClips[walkR2].y = 218;
    characterSpriteClips[walkR2].w = 104;
    characterSpriteClips[walkR2].h = 190;

    characterSpriteClips[walkR3].x = 238;
    characterSpriteClips[walkR3].y = 218;
    characterSpriteClips[walkR3].w = 113;
    characterSpriteClips[walkR3].h = 190;

    characterSpriteClips[walkR4].x = 360;
    characterSpriteClips[walkR4].y = 218;
    characterSpriteClips[walkR4].w = 121;
    characterSpriteClips[walkR4].h = 190;

    characterSpriteClips[walkR5].x = 482;
    characterSpriteClips[walkR5].y = 218;
    characterSpriteClips[walkR5].w = 107;
    characterSpriteClips[walkR5].h = 190;

    characterSpriteClips[walkR6].x = 589;
    characterSpriteClips[walkR6].y = 218;
    characterSpriteClips[walkR6].w = 97;
    characterSpriteClips[walkR6].h = 190;

    characterSpriteClips[walkL6].x = 15;
    characterSpriteClips[walkL6].y = 420;
    characterSpriteClips[walkL6].w = 99;
    characterSpriteClips[walkL6].h = 190;

    characterSpriteClips[walkL5].x = 113;
    characterSpriteClips[walkL5].y = 420;
    characterSpriteClips[walkL5].w = 107;
    characterSpriteClips[walkL5].h = 190;

    characterSpriteClips[walkL4].x = 224;
    characterSpriteClips[walkL4].y = 420;
    characterSpriteClips[walkL4].w = 115;
    characterSpriteClips[walkL4].h = 190;

    characterSpriteClips[walkL3].x = 358;
    characterSpriteClips[walkL3].y = 420;
    characterSpriteClips[walkL3].w = 100;
    characterSpriteClips[walkL3].h = 190;

    characterSpriteClips[walkL2].x = 482;
    characterSpriteClips[walkL2].y = 420;
    characterSpriteClips[walkL2].w = 100;
    characterSpriteClips[walkL2].h = 190;

    characterSpriteClips[walkL1].x = 600;
    characterSpriteClips[walkL1].y = 420;
    characterSpriteClips[walkL1].w = 100;
    characterSpriteClips[walkL1].h = 190;

    characterSpriteClips[stand].x = 0;
    characterSpriteClips[stand].y = 0;
    characterSpriteClips[stand].w = 115;
    characterSpriteClips[stand].h = 192;

    characterSpriteClips[attack].x = 280;
    characterSpriteClips[attack].y = 622;
    characterSpriteClips[attack].w = 190;
    characterSpriteClips[attack].h = 190;

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
    if (frame==attack && gottaFlip==true) {
        SDL_RenderCopyEx(renderer, cTexture, clip, &renderQuad, 0, NULL, SDL_FLIP_HORIZONTAL);
    }
    else {
        SDL_RenderCopy(renderer, cTexture, clip, &renderQuad);
    }
}

void myCharacter::renderCurrentAction(SDL_Renderer* renderer) {
    int distance = this->move();
    if (distance > 0  ) {
        if (SDL_GetTicks() > frameTime + nextFrame) {
            frame++;
            frameTime = SDL_GetTicks();
            if (frame>walkR6)
            {
                frame = walkR1;
            }
        }
    }
    else if (distance < 0 ) {
        if (SDL_GetTicks() > frameTime + nextFrame) {
            frame++;
            frameTime = SDL_GetTicks();
            if (frame<walkL1||frame>walkL6)
            {
                frame = walkL1;
            }
        }
    }

    SDL_Rect* currentClip = &characterSpriteClips[frame];
    render(renderer, currentClip);

    //render bullet
    for (int i = 0; i < max_fire_spell; i++)
    {
        if (fire[i]!=nullptr) {
            fire[i]->renderBulletPosition(renderer);
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
        switch (e.key.keysym.sym)
        {
        case SDLK_RIGHT:
            gottaFlip = false;
            toRight = true;
            toLeft = false;
            cVelX += cVelocity;
            break;

        case SDLK_LEFT:
            gottaFlip = true;
            toLeft = true;
            toRight = false;
            cVelX -= cVelocity;
            break;
        
        case SDLK_UP:
            if (cPosY == ground) {
                cVelY -= cVelocity * 3;
            }
            break;

        case SDLK_f:

            for (int i = 0; i < max_fire_spell; i++)
            {
                if (fire[i] == nullptr) {
                    fire[i] = new Fire(toRight, cPosX, cPosY);

                    //Try to merge into constructor
                    fire[i]->setSpriteClips();
                    fire[i]->loadFromFile("image/wizardSheet.png", render);
                    break;
                }
                else continue;
            }

            //Change to attack frame
            frame = attack;
            frameTime = SDL_GetTicks() + 750;

            //Move back a bit because of the spell's rebound
            if (toRight) {
                cPosX -= 4;
            }
            else if (toLeft) {
                cPosX += 4;
            }
            break;
        }
    }
    //If a key was released
    else if (e.type == SDL_KEYUP && e.key.repeat == 0)
    {
        //Adjust the velocity
        switch (e.key.keysym.sym)
        {
        case SDLK_LEFT:
            cVelX += cVelocity;
            frame = walkL3;
            break;
        case SDLK_RIGHT:
            cVelX -= cVelocity;
            frame = walkR3;
            break;
        case SDLK_f:
            //Move back a bit because of the spell's rebound
            if (toRight) {
                cPosX += 4;
                moveTime = SDL_GetTicks() + 100;
            }
            else if (toLeft) {
                cPosX -= 4;
                moveTime = SDL_GetTicks() + 100;
            }
            break;
        }
    }
}

int myCharacter::move()
{
    int firstPosX = cPosX;

    //Move the left or right
    if (SDL_GetTicks() > moveTime + 1) {
        cPosX += cVelX;
        moveTime = SDL_GetTicks();
    }
    
    //If went too far to the left or right
    if ((cPosX < 0) || (cPosX > SCREEN_WIDTH - 10))
    {
        //Move back
        cPosX -= cVelX;
    }

    //Move the up or down
    cPosY += cVelY;
    
    //If went too far up or down
    if (cPosY < 170) {
        cVelY += cVelocity;
    }
    if (cPosY > ground) {
        cPosY = ground;
        cVelY = 0;
    }

    //Delete bullet if out of range
    for (int i = 0; i < max_fire_spell; i++)
    {
        if (fire[i]!=nullptr && fire[i]->outOfRange()) {
            fire[i]->free();
            fire[i] = nullptr;
        }
        else if (fire[i] != nullptr) {
            fire[i]->move();
        }
    }

    return (cPosX - firstPosX);
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

int myCharacter::getFrame() {
    return frame;
}