#include "basic_init.h"
#include <SDL_image.h>

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
TTF_Font* font = NULL;

SDL_Renderer* initSDL() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        logError(std::cerr, "Init", true);
        return nullptr;
    }
    else {
        //Set texture filtering to linear
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) //or "linear"
        {
            printf("Warning: Linear texture filtering not enabled!");
        }

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
    //SDL_RenderSetLogicalSize(render, SCREEN_WIDTH, SCREEN_HEIGHT);
    return renderer;
}

TTF_Font* initTTF() {
    //Initialize SDL_ttf
    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError();
        return nullptr;
    }
    else {
        //Open the font
        font = TTF_OpenFont("FRSCRIPT.ttf", 60);
        if (font == NULL)
        {
            std::cerr << "Failed to load lazy font! SDL_ttf Error: " << TTF_GetError() << std::endl;
            return nullptr;
        }
    }
    return font;
}


SDL_Texture* loadFromRenderedText(std::string textureText, SDL_Color textColor)
{
    SDL_Texture* text = nullptr;

    //Render text surface
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, textureText.c_str(), textColor);
    if (textSurface == NULL)
    {
        std::cerr << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << std::endl;
    }
    else
    {
        //Create texture from surface pixels
        text = SDL_CreateTextureFromSurface(renderer, textSurface);
        if (text == NULL)
        {
            std::cerr << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << std::endl;
        }

        //Get rid of old surface
        SDL_FreeSurface(textSurface);
    }

    //Return success
    return text;
}

void loadText(SDL_Texture* text, std::string sentence)
{
    SDL_Color textColor = { 255, 255, 255 };
    //Render text
    text = loadFromRenderedText(sentence, textColor);
    if (text == NULL)
    {
        std::cerr << "Failed to render text texture!" << std::endl;
    }
}


void logError(std::ostream& out, const std::string& ms, bool fatal) {
    out << ms << " Error: " << SDL_GetError() << std::endl;
    if (fatal) {
        SDL_Quit();
        exit(1);
    }
}

void freeTexture(SDL_Texture* texture) {
    if (texture != NULL)
    {
        SDL_DestroyTexture(texture);
        texture = NULL;
    }
}

void close() {
    //Free global font
    TTF_CloseFont(font);
    font = NULL;

    //Destroy window	
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    window = NULL;
    renderer = NULL;

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}
