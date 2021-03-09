#pragma once

#include "basic_init.h"

//Texture wrapper class
class myKnight
{
public:
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
	void render(int x, int y, SDL_Renderer* renderer, SDL_Rect* clip = NULL);
	void setSpriteClips();
	void renderCurrentAction(int frame, SDL_Renderer* renderer);

	//Deallocates texture
	void free();

	//Gets image dimensions
	int getWidth();
	int getHeight();

private:
	//Walking animation
	enum knightposes { kjump1, kjump2, kjump3, kpunch1, kpunch2, kwalk1, kwalk2, kwalk3, kwalk4, kwalk5, kstand, ktotal };

	//Scene textures
	SDL_Rect characterSpriteClips[ktotal];

	//The actual hardware texture
	SDL_Texture* mTexture;
	//Image dimensions
	int mWidth;
	int mHeight;
};