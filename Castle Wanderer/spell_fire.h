#pragma once
#include "basic_init.h"

class Fire
{
public:

	//Maximum axis velocity of knight
	const int fVelocity = 4;

	//Initializes variables
	Fire(int& wizFrame, int& wizPosX, int& wizPosY);



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
	void renderBulletPosition(SDL_Renderer* renderer);

	void move();

	bool okayToDelete();

	void setTexture(SDL_Texture* cTexture);


	//Deallocates texture
	void free();

	//Gets image dimensions
	int getWidth();
	int getHeight();

	//Gets image offsets
	int getPosX();
	int getPosY();


private:

	//Scene textures
	SDL_Rect fireSpriteClips[3];

	//The actual hardware texture
	SDL_Texture* fTexture;

	//The X and Y offsets
	int fPosX, fPosY;

	int fVelX;

	//Image dimensions
	int fWidth;
	int fHeight;
	int frame;

	Uint32 frameTime;
	Uint32 moveTime;
};
