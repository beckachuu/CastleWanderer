#include "Knight.h"
#include "basic_init.h"


myKnight::myKnight()
{
    //Initialize
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;

    //Initialize the offsets
    mPosX = 0;
    mPosY = 500;

    //Initialize the velocity
    mVelX = 0;
    mVelY = 0;
}

myKnight::~myKnight()
{
    //Deallocate
    free();
}

bool myKnight::loadFromFile(std::string path, SDL_Renderer* renderer)
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

void myKnight::render(SDL_Renderer* renderer, SDL_Rect* clip)
{
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { mPosX, mPosY, mWidth, mHeight };

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
    characterSpriteClips[kwalk2].w = 125;
    characterSpriteClips[kwalk2].h = 200;

    characterSpriteClips[kwalk3].x = 330;
    characterSpriteClips[kwalk3].y = 50;
    characterSpriteClips[kwalk3].w = 100;
    characterSpriteClips[kwalk3].h = 200;

    characterSpriteClips[kwalk4].x = 435;
    characterSpriteClips[kwalk4].y = 50;
    characterSpriteClips[kwalk4].w = 120;
    characterSpriteClips[kwalk4].h = 200;

    characterSpriteClips[kstand].x = 0;
    characterSpriteClips[kstand].y = 50;
    characterSpriteClips[kstand].w = 100;
    characterSpriteClips[kstand].h = 200;

    characterSpriteClips[kpunch1].x = 325;
    characterSpriteClips[kpunch1].y = 250;
    characterSpriteClips[kpunch1].w = 125;
    characterSpriteClips[kpunch1].h = 200;

    characterSpriteClips[kpunch2].x = 450;
    characterSpriteClips[kpunch2].y = 250;
    characterSpriteClips[kpunch2].w = 125;
    characterSpriteClips[kpunch2].h = 200;
}

void myKnight::renderCurrentAction(int frame, SDL_Renderer* renderer) {

    // clear screen
    //SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    //SDL_RenderClear(renderer);

    SDL_Rect* currentClip = &characterSpriteClips[frame];
    render(renderer, currentClip);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void myKnight::handleEvent(SDL_Event& e)
{
    //If a key was pressed
    if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
    {
        //Adjust the velocity
        switch (e.key.keysym.sym)
        {
        case SDLK_LEFT: mVelX -= kVelocity; break;
        case SDLK_RIGHT: mVelX += kVelocity; break;
        }
    }
    //If a key was released
    else if (e.type == SDL_KEYUP && e.key.repeat == 0)
    {
        //Adjust the velocity
        switch (e.key.keysym.sym)
        {
        case SDLK_LEFT: mVelX += kVelocity; break;
        case SDLK_RIGHT: mVelX -= kVelocity; break;
        }
    }
}

void myKnight::move()
{
    //Move the dot left or right
    mPosX += mVelX;

    //If the dot went too far to the left or right
    if ((mPosX < 0) || (mPosX > SCREEN_WIDTH-10))
    {
        //Move back
        mPosX -= mVelX;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////


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
