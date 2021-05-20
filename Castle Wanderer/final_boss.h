#pragma once
#include "basic_init.h"

enum bossPoses { closeAttack1, closeAttack2, closeAttack3, closeAttack4, closeAttack5,
	healing1, healing2, healing3, healing4, die1, die2, die3, die4, die5, totalBossPoses };

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

	const int health = 300;
	const int healthRectLength = 345;
	const int minHealing = 7;
	const int nextHealTime = nextFrameTime * 4;

	const int maxAttackStrength = 30;
	const int minAttackStrength = 12;
	const Uint32 nextAttackTime = nextFrameTime * 4;

	const int nextTeleportTime = 5000;
	const int chanceOfTeleport = 5;

	FinalBoss();
	void resetBoss();
	~FinalBoss();


	void setSpriteClips();
	void renderBoss(SDL_Renderer* renderer);
	void renderHealthBar(SDL_Renderer* renderer);
	void setCurrentFrame(int currentTime);
	void renderCurrentAction(SDL_Renderer* renderer);

	void attackTarget(int targetPosX, int targetWidth, int targetFeetPosY, int currentTime);
	bool isAttackingClose();
	void checkBossLimits();
	void move(int targetLeft, int targetRight, int targetFeetPoint, unsigned int currentTime);

	void receiveAttack(int damage, int currentTime);
	void selfHealing(int currentTime);
	int getAttackDamage(unsigned int currentTime);

	bool isDead();


	int getBossPosX();
	int getBossFeetPoint();

	void setPlusVelocity(int bgVelocity);
	int getBossVelX();
	int getBossVelY();

	int getBossWidth();
	int getBossHeight();

	SDL_Texture* getBossTexture();
	SDL_Rect* getBossSrcRect();
	SDL_Rect* getBossDstRect();

	void free();

private:

	Uint32 frameTime;
	int frame;

	Uint32 moveTime;

	SDL_Texture* bossNameTexture;
	SDL_Rect bossNameRect;

	SDL_Texture* bossTexture;
	SDL_Rect bossSpriteClips[totalBossPoses];

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

	Uint32 healtime;

	Uint32 teleportTime;
};
