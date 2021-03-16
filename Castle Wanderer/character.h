#pragma once
#include "basic_init.h"

enum poses { walk1, walk2, walk3, walk4, stand, attack, hide, total };

//Texture wrapper class
class myCharacter
{
public:

	//Maximum axis velocity of knight
	const int cVelocity = 1;

	//Initializes variables
	myCharacter();

	//Deallocates memory
	~myCharacter();

	//Loads image at specified path
	bool loadFromFile(std::string path, SDL_Renderer* renderer);

	//Set color modulation
	void setColor(Uint8 red, Uint8 green, Uint8 blue);
	//Set blending
	void setBlendMode(SDL_BlendMode blending);
	//Set alpha modulation
	void setAlpha(Uint8 alpha);

	//Renders texture at given point
	void render(SDL_Renderer* renderer, SDL_Rect* clip = NULL);
	void setSpriteClips();
	void renderCurrentAction(int frame, SDL_Renderer* renderer);

	//Move
	void handleEvent(SDL_Event& e);
	int move();


	//Deallocates texture
	void free();

	//Gets image dimensions
	int getWidth();
	int getHeight();

	//Gets image offsets
	int getcPosX();
	int getcPosY();

	//Gets image velocity
	int getcVelX();
	int getcVelY();

private:

	//Scene textures
	SDL_Rect characterSpriteClips[total];

	//The actual hardware texture
	SDL_Texture* cTexture;

	//The X and Y offsets
	int cPosX, cPosY;

	//velocity
	int cVelX, cVelY;

	//Image dimensions
	int cWidth;
	int cHeight;
};