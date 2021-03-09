#include "basic_init.h"
#include "Knight.h"


int main(int argc, char* argv[]) {
    SDL_Event e;

    SDL_Rect viewport;
    viewport.x = 20;
    viewport.y = 400;
    viewport.w = 100;
    viewport.h = 200;

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

                    case SDLK_RIGHT: {

                        if (viewport.x < 1490) {
                            viewport.x += 5;
                        }
                        SDL_RenderSetViewport(render, &viewport);

                        break;
                    }
                    case SDLK_LEFT: {
                        if (viewport.x > 10) {
                            viewport.x -= 5;
                        }
                        SDL_RenderSetViewport(render, &viewport);
                        break;
                    }
                    case SDLK_ESCAPE:
                        close();
                        return 0;
                    }
                }
                else if (e.type == SDL_QUIT) {
                    quit = true;
                }
            }

            //Clear screen
            SDL_RenderClear(render);

            //Render background to the screen
            myKnight* background{};
            background->loadFromFile("image/background/jungle background.png", render);
            background->render(0, 0, render);
            SDL_RenderPresent(render);

            /*

            //Render knight to the screen
            myKnight* knight{};
            if (!knight->loadFromFile("image/knight/knightSheet.png", render))
            {
                std::cerr << "Failed to load walking animation texture!\n";
                return 0;
            }
            else
            {
                //Set sprite clips
                knight->setSpriteClips();
            }

            int frame = 0;
            knight->renderCurrentAction(frame, render);

            //Update screen
            SDL_RenderPresent(render);

            //Go to next frame
            ++frame;

            //Cycle animation
            if (frame / 4 >= 4)
            {
                frame = 0;
            }

            */

        }
    }
    close(); // not good enough, should destroy all pictures instead of one
    return 0;
}