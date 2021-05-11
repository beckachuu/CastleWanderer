#pragma once
#include "basic_init.h"

enum bossPoses { closeAttack1, closeAttack2, closeAttack3, closeAttack4, closeAttack5,
	distantAttack1, distantAttack2, distantAttack3, distantAttack4, die1, die2, die3, die4, die5, totalBossPoses };

class FinalBoss
{
public:

	const std::string bossName = "Commander Pharaoh from far far away";
	const int bossAttackDistant = 110;

	const int bossApproxWidth = 220;
	const int bossApproxHeight = 350;

	const Uint32 nextFrameTime = 120;

	const Uint32 maxNextMovetime = 6000;
	const Uint32 minNextMovetime = 2000;

	const int walkLimit = 570;
	const int baseGround = SCREEN_HEIGHT - 5;
	const int maxLeftPos = 215;
	const int maxRightPos = SCREEN_WIDTH - 15;

	const int bossVelocity = 4;

	const int health = 400;
	const int healthRectLength = 188;

	const int maxAttackStrength = 7;
	const int minAttackStrength = 1;
	const Uint32 nextAttackTime = 250;

	const int nextTeleportTime = 5000;
	const int chanceOfTeleport = 5;

	FinalBoss();
	~FinalBoss();


	void setSpriteClips();
	void renderBoss(SDL_Renderer* renderer);
	void renderSpeechBubble(SDL_Renderer* renderer);
	void renderHealthBar(SDL_Renderer* renderer);
	void renderCurrentAction(SDL_Renderer* renderer, unsigned int currentTime);

	void attackTarget(int targetPosX, int targetWidth, int targetFeetPosY, int currentTime);
	void checkBossLimits();
	void move(int targetPosX, int targetWidth, int targetFeetPosY, unsigned int currentTime);

	void receiveAttack(int damage, int currentTime);
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

	int bossLeftPoint, bossPosY;
	int bossRightPoint, bossFeetPoint;

	int bossWalkVelocity;
	int plusVelocity;
	int bossVelX, bossVelY;

	int bossWidth;
	int bossHeight;

	bool toLeft;
	bool toRight;
	bool walking;

	int damageReceived;
	bool die;

	int attackStrength;
	Uint32 attackTime;
	bool attackClose;

	Uint32 teleportTime;
};
