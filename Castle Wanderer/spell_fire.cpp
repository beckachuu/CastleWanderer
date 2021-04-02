#include "spell_fire.h"
#include <SDL_image.h>

Fire::Fire(bool& toRight, int& wizPosX, int& wizPosY)
{
    //Initialize
    fTexture = NULL;
    fWidth = 0;
    fHeight = 0;

    //Set fire flying direction and initial position at the end of wizard's staff
    if (toRight) {
        fPosX = wizPosX + 150;
        fVelX = fVelocity;
    }
    else {
        fPosX = wizPosX - 30;
        fVelX = -fVelocity;
    }
    fPosY = wizPosY + 100;

    frame = 0;

    fireDamage = rand() % 15 + 15;
}


void Fire::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
    //Modulate texture rgb
    SDL_SetTextureColorMod(fTexture, red, green, blue);
}
void Fire::setBlendMode(SDL_BlendMode blending)
{
    //Set blending function
    SDL_SetTextureBlendMode(fTexture, blending);
}
void Fire::setAlpha(Uint8 alpha)
{
    //Modulate texture alpha
    SDL_SetTextureAlphaMod(fTexture, alpha);
}

bool Fire::loadFromFile(std::string path, SDL_Renderer* renderer)
{
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
            fWidth = loadedSurface->w;
            fHeight = loadedSurface->h;
        }

        //Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }

    //Return success
    fTexture = newTexture;
    return (fTexture != NULL);
}

//////////////////////////////////// Spell rendering functions /////////////////////////////////////////////

void Fire::setSpriteClips() {
    //Set sprite clips

    fireSpriteClips[0].x = 643;
    fireSpriteClips[0].y = 12;
    fireSpriteClips[0].w = 111;
    fireSpriteClips[0].h = 24;

    fireSpriteClips[1].x = 643;
    fireSpriteClips[1].y = 42;
    fireSpriteClips[1].w = 111;
    fireSpriteClips[1].h = 24;

    fireSpriteClips[2].x = 643;
    fireSpriteClips[2].y = 76;
    fireSpriteClips[2].w = 111;
    fireSpriteClips[2].h = 24;

}

void Fire::render(SDL_Renderer* renderer, SDL_Rect* clip)
{
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { fPosX, fPosY, fWidth, fHeight };

    //Set clip rendering dimensions
    if (clip != NULL)
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    //Render to screen
    if (fVelX < 0) {
        SDL_RenderCopyEx(renderer, fTexture, clip, &renderQuad, 0, NULL, SDL_FLIP_HORIZONTAL);
    }
    else {
        SDL_RenderCopy(renderer, fTexture, clip, &renderQuad);
    }
}

void Fire::renderSpellPosition(SDL_Renderer* renderer) {

    frame++;

    if (frame > 2)
    {
        frame = 0;
    }

    SDL_Rect* currentClip = &fireSpriteClips[frame];
    render(renderer, currentClip);
}


void Fire::move()
{
    fPosX += fVelX;
}

bool Fire::outOfRange() {
    if (this != nullptr && (fPosX<leftMostFirePos || fPosX>rightMostFirePos)) {
        return true;
    }
    else return false;
}


void Fire::free()
{
    //Free texture if it exists
    if (this != nullptr && fTexture != NULL)
    {
        SDL_DestroyTexture(fTexture);
        fTexture = NULL;
        fWidth = 0;
        fHeight = 0;
    }
}

int Fire::getWidth() {
    return fWidth;
}
int Fire::getHeight() {
    return fHeight;
}

int Fire::getPosX() {
    return fPosX;
}
int Fire::getPosY() {
    return fPosY;
}

int Fire::getFireDamage() {
    return fireDamage;
}