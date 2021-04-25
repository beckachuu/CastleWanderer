#pragma once
#include "basic_init.h"

enum guardPoses { guardWalk0, guardWalk1, guardWalk2, guardWalk3, guardWalk4, attack1, attack2, totalGuardPoses };

//Texture wrapper class
class Guard
{
public:
	//Limit for position
	const int walkLimit = 350;
	const int baseGround = SCREEN_HEIGHT - 200;
	const int jumpHeight = 210;

	const int maxVelocity = 8;
	const int minVelocity = 4;

	//Frame settings
	const Uint32 nextFrameTime = 400;
	const Uint32 maxNextMovetime = 6000;
	const Uint32 minNextMovetime = 2000;

	const Uint32 textWrapLength = 250;

	const int maxHealth = 150;
	const int minHealth = 100;

	const int maxAttackStrength = 7;
	const int minAttackStrength = 1;
	const Uint32 nextAttackTime = 700;

	Guard(int* guardNameCount);
	~Guard();

	void setSpriteClips();
	void renderGuard(SDL_Renderer* renderer, SDL_Rect* clip = NULL);
	void renderSpeechBubble(SDL_Renderer* renderer);
	void renderHealthBar(SDL_Renderer* renderer);
	void renderCurrentAction(SDL_Renderer* renderer);

	bool okayToSpeak;
	void randomSpeech();
	void moveRandom();

	void move(int targetPosX, int targetPosY, int targetWidth);
	void checkGuardLimits();

	void receiveAttack(int damage);
	bool isDead();
	void chaseTarget(int targetPosX, int targetPosY, int targetWidth);
	void attack();
	bool isAttacking();
	int getAttackDamage();

	//Deallocates texture
	void free();

	//Gets image dimensions
	int getGuardWidth();
	int getGuardHeight();

	//Gets image offsets
	int getGuardPosX();
	int getGuardPosY();

	//Gets image velocity
	void setPlusVelocity(int bgVelocity);
	int getGuardVelX();
	int getGuardVelY();

private:
	std::string guardName;

	int guardWalkVelocity;

	//Guard and speech render spaces
	SDL_Rect guardNameRect;
	SDL_Rect guardSpriteClips[totalGuardPoses];
	SDL_Rect speechBubbleRect;
	SDL_Rect healthBarRect;
	SDL_Rect healthRect;

	//The actual hardware texture
	SDL_Texture* guardNameTexture;
	SDL_Texture* guardTexture;
	SDL_Texture* speechTexture;
	SDL_Texture* bubbleSpeechTexture;
	SDL_Texture* healthBarTexture;
	SDL_Texture* healthTexture;

	//The X and Y offsets
	int guardPosX, guardPosY;
	int rightmostGuardPos, leftmostGuardPos;

	//Velocity
	int plusVelocity;
	int guardVelX, guardVelY;

	//Image dimensions
	int guardWidth;
	int guardHeight;

	//Frame order and time
	int frame;
	int nextOrBackFrame;

	Uint32 frameTime;

	Uint32 moveTime;
	Uint32 nextMoveTime;

	Uint32 speakTime;
	Uint32 nextSpeakTime;
	Uint32 speechLeftToSay;

	//Moving status
	bool toLeft;
	bool toRight;
	bool walking;

	//Act as good or bad
	bool righteous;

	//Health control
	int health;
	int damageReceived;
	bool die;

	//Attack control
	int attackStrength;
	Uint32 attackTime;
	bool attacking;
};
