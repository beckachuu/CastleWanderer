#include "basic_init.h"
#include "wizard.h"


int main(int argc, char* argv[]) {
    SDL_Event e;
    SDL_Renderer* render = initSDL();
    
    SDL_Texture* background = loadTexture ("image/jungle background.png");

    myWizard wizard;
    wizard.setSpriteClips();
    int frame = 0;

    if (!wizard.loadFromFile("image/wizardSheet.png", render)) {
        std::cerr << "Load wizard sheet error!";
        return 0;
    }

    if (render == nullptr) {
        std::cerr << "Failed to initialize!" << std::endl;
    }
    else {
        bool quit = false;

        while (!quit) {
            SDL_RenderClear(render);
            SDL_RenderCopy(render, background, NULL, NULL);

            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_KEYDOWN) {

                    switch (e.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        close();
                        return 0;
                    }
                }
                else if (e.type == SDL_QUIT) {
                    quit = true;
                }

                wizard.handleEvent(e);
            }

            if (wizard.move() % 65 == 0 && wizard.getmPosX()!=0) {
                
                frame++;
                if (frame >= 4)
                {
                    frame = 0;
                }
            }
            else if (wizard.getmVelX() == 0) {
                frame == stand;
            }

            wizard.renderCurrentAction(frame, render);
            SDL_RenderPresent(render);
        }
    }

    wizard.free();
    close(); // not good enough, should destroy all pictures instead of one
    return 0;
}