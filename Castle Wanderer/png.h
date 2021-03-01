#pragma once
#include "basic_init.h"


//Loads individual image
SDL_Surface* loadPNGSurface(const char* name);

//Loads media
bool loadPNGMedia(const char* name);



SDL_Surface* loadPNGSurface(const char* name)
{
	//The final optimized image
	SDL_Surface* optimizedSurface = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(name);
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", name, IMG_GetError());
	}
	else
	{
		//Convert surface to screen format
		optimizedSurface = SDL_ConvertSurface(loadedSurface, screenSurface->format, 0);
		if (optimizedSurface == NULL)
		{
			printf("Unable to optimize image %s! SDL Error: %s\n", name, SDL_GetError());
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	return optimizedSurface;
}


bool loadPNGMedia(const char* name)
{
	//Loading success flag
	bool success = true;

	//Load surface
	CurrentSurface = loadPNGSurface(name);
	if (CurrentSurface == NULL)
	{
		printf("Failed to load PNG image!\n");
		success = false;
	}

	return success;
}
