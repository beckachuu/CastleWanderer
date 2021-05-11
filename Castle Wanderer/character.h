#pragma once
#include "basic_init.h"

enum characterPoses { walkR1, walkR2, walkR3, walkR4, walkR5, walkR6,
	walkL1, walkL2, walkL3, walkL4, walkL5, walkL6, stand, attack, totalCharacterPoses};

class MyCharacter
{
public:

	const Uint32 nextFrameTime = 170;

	const int walkLimit = 350;
	const int baseGround = SCREEN_HEIGHT - 200;
	const int jumpHeight = 210;

	const int health = 210;


	MyCharacter();
	~MyCharacter();


	void setSpriteClips();
	void renderCharacter(SDL_Renderer* renderer, SDL_Rect* clip = NULL);
	void renderHealthBar(SDL_Renderer* renderer);
	void renderCurrentAction(SDL_Renderer* renderer, unsigned int currentTime);

	void handleEvent(SDL_Event& e, unsigned int currentTime);
	void move();
	void checkCharLimits();
	void setFurthestPoints(int furthestLeftPoint, int furthestRightPoint);

	bool isAtEdgeOfScreen();
	bool isToRight();

	void receiveDamage(int damage);
	void heal(int guard);

	int getCharPosX();
	int getCharPosY();

	void setVelocity(int BGVelocity);
	int getCharVelX();
	int getCharVelY();

	int getWidth();
	int getHeight();

	void free();

private:

	Uint32 frameTime;
	int frame;

	SDL_Texture* charTexture;
	SDL_Rect characterSpriteClips[totalCharacterPoses];

	SDL_Texture* charNameTexture;
	SDL_Rect charNameRect;

	SDL_Texture* healthTexture;
	SDL_Rect healthRect;

	SDL_Texture* healthBarTexture;
	SDL_Rect healthBarRect;

	int leftmostCharacterPos;
	int rightmostCharacterPos;
	int charPosX, charPosY, ground;

	int charVelocity;
	int charVelocityJump;
	int charVelX, charVelY;

	int charWidth;
	int charHeight;

	bool toLeft;
	bool toRight;
	bool walking;
	bool jumped;
	bool atEdgeOfScreen;

	int damageReceived;
	bool die;
};
