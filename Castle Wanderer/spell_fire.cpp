#include "spell_fire.h"


Fire::Fire(int& wizFrame, int& wizPosX, int& wizPosY)
{
    //Initialize
    fTexture = NULL;
    fWidth = 0;
    fHeight = 0;

    //Initialize the offsets
    if (wizFrame <= 3) {
        fPosX = wizPosX + 180;
        fVelX = fVelocity;
    }
    else {
        fPosX = wizPosX - 40;
        fVelX = -fVelocity;
    }
    
    fPosY = wizPosY + 25;

    frame = 0;

    frameTime = 0;
    moveTime = 0;

    std::cout << "FIRED at: (" << fPosX << " ," << fPosY << ").";
}


bool Fire::loadFromFile(std::string path, SDL_Renderer* renderer)
{
    //Get rid of preexisting texture
    //free();

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
    SDL_RenderCopy(renderer, fTexture, clip, &renderQuad);
}

void Fire::setSpriteClips() {
    //Set sprite clips

    fireSpriteClips[0].x = 643;
    fireSpriteClips[0].y = 6;
    fireSpriteClips[0].w = 111;
    fireSpriteClips[0].h = 24;

    fireSpriteClips[1].x = 643;
    fireSpriteClips[1].y = 36;
    fireSpriteClips[1].w = 111;
    fireSpriteClips[1].h = 24;

    fireSpriteClips[2].x = 643;
    fireSpriteClips[2].y = 70;
    fireSpriteClips[2].w = 111;
    fireSpriteClips[2].h = 24;

}

void Fire::renderBulletPosition(SDL_Renderer* renderer) {

    this->move();

    if (SDL_GetTicks() > frameTime + 50) {
        frame++;
        frameTime = SDL_GetTicks();
    }

    if (frame > 2)
    {
        frame = 0;
    }

    SDL_Rect* currentClip = &fireSpriteClips[frame];
    render(renderer, currentClip);
}



void Fire::move()
{
    if (SDL_GetTicks() > moveTime +1000) {
        fPosX += fVelX;
        moveTime = SDL_GetTicks();
    }
    
    if (okayToDelete()) {
        this->free();
    }
}

bool Fire::okayToDelete() {
    if (this!=nullptr&&(fPosX<-100 || fPosX>SCREEN_WIDTH)) {
        return false;
    }
    else return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Fire::setTexture(SDL_Texture* cTexture) {
    if (this != nullptr) {
        fTexture = cTexture;
    }
    else if (this==nullptr || fTexture==NULL) {
        std::cerr << "Fire texture is null.";
    }
}

void Fire::free()
{
    //Free texture if it exists
    if (this!=nullptr&&fTexture != NULL)
    {
        SDL_DestroyTexture(fTexture);
        fTexture = NULL;
        fWidth = 0;
        fHeight = 0;
    }
}