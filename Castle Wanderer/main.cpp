#include "basic_init.h"
#include "Knight.h"


int main(int argc, char* argv[]) {
    SDL_Event e;
    SDL_Renderer* render = initSDL();

    SDL_Rect viewport;
    viewport.x = 5;
    viewport.y = SCREEN_HEIGHT-300;
    viewport.w = 150;
    viewport.h = 200;

    SDL_Rect backgroundViewport;
    viewport.x = 0;
    viewport.y = 0;
    viewport.w = 1280;
    viewport.h = 720;
    
    SDL_Texture* background = loadTexture ("image/background/jungle background.png");

    myKnight knight;
    knight.setSpriteClips();
    int frame = 0;

    if (!knight.loadFromFile("image/knight/knightSheet.png", render)) {
        std::cerr << "Load knight error!";
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
                    case SDLK_SPACE: {
                        waitUntilKeyPressed();
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
                knight.handleEvent(e);
            }

            knight.move();

            knight.renderCurrentAction(frame, render);

            SDL_RenderPresent(render);

            //Go to next frame
            frame++;

            //Cycle animation
            if (frame >= 4)
            {
                frame = 0;
            }
            SDL_Delay(200);
        }
    }
    close(); // not good enough, should destroy all pictures instead of one
    return 0;
}