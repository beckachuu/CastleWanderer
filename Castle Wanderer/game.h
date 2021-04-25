#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include "background.h"
#include "guard.h"
#include "goblin.h"
#include "character.h"
#include "spell_fire.h"

const int fireSpellNum = 4;
const int guardNum = 7;
const int goblinNum = 5;

class Game
{
public:
	const unsigned int nextFrameTime = 20;

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
	unsigned int gameTime;

	SDL_Renderer* render = nullptr;
	TTF_Font* font = nullptr;

	//Init background
	Background* background;

	Guard* guard[guardNum];
	int tempGuardPosX[guardNum], tempGuardPosY[guardNum], tempGuardWidth[guardNum], tempGuardHeight[guardNum];
	int guardNameCount;

	Goblin* goblin[goblinNum];
	int tempGoblinPosX[goblinNum], tempGoblinPosY[goblinNum], tempGoblinWidth[goblinNum], tempGoblinHeight[goblinNum];

	MyCharacter* wizard;
	int tempCharPosX, tempCharPosY, tempCharWidth, tempCharHeight;

	Fire* fire[fireSpellNum] = { nullptr };
	int tempFirePosX[fireSpellNum], tempFirePosY[fireSpellNum], tempFireWidth[fireSpellNum], tempFireHeight[fireSpellNum];
};

