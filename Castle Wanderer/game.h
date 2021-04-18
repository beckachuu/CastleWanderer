#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include "background.h"
#include "guard.h"
#include "goblin.h"
#include "character.h"
#include "spell_fire.h"

const int guardNum = 7;
const int goblinNum = 9;

class Game
{
public:
	const int minDistantY = 15;
	const uint32_t nextFrameTime = 20;

	const int characterFurthestLeft = 300;
	const int characterFurthestRight = 900;

	Game();
	~Game();

	void run();
	bool handlingKeyboardEvents();
	void moveObjects();
	void detectTouchingObjects();
	void updateScreen();

private:

	SDL_Event e = { NULL };
	uint32_t gameTime;

	SDL_Renderer* render = nullptr;
	TTF_Font* font = nullptr;

	//Init background
	Background* background;

	Guard* guard[guardNum];
	Goblin* goblin[goblinNum];

	MyCharacter* wizard;

	Fire* fire[max_fire_spell] = { nullptr };

	bool touchingX;
	bool touchingY;
};

