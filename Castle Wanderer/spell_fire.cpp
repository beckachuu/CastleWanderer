#include "spell_fire.h"
#include "basic_init.h"
#include <SDL_image.h>

Fire::Fire(bool toRight, int wizPosX, int wizPosY, int characterVelocity)
{
    frame = 0;

    fTexture = loadFromImage("image/Sprite sheets/wizardSheet.png");
    setSpriteClips();

    fWidth = 0;
    fHeight = 0;

    shoot(toRight, wizPosX, wizPosY, characterVelocity);
    available = true;
}

void Fire::shoot(bool toRight, int wizPosX, int wizPosY, int characterVelocity) {
    setVelocity(characterVelocity);

    if (toRight) {
        fPosX = wizPosX + 150;
        fVelX = fVelocity;
    }
    else {
        fPosX = wizPosX - 30;
        fVelX = -fVelocity;
    }
    fPosY = wizPosY + 100;

    available = false;
}
void Fire::reload() {
    available = true;
}

Fire::~Fire() {
    free();
}


void Fire::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
    SDL_SetTextureColorMod(fTexture, red, green, blue);
}
void Fire::setBlendMode(SDL_BlendMode blending)
{
    SDL_SetTextureBlendMode(fTexture, blending);
}
void Fire::setAlpha(Uint8 alpha)
{
    SDL_SetTextureAlphaMod(fTexture, alpha);
}


void Fire::setSpriteClips() {

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
    if (clip != NULL)
    {
        fWidth = clip->w;
        fHeight = clip->h;
    }
    SDL_Rect renderQuad = { fPosX, fPosY, fWidth, fHeight };

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

    SDL_Rect currentClip = fireSpriteClips[frame];
    render(renderer, &currentClip);
}


bool Fire::isAvailable() {
    return available;
}

int Fire::getFireDamage() {
    return rand() % (maxFireDamage - minFireDamage) + minFireDamage;
}

void Fire::move()
{
    fPosX += fVelX;
}

bool Fire::outOfRange() {
    if (fPosX < leftMostFirePos || fPosX > rightMostFirePos) {
        available = true;
        return true;
    }
    else return false;
}

void Fire::setVelocity(int charactervelocity) {
    fVelocity = charactervelocity * 6;
}

int Fire::getPosX() {
    return fPosX;
}
int Fire::getPosY() {
    return fPosY;
}

int Fire::getWidth() {
    return fWidth;
}
int Fire::getHeight() {
    return fHeight;
}

void Fire::free()
{
    freeTexture(fTexture);
    fWidth = 0;
    fHeight = 0;
}