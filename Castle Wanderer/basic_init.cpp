#include "basic_init.h"
#include "Knight.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;


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

void logError(std::ostream& out, const std::string& ms, bool fatal) {
    out << ms << " Error: " << SDL_GetError() << std::endl;
    if (fatal) {
        SDL_Quit();
        exit(1);
    }
}


void close() {
    //knight->free();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

SDL_Texture* loadTexture(std::string path)
{
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
        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if (newTexture == NULL)
        {
            printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        }

        //Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }
    return newTexture;
}