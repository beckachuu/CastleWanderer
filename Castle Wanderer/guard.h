#pragma once
#include "basic_init.h"

enum guardPoses { guardWalk0, guardWalk1, guardWalk2, guardWalk3, guardWalk4, attack1, attack2, totalGuardPoses };

class Guard
{
public:

	const Uint32 nextFrameTime = 200;

	const Uint32 maxNextMovetime = 6000;
	const Uint32 minNextMovetime = 2000;

	const int walkLimit = 350;
	const int baseGround = SCREEN_HEIGHT - 200;
	const int jumpHeight = 210;

	const int maxVelocity = 5;
	const int minVelocity = 3;

	const int maxHealth = 120;
	const int minHealth = 70;
	const int healthbarLength = 188;

	const int maxAttackStrength = 7;
	const int minAttackStrength = 1;
	const Uint32 nextAttackTime = 200;
	const int getAngryTime = 500;

	Guard(int* guardNameCount);
	void reviveGuard(int* guardNameCount);
	~Guard();


	void setSpriteClips();
	void renderGuard(SDL_Renderer* renderer, SDL_Rect* clip = NULL);
	void renderSpeechBubble(SDL_Renderer* renderer);
	void renderHealthBar(SDL_Renderer* renderer);
	void setCurrentFrame(int currentTime);
	void renderCurrentAction(SDL_Renderer* renderer);

	bool okayToSpeak;
	void randomSpeech();
	void moveRandom(unsigned int currentTime);
	void chaseTarget(int targetLeft, int targetRight, int targetFeetPoint);
	void move(int targetLeft, int targetRight, int targetFeetPoint, unsigned int currentTime);
	void checkGuardLimits();

	void receiveAttack(int damage, int currentTime);
	bool isAttacking();
	int getAttackDamage(unsigned int currentTime);

	bool isDead();
	bool isGood();

	int getGuardPosX();
	int getGuardFeetPoint();

	void setPlusVelocity(int bgVelocity);
	int getGuardVelX();
	int getGuardVelY();

	int getGuardWidth();
	int getGuardHeight();

	void free();

private:

	Uint32 frameTime;
	int frame;
	int nextOrBackFrame;

	Uint32 moveTime;
	Uint32 nextMoveTime;

	Uint32 speakTime;
	Uint32 nextSpeakTime;
	Uint32 speechLeftToSay;

	std::string guardName;
	SDL_Texture* guardNameTexture;
	SDL_Rect guardNameRect;

	SDL_Texture* guardTexture;
	SDL_Rect guardSpriteClips[totalGuardPoses];

	SDL_Texture* bubbleSpeechTexture;
	SDL_Rect bubbleSpeechRect;

	SDL_Texture* speechTexture;
	SDL_Rect speechRect;

	SDL_Texture* healthTexture;
	SDL_Rect healthRect;

	SDL_Texture* healthBarTexture;
	SDL_Rect healthBarRect;

	int rightmostGuardPos, leftmostGuardPos;
	int guardPosX, guardPosY;

	int guardWalkVelocity;
	int plusVelocity;
	int guardVelX, guardVelY;

	int guardWidth;
	int guardHeight;

	bool toLeft;
	bool toRight;
	bool walking;

	bool righteous;

	int health;
	int damageReceived;
	bool die;

	int attackStrength;
	Uint32 attackTime;
	bool attacking;
};
