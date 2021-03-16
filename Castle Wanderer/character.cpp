#include "character.h"
#include "basic_init.h"


myCharacter::myCharacter()
{
    //Initialize
    cTexture = NULL;
    cWidth = 0;
    cHeight = 0;

    //Initialize the offsets
    cPosX = 0;
    cPosY = 475;

    //Initialize the velocity
    cVelX = 0;
    cVelY = 0;
}

myCharacter::~myCharacter()
{
    //Deallocate
    free();
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
        std::cerr << "Unable to load image! SDL_image Error:" << IMG_GetError();
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

    //Render to screen
    SDL_RenderCopy(renderer, cTexture, clip, &renderQuad);
}

void myCharacter::setSpriteClips() {
    //Set sprite clips

    characterSpriteClips[walk1].x = 97;
    characterSpriteClips[walk1].y = 170;
    characterSpriteClips[walk1].w = 82;
    characterSpriteClips[walk1].h = 150;

    characterSpriteClips[walk2].x = 176;
    characterSpriteClips[walk2].y = 170;
    characterSpriteClips[walk2].w = 82;
    characterSpriteClips[walk2].h = 150;

    characterSpriteClips[walk3].x = 356;
    characterSpriteClips[walk3].y = 170;
    characterSpriteClips[walk3].w = 73;
    characterSpriteClips[walk3].h = 150;

    characterSpriteClips[walk4].x = 446;
    characterSpriteClips[walk4].y = 170;
    characterSpriteClips[walk4].w = 82;
    characterSpriteClips[walk4].h = 150;

    characterSpriteClips[stand].x = 0;
    characterSpriteClips[stand].y = 50;
    characterSpriteClips[stand].w = 100;
    characterSpriteClips[stand].h = 200;

}

void myCharacter::renderCurrentAction(int frame, SDL_Renderer* renderer) {

    // clear screen
    //SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    //SDL_RenderClear(renderer);

    SDL_Rect* currentClip = &characterSpriteClips[frame];
    render(renderer, currentClip);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void myCharacter::handleEvent(SDL_Event& e)
{
    //If a key was pressed
    if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
    {
        //Adjust the velocity
        switch (e.key.keysym.sym)
        {
        case SDLK_LEFT: cVelX -= cVelocity; break;
        case SDLK_RIGHT: cVelX += cVelocity; break;
        case SDLK_UP: if (cPosY==475) cVelY -= 2; break;
        }
    }
    //If a key was released
    else if (e.type == SDL_KEYUP && e.key.repeat == 0)
    {
        //Adjust the velocity
        switch (e.key.keysym.sym)
        {
        case SDLK_LEFT: cVelX += cVelocity; break;
        case SDLK_RIGHT: cVelX -= cVelocity; break;
        case SDLK_UP: if (cPosY == 475) cVelY -= 2; break;
        }
    }
}

int myCharacter::move()
{
    //Move the left or right
    cPosX += cVelX;

    //If went too far to the left or right
    if ((cPosX < 0) || (cPosX > SCREEN_WIDTH-10))
    {
        //Move back
        cPosX -= cVelX;
    }
    //Move the up or down
    cPosY += cVelY;

    if (cPosY < 170) {
        cVelY += 2;
    }
    if (cPosY > 475) {
        cPosY = 475;
        cVelY = 0;
    }
    return cPosX;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////


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
