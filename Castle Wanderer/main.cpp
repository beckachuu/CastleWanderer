#include "stretch.h"
#include "bmp.h"
#include "png.h"
#include "texture.h"
#include "key_press.h"


int main(int argc, char* argv[])
{
    
    initSDL(window, renderer);

    // Your drawing code here
    // use SDL_RenderPresent(renderer) to show it

    loadPNGMedia("hearts.png");
    fourKeys();



    waitUntilKeyPressed();
    quitSDL(window, renderer);
    return 0;
}
