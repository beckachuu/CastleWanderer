#pragma once
#include <SDL.h>
#include <SDL_ttf.h>

#include "background.h"

#include "guard.h"
#include "goblin.h"

#include "character.h"
#include "spell_fire.h"

#include "final_boss.h"

#include "start_screen.h"

const int fireSpellNum = 4;
const int guardNum = 6;
const int goblinNum = 4;

class Game
{
public:
	const unsigned int nextFrameTime = 17;

	const int badGuardDieLimit = 4;
	const int goodGuardDieLimit = 2;

	Game();
	~Game();

	bool startGameScreen();
	void showInstructScreen();
	bool handlingKeyboardEvents();

	void setObjectPlusVelocity();
	void moveObjects();

	void detectTouchingObjects();
	void checkDefeatedGuards();

	void updateScreen();

	void runGameLoop();

	void playGame();

private:

	Mix_Music* gameMusic;
	int volume;

	bool choseInstruct1, choseInstruct2;

	SDL_Event e = { NULL };
	unsigned int gameTime;
	unsigned int currentTime;

	SDL_Renderer* render = nullptr;
	TTF_Font* font = nullptr;

	StartScreen* startScreen = nullptr;
	SDL_Texture* instruct1;
	SDL_Texture* instruct2;

	Background* background = nullptr;

	Guard* guard[guardNum] = { nullptr };
	int tempGuardPosX[guardNum], tempGuardPosY[guardNum], tempGuardWidth[guardNum], tempGuardHeight[guardNum];
	int guardNameCount;

	Goblin* goblin[goblinNum] = { nullptr };
	int tempGoblinPosX[goblinNum], tempGoblinPosY[goblinNum], tempGoblinWidth[goblinNum], tempGoblinHeight[goblinNum];

	MyCharacter* wizard = nullptr;
	int tempCharPosX, tempCharPosY, tempCharWidth, tempCharHeight;

	Fire* fire[fireSpellNum] = { nullptr };
	int tempFirePosX[fireSpellNum], tempFirePosY[fireSpellNum], tempFireWidth[fireSpellNum], tempFireHeight[fireSpellNum];

	FinalBoss* boss = nullptr;
	bool showFinalBoss;

	int defeatBadGuard, defeatGoodGuard;
};

