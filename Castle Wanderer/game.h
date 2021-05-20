#pragma once
#include <SDL.h>
#include "render_objects.h"

#include "background.h"

#include "guard.h"
#include "goblin.h"

#include "character.h"
#include "spell_fire.h"

#include "final_boss.h"

#include "start_screen.h"

const int fireSpellNum = 4;
const int guardNum = 7;
const int goblinNum = 4;

class Game
{
public:
	const unsigned int nextFrameTime = 17;

	const int badGuardDieLimit = 4;
	const int goodGuardDieLimit = 2;

	const int characterFurthestLeft = 300;
	const int characterFurthestRight = 900;

	const int fireDistantFromGround = 90;

	Game();
	~Game();

	bool startGameScreen();
	void showInstructScreen();
	bool endGameScreen();
	bool handlingKeyboardEvents();

	void setObjectPlusVelocity();
	void moveObjects();

	void detectFireAttack();
	void detectTouchingObjects();
	void checkDefeatedGuards();

	void renderWizard(int i = -1);
	void renderGuard(int i);
	void renderGoblin(int i);
	void renderFinalBoss(int i = -1);
	void updateScreen();

	void restartGame();
	void runGameLoop();

	void playGame();

private:

	bool win, lose;
	bool quitGame;

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

	SDL_Texture* youWin;
	SDL_Texture* youLose;

	Background* background = nullptr;

	Guard* guard[guardNum] = { nullptr };
	int tempGuardLeft[guardNum], tempGuardRight[guardNum], tempGuardFeetPoint[guardNum];
	int guardNameCount;

	Goblin* goblin[goblinNum] = { nullptr };
	int tempGoblinLeft[goblinNum], tempGoblinRight[goblinNum], tempGoblinFeetPoint[goblinNum];

	MyCharacter* wizard = nullptr;
	int tempCharLeft, tempCharRight, tempCharFeetPoint;

	Fire* fire[fireSpellNum] = { nullptr };
	int tempFireLeft[fireSpellNum], tempFireRight[fireSpellNum], tempFireFeetPoint[fireSpellNum];

	FinalBoss* boss = nullptr;
	int tempBossLeft, tempBossRight, tempBossFeetPoint;
	bool showFinalBoss;

	int defeatBadGuard, defeatGoodGuard;

};

