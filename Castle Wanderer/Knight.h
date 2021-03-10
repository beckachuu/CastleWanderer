#pragma once

#include "basic_init.h"

enum knightposes { kwalk1, kwalk2, kwalk3, kwalk4, kstand, kpunch1, kpunch2, kjump1, kjump2, kjump3, ktotal };

//Texture wrapper class
class myKnight
{
public:

	//Maximum axis velocity of knight
	static const int kVelocity = 21;

	//Initializes variables
	myKnight();

	//Deallocates memory
	~myKnight();

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
	void move();


	//Deallocates texture
	void free();

	//Gets image dimensions
	int getWidth();
	int getHeight();

private:

	//Scene textures
	SDL_Rect characterSpriteClips[ktotal];

	//The actual hardware texture
	SDL_Texture* mTexture;

	//The X and Y offsets of the dot
	int mPosX, mPosY;

	//The velocity of the dot
	int mVelX, mVelY;

	//Image dimensions
	int mWidth;
	int mHeight;
};