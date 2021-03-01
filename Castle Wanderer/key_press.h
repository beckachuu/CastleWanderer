#ifndef KEY_PRESS_H
#define KEY_PRESS_H

#include "basic_init.h"


//Key press surfaces constants
enum KeyPressSurfaces
{
	KEY_PRESS_SURFACE_DEFAULT,
	KEY_PRESS_SURFACE_UP,
	KEY_PRESS_SURFACE_DOWN,
	KEY_PRESS_SURFACE_LEFT,
	KEY_PRESS_SURFACE_RIGHT,
	KEY_PRESS_SURFACE_TOTAL
};


//The images that correspond to a keypress
SDL_Surface* KeyPressSurfaces[KEY_PRESS_SURFACE_TOTAL];


bool loadKeyPressMedia();

void fourKeys();


/// <summary>
/// /////////////////////////////////////////////////////////////////////////
/// </summary>
/// <returns></returns>

bool loadKeyPressMedia()
{
	//Loading success flag
	bool success = true;

	//Load default surface
	KeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] = SDL_LoadBMP("castle.bmp");
	if (KeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] == NULL)
	{
		printf("Failed to load default image!\n");
		success = false;
	}

	//Load up surface
	KeyPressSurfaces[KEY_PRESS_SURFACE_UP] = SDL_LoadBMP("up.bmp");
	if (KeyPressSurfaces[KEY_PRESS_SURFACE_UP] == NULL)
	{
		printf("Failed to load up image!\n");
		success = false;
	}

	//Load down surface
	KeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] = SDL_LoadBMP("down.bmp");
	if (KeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] == NULL)
	{
		printf("Failed to load down image!\n");
		success = false;
	}

	//Load left surface
	KeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] = SDL_LoadBMP("left.bmp");
	if (KeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] == NULL)
	{
		printf("Failed to load left image!\n");
		success = false;
	}

	//Load right surface
	KeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] = SDL_LoadBMP("right.bmp");
	if (KeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] == NULL)
	{
		printf("Failed to load right image!\n");
		success = false;
	}

	return success;
}


void fourKeys() {

	if (!loadKeyPressMedia())
	{
		cout << "again plzzz...";
		return;
	}

	//Main loop flag
	bool quit = false;

	//Event handler
	SDL_Event e;

	//Set default current surface
	//CurrentSurface = KeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];

	//While application is running
	while (!quit)
	{
		//Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			//User requests quit
			if (e.type == SDL_QUIT)
			{
				quit = true;
				break;
			}

			//User presses a key
			if (e.type == SDL_KEYDOWN)
			{
				//Select surfaces based on key press
				switch (e.key.keysym.sym)
				{
				case SDLK_UP:
					CurrentSurface = KeyPressSurfaces[KEY_PRESS_SURFACE_UP];
					break;

				case SDLK_DOWN:
					CurrentSurface = KeyPressSurfaces[KEY_PRESS_SURFACE_DOWN];
					break;

				case SDLK_LEFT:
					CurrentSurface = KeyPressSurfaces[KEY_PRESS_SURFACE_LEFT];
					break;

				case SDLK_RIGHT:
					CurrentSurface = KeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT];
					break;

				default:
					CurrentSurface = KeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
					break;
				}
			}
		}

		//Apply the current image
		SDL_BlitSurface(CurrentSurface, NULL, screenSurface, NULL);

		//Update the surface
		SDL_UpdateWindowSurface(window);

	}

	//Deallocate surfaces
	for (int i = 0; i < KEY_PRESS_SURFACE_TOTAL; ++i)
	{
		SDL_FreeSurface(KeyPressSurfaces[i]);
		KeyPressSurfaces[i] = NULL;
	}
}



#endif