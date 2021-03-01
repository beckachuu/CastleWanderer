#pragma once
#include "basic_init.h"
#include "bmp.h"


//Current displayed texture
SDL_Texture* texture = NULL;


//Loads individual image as texture
SDL_Texture* loadTextureSurface(string path);

bool loadTextureMedia(const char* name);

void loadTexture();

void closeTexture();


/// <summary>
/// ////////////////////////////////////////////////////////////
/// </summary>


SDL_Texture* loadTextureSurface(string path)
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	return newTexture;
}

bool loadTextureMedia(const char* name)
{
	//Loading success flag
	bool success = true;

	//Load PNG texture
	texture = loadTextureSurface(name);
	if (texture == NULL)
	{
		cout << "Failed to load texture image!\n";
		success = false;
	}

	return success;
}

void loadTexture ()
{
	//Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
		cout << "Failed to initialize!\n";
		return;
	}

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

		//Clear screen
		SDL_RenderClear(renderer);

		//Render texture to screen
		SDL_RenderCopy(renderer, texture, NULL, NULL);

		//Update screen
		SDL_RenderPresent(renderer);
	}

	closeTexture();
}

void closeTexture()
{
	//Free loaded image
	SDL_DestroyTexture(texture);
	texture = NULL;
}