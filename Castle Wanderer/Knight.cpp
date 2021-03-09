#include "Knight.h"
#include "basic_init.h"


myKnight::myKnight()
{
    //Initialize
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
}

myKnight::~myKnight()
{
    //Deallocate
    free();
}

bool myKnight::loadFromFile(std::string path, SDL_Renderer* renderer)
{
    //Get rid of preexisting texture
    free();

    //The final texture
    SDL_Texture* newTexture = NULL;

    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL)
    {
        std::cerr << "Unable to load knight image! SDL_image Error:" << IMG_GetError();
    }
    else
    {
        //Color key image
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 147, 193, 217));

        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if (newTexture == NULL)
        {
            std::cerr << "Unable to create texture from %s! SDL Error: %s\n" << path.c_str() << SDL_GetError();
        }
        else
        {
            //Get image dimensions
            mWidth = loadedSurface->w;
            mHeight = loadedSurface->h;
        }

        //Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }

    //Return success
    mTexture = newTexture;
    return (mTexture != NULL);
}

void myKnight::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
    //Modulate texture rgb
    SDL_SetTextureColorMod(mTexture, red, green, blue);
}
void myKnight::setBlendMode(SDL_BlendMode blending)
{
    //Set blending function
    SDL_SetTextureBlendMode(mTexture, blending);
}
void myKnight::setAlpha(Uint8 alpha)
{
    //Modulate texture alpha
    SDL_SetTextureAlphaMod(mTexture, alpha);
}

void myKnight::render(int x, int y, SDL_Renderer* renderer, SDL_Rect* clip)
{
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { x, y, mWidth, mHeight };

    //Set clip rendering dimensions
    if (clip != NULL)
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    //Render to screen
    SDL_RenderCopy(renderer, mTexture, clip, &renderQuad);
}

void myKnight::setSpriteClips() {
    //Set sprite clips

    characterSpriteClips[kwalk1].x = 100;
    characterSpriteClips[kwalk1].y = 50;
    characterSpriteClips[kwalk1].w = 100;
    characterSpriteClips[kwalk1].h = 200;

    characterSpriteClips[kwalk2].x = 200;
    characterSpriteClips[kwalk2].y = 50;
    characterSpriteClips[kwalk2].w = 100;
    characterSpriteClips[kwalk2].h = 200;

    characterSpriteClips[kwalk3].x = 300;
    characterSpriteClips[kwalk3].y = 50;
    characterSpriteClips[kwalk3].w = 100;
    characterSpriteClips[kwalk3].h = 200;

    characterSpriteClips[kwalk4].x = 400;
    characterSpriteClips[kwalk4].y = 50;
    characterSpriteClips[kwalk4].w = 100;
    characterSpriteClips[kwalk4].h = 200;
}

void myKnight::renderCurrentAction(int frame, SDL_Renderer* renderer) {

    // clear screen
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(renderer);

    // Render pacman action
    // Do action 
    SDL_Rect* currentClip = &characterSpriteClips[frame / 4];
    render((SCREEN_WIDTH - currentClip->w) / 2, (SCREEN_HEIGHT - currentClip->h) / 2, renderer, currentClip);
}

void myKnight::free()
{
    //Free texture if it exists
    if (mTexture != NULL)
    {
        SDL_DestroyTexture(mTexture);
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
    }
}

int myKnight::getWidth()
{
    return mWidth;
}

int myKnight::getHeight()
{
    return mHeight;
}
