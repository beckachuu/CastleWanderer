#include "basic_init.h"


SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;


//Scene textures
MyTexture characterTexture;
MyTexture BackgroundTexture;


MyTexture::MyTexture()
{
    //Initialize
    mTexture[total] = { NULL };
    mWidth = 0;
    mHeight = 0;
}

MyTexture::~MyTexture()
{
    //Deallocate
    free();
}

bool MyTexture::loadFromFile(std::string path)
{
    //Get rid of preexisting texture
    free();

    //The final texture
    SDL_Texture* newTexture = NULL;

    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL)
    {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    }
    else
    {
        //Color key image
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if (newTexture == NULL)
        {
            printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
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
    return mTexture != NULL;
}

void MyTexture::free()
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

void MyTexture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
    //Modulate texture rgb
    SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void MyTexture::setBlendMode(SDL_BlendMode blending)
{
    //Set blending function
    SDL_SetTextureBlendMode(mTexture, blending);
}

void MyTexture::setAlpha(Uint8 alpha)
{
    //Modulate texture alpha
    SDL_SetTextureAlphaMod(mTexture, alpha);
}

void MyTexture::render(int x, int y, SDL_Rect* clip)
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

int MyTexture::getWidth()
{
    return mWidth;
}

int MyTexture::getHeight()
{
    return mHeight;
}


//Free loaded images
//gModulatedTexture.free();
//gBackgroundTexture.free();




SDL_Renderer* initSDL() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        logError(std::cerr, "Init", true);
        return nullptr;
    }
    else {
        window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
        if (window == NULL) {
            logError(std::cerr, "Window", false);
            return nullptr;
        }
        else {
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            if (renderer == NULL) {
                logError(std::cerr, "Render", false);
                return nullptr;
            }
            else {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                int Flag = IMG_INIT_PNG;
                if (!(IMG_Init(Flag) & Flag)) {
                    logError(std::cerr, "Init PNG", false);
                    return nullptr;
                }
            }
        }
    }
    //SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    //SDL_RenderSetLogicalSize(render, SCREEN_WIDTH, SCREEN_HEIGHT);
    return renderer;
}

SDL_Texture* loadIMG(const std::string& path) { // thay cho SDL_Surface* loadIMG(const std::string& path)
    SDL_Texture* newTexture = NULL;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL) {
        std::cerr << "Unable to load image! SDL_Image Error: " << IMG_GetError() << std::endl;
    }
    else {
        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if (newTexture == NULL) {
            logError(std::cerr, "Texture", false);
        }
        SDL_FreeSurface(loadedSurface);
    }
    return newTexture;
}

bool loadMedia() {

    if (!BackgroundTexture.loadFromFile("image\background")) {
        std::cerr << "Failed to load texture image!\n";
        return false;
    }


    return true;
}

void logError(std::ostream& out, const std::string& ms, bool fatal) {
    out << ms << " Error: " << SDL_GetError() << std::endl;
    if (fatal) {
        SDL_Quit();
        exit(1);
    }
}

void close(SDL_Texture* screen) {
    SDL_DestroyTexture(screen);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
