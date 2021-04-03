#pragma once
#include <SDL.h>
#include <iostream>
#include <string>

const int leftMostFirePos = -100;
const int rightMostFirePos = 1400;

class Fire
{
public:

	//Maximum axis velocity
	const int fVelocity = 32;

	//Initializes variables
	Fire(bool& toRight, int& wizPosX, int& wizPosY, SDL_Renderer* renderer);

	//Set color modulation
	void setColor(Uint8 red, Uint8 green, Uint8 blue);
	//Set blending
	void setBlendMode(SDL_BlendMode blending);
	//Set alpha modulation
	void setAlpha(Uint8 alpha);

	//Renders texture at given point
	void setSpriteClips();
	void render(SDL_Renderer* renderer, SDL_Rect* clip = NULL);
	void renderSpellPosition(SDL_Renderer* renderer);

	void move();
	//Check if out of screen
	bool outOfRange();

	//Deallocates texture
	void free();

	//Gets image dimensions
	int getWidth();
	int getHeight();

	//Gets image offsets
	int getPosX();
	int getPosY();

	int getFireDamage();

private:

	//Scene textures
	SDL_Rect fireSpriteClips[3] = { NULL };

	//The actual hardware texture
	SDL_Texture* fTexture;

	//The X and Y offsets
	int fPosX, fPosY;
	//Velocity
	int fVelX;

	//Image dimensions
	int fWidth;
	int fHeight;

	int frame;

	int fireDamage;
};
