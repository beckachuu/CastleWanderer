#pragma once
#include "basic_init.h"


//Loads individual image
SDL_Surface* loadBMPSurface(const char* name);

//Loads media
bool loadBMPMedia(const char* name);

//Frees media and shuts down SDL
void close();



SDL_Surface* loadBMPSurface(const char* name)
{
	//The final optimized image
	SDL_Surface* optimizedSurface = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = SDL_LoadBMP(name);
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL Error: %s\n", name, SDL_GetError());
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

bool loadBMPMedia(const char* name)
{
	//Loading success flag
	bool success = true;

	//Load surface
	CurrentSurface = loadBMPSurface(name);
	if (CurrentSurface == NULL)
	{
		printf("Failed to load BMP image!\n");
		success = false;
	}

	return success;
}

void close()
{
	//Free loaded image
	SDL_FreeSurface(CurrentSurface);
	CurrentSurface = NULL;
}



void showCurrentSurface()
{
	//Main loop flag
	bool quit = false;

	//Event handler
	SDL_Event e;

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
			}
		}

		//Apply the image
		SDL_BlitSurface(CurrentSurface, NULL, screenSurface, NULL);

		//Update the surface
		SDL_UpdateWindowSurface(window);
	}
}