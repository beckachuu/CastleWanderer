#pragma once
#include "basic_init.h"

enum bossPoses { closeAttack1, closeAttack2, closeAttack3, closeAttack4, closeAttack5,
	distantAttack1, distantAttack2, distantAttack3, distantAttack4, die1, die2, die3, die4, die5, totalBossPoses };

class FinalBoss
{
public:

	const std::string bossName="Commander Pharaoh from far far away";

	const Uint32 nextFrameTime = 200;

	const Uint32 maxNextMovetime = 6000;
	const Uint32 minNextMovetime = 2000;

	const int walkLimit = 350;
	const int baseGround = SCREEN_HEIGHT - 200;
	const int jumpHeight = 210;

	const int bossVelocity = 5;

	const int maxHealth = 150;
	const int minHealth = 100;

	const int maxAttackStrength = 7;
	const int minAttackStrength = 1;
	const Uint32 nextAttackTime = 200;


	FinalBoss(int* bossNameCount);
	~FinalBoss();


	void setSpriteClips();
	void renderBoss(SDL_Renderer* renderer, SDL_Rect* clip = NULL);
	void renderSpeechBubble(SDL_Renderer* renderer);
	void renderHealthBar(SDL_Renderer* renderer);
	void renderCurrentAction(SDL_Renderer* renderer, unsigned int currentTime);

	void move(int targetPosX, int targetPosY, int targetWidth, unsigned int currentTime);
	void checkBossLimits();

	void receiveAttack(int damage, int currentTime);
	void chaseTarget(int targetPosX, int targetPosY, int targetWidth);
	bool isAttacking();
	int getAttackDamage(unsigned int currentTime);

	bool isDead();


	int getBossPosX();
	int getBossPosY();

	void setPlusVelocity(int bgVelocity);
	int getBossVelX();
	int getBossVelY();

	int getBossWidth();
	int getBossHeight();

	void free();

private:

	Uint32 frameTime;
	int frame;

	Uint32 moveTime;
	Uint32 nextMoveTime;

	Uint32 speakTime;
	Uint32 nextSpeakTime;

	SDL_Texture* bossNameTexture;
	SDL_Rect bossNameRect;

	SDL_Texture* bossTexture;
	SDL_Rect bossSpriteClips[totalBossPoses];

	SDL_Texture* bubbleSpeechTexture;
	SDL_Rect bubbleSpeechRect;

	SDL_Texture* speechTexture;
	SDL_Rect speechRect;

	SDL_Texture* healthTexture;
	SDL_Rect healthRect;

	SDL_Texture* healthBarTexture;
	SDL_Rect healthBarRect;

	int bossPosX, bossPosY;

	int bossWalkVelocity;
	int plusVelocity;
	int bossVelX, bossVelY;

	int bossWidth;
	int bossHeight;

	bool toLeft;
	bool toRight;
	bool walking;

	int health;
	int damageReceived;
	bool die;

	int attackStrength;
	Uint32 attackTime;
	bool attacking;
};
