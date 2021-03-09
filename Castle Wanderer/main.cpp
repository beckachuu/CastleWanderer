#include "basic_init.h"
#include "Knight.h"


int main(int argc, char* argv[]) {
    SDL_Event e;
    
    SDL_Renderer* render = initSDL();

    if (render == nullptr) {
        std::cerr << "Failed to initialize!" << std::endl;
    }
    else {
        bool quit = false;

        while (!quit) {
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_KEYDOWN) {

                    switch (e.key.keysym.sym) {
                    case SDLK_UP: {

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
                        close();
                        return 0;
                    default:
                        break;
                    }
                }

                else if (e.type == SDL_QUIT) {
                    quit = true;
                }
            }

            myKnight* background{};
            background->loadFromFile("image/background/jungle background.png",render);
            /*

    bool loadMediaPacman():

    //Load sprite sheet texture
    if( !pacman.loadFromFile( "14_animated_sprites_and_vsync/foo.png" ) )
    {
        printf( "Failed to load walking animation texture!\n" );
        success = false;
    }
    else
    {
        //Set sprite clips
        pacman->setSpriteClips();
    }

    void start():
    // Update pacman's action
                pacman->renderCurrentAction(frame, gRenderer)

                //Update screen
                SDL_RenderPresent( gRenderer );

                //Go to next frame
                ++frame;

                //Cycle animation
                if( frame / 4 >= WALKING_ANIMATION_FRAMES )
                {
                    frame = 0;
                }

*/


            SDL_RenderClear(render);
            //SDL_RenderCopy(render, screen, NULL, NULL);
            SDL_RenderPresent(render);
        }
    }
    //close(screen); // not good enough, should destroy all pictures instead of one
    return 0;
}