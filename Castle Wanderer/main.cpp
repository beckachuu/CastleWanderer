#include "basic_init.h"


int main(int argc, char* argv[]) {
    SDL_Event e;

    SDL_Texture* screen = NULL;
    SDL_Texture* picture[total] = { NULL };

    SDL_Renderer* render = initSDL();

    if (render == nullptr) {
        std::cerr << "Failed to initialize!" << std::endl;
    }
    else {
        if (!loadMedia(picture)) {
            std::cerr << "Failed to load image" << std::endl;
        }
        else {
            std::cerr << "loaded image" << std::endl;

            screen = picture[Default];

            //screen = picture[Default];
            bool quit = false;

            while (!quit) {
                while (SDL_PollEvent(&e) != 0) {
                    if (e.type == SDL_KEYDOWN) {

                        switch (e.key.keysym.sym) {
                        case SDLK_UP: {
                            screen = picture[Up];

                            //Top left corner viewport
                            SDL_Rect topLeftViewport;
                            topLeftViewport.x = 0;
                            topLeftViewport.y = 0;
                            topLeftViewport.w = SCREEN_WIDTH / 2;
                            topLeftViewport.h = SCREEN_HEIGHT / 2;
                            SDL_RenderSetViewport(render, &topLeftViewport);

                            break;
                        }

                        case SDLK_RIGHT: {
                            screen = picture[Right];

                            //Top left corner viewport
                            SDL_Rect topLeftViewport;
                            topLeftViewport.x = 0;
                            topLeftViewport.y = 0;
                            topLeftViewport.w = SCREEN_WIDTH / 2;
                            topLeftViewport.h = SCREEN_HEIGHT / 2;
                            SDL_RenderSetViewport(render, &topLeftViewport);

                            break;
                        }
                        case SDLK_DOWN: {
                            screen = picture[Down];

                            //Top left corner viewport
                            SDL_Rect topLeftViewport;
                            topLeftViewport.x = 0;
                            topLeftViewport.y = 0;
                            topLeftViewport.w = SCREEN_WIDTH / 2;
                            topLeftViewport.h = SCREEN_HEIGHT / 2;
                            SDL_RenderSetViewport(render, &topLeftViewport);

                            break;
                        }
                        case SDLK_LEFT: {
                            screen = picture[Left];

                            //Top left corner viewport
                            SDL_Rect topLeftViewport;
                            topLeftViewport.x = 0;
                            topLeftViewport.y = 0;
                            topLeftViewport.w = SCREEN_WIDTH / 2;
                            topLeftViewport.h = SCREEN_HEIGHT / 2;
                            SDL_RenderSetViewport(render, &topLeftViewport);

                            break;
                        }
                        case SDLK_TAB: {
                            screen = picture[Dragon];

                            //Top left corner viewport
                            SDL_Rect topLeftViewport;
                            topLeftViewport.x = 0;
                            topLeftViewport.y = 0;
                            topLeftViewport.w = SCREEN_WIDTH / 2;
                            topLeftViewport.h = SCREEN_HEIGHT / 2;
                            SDL_RenderSetViewport(render, &topLeftViewport);

                            break;
                        }

                        case SDLK_ESCAPE:
                            close(screen);
                            return 0;
                        default:
                            screen = picture[Default]; break;
                        }
                    }

                    else if (e.type == SDL_QUIT) {
                        quit = true;
                    }
                }

                

                SDL_RenderClear(render);
                SDL_RenderCopy(render, screen, NULL, NULL);
                SDL_RenderPresent(render);
            }
        }
    }
    close(screen); // not good enough, should destroy all pictures instead of one
    return 0;
}