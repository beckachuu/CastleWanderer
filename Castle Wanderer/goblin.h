#pragma once
#include "basic_init.h"

enum goblinPoses { walk0, walk1, walk2, walk3, walk4, angry1, angry2, angry3, angry4, angry5, explode, totalGoblinPoses };

class Goblin
{
public:

	const Uint32 nextFrameTime = 200;

	const Uint32 maxNextMoveTime = 4000;
	const Uint32 minNextMoveTime = 1000;

	const Uint32 maxNextSpeakTime = 15000;
	const Uint32 minNextSpeakTime = 10000;

	const int walkLimit = SCREEN_HEIGHT - 170 - 125;
	const int baseGround = SCREEN_HEIGHT - 125;
	const int jumpHeight = 180;

	const int maxExplodeDamage = 140;
	const int minExplodeDamage = 70;


	Goblin();
	void reviveGoblin();

	~Goblin();


	void setSpriteClips();
	void renderGoblin(SDL_Renderer* renderer, SDL_Rect* clip = NULL);
	void renderGoblinSpeech(SDL_Renderer* renderer);
	void setCurrentFrame(int currentTime);
	void renderCurrentAction(SDL_Renderer* renderer);

	void randomSpeech();
	void moveRandom(unsigned int currentTime);
	void chaseTarget(int targetLeft, int targetRight, int targetFeetPoint);
	void move(int targetLeft, int targetRight, int targetFeetPoint, unsigned int currentTime);
	void checkGoblinLimits();

	void getAngry(unsigned int currentTime);
	bool isAngry();
	int getExplodeDamage();

	bool isDead();


	int getGoblinPosX();
	int getGoblinFeetPoint();

	void setPlusVelocity(int bgVelocity);
	int getGoblinVelX();
	int getGoblinVelY();

	int getGoblinWidth();
	int getGoblinHeight();

	void free();

private:

	Uint32 frameTime;
	int frame;
	int nextOrBackFrame;

	Uint32 moveTime;
	Uint32 nextMoveTime;

	Uint32 speakTime;
	Uint32 nextSpeakTime;
	Uint32 eraseSpeechTime;

	SDL_Texture* goblinTexture;
	SDL_Rect goblinSpriteClips[totalGoblinPoses];

	SDL_Texture* bubbleSpeechTexture;
	SDL_Rect bubbleSpeechRect;

	SDL_Texture* speechTexture;
	SDL_Rect speechRect;


	int rightmostGoblinPos, leftmostGoblinPos;
	int goblinPosX, goblinPosY;

	int goblinWalkVelocity;
	int plusVelocity;
	int goblinVelX, goblinVelY;

	int goblinWidth;
	int goblinHeight;

	bool toLeft;
	bool toRight;
	bool walking;

	bool angry;
	int explodeDamage;
	bool exploded;
};
