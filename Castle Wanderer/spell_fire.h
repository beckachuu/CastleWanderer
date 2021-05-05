#pragma once
#include <SDL.h>
#include <iostream>
#include <string>

const int leftMostFirePos = 10;
const int rightMostFirePos = 1150;

class Fire
{
public:

	const int maxFireDamage = 21;
	const int minFireDamage = 7;

	Fire(bool toRight, int wizPosX, int wizPosY, int characterVelocity);
	void shoot(bool toRight, int wizPosX, int wizPosY, int characterVelocity);
	void reload();
	~Fire();

	void setColor(Uint8 red, Uint8 green, Uint8 blue);
	void setBlendMode(SDL_BlendMode blending);
	void setAlpha(Uint8 alpha);

	void setSpriteClips();
	void render(SDL_Renderer* renderer, SDL_Rect* clip = NULL);
	void renderSpellPosition(SDL_Renderer* renderer);

	bool isAvailable();
	int getFireDamage();
	void move();
	bool outOfRange();

	int getPosX();
	int getPosY();

	void setVelocity(int characterVelocity);

	int getWidth();
	int getHeight();

	void free();

private:
	int frame;

	SDL_Texture* fTexture;
	SDL_Rect fireSpriteClips[3] = { NULL };

	int fPosX, fPosY;

	int fVelocity;
	int fVelX;

	int fWidth;
	int fHeight;

	bool available;
};
