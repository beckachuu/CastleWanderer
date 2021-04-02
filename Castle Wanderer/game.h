#pragma once
#include <SDL.h>
#include <SDL_ttf.h>

class Game
{
public:
	const uint32_t nextFrameTime = 20;

	Game();
	~Game();

	void run();
private:

	SDL_Event e;
	uint32_t gameTime;

	SDL_Renderer* render = nullptr;
	TTF_Font* font = nullptr;
};

