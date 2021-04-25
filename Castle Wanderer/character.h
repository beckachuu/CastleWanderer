#pragma once
#include "basic_init.h"

enum characterPoses { walkR1, walkR2, walkR3, walkR4, walkR5, walkR6,
	walkL1, walkL2, walkL3, walkL4, walkL5, walkL6, stand, attack, totalCharacterPoses};

//Texture wrapper class
class MyCharacter
{
public:

	//Time to change to next frame
	const int nextFrame = 170;

	//Limit for position
	const int walkLimit = 350;
	const int baseGround = SCREEN_HEIGHT - 200;
	const int jumpHeight = 210;

	const int health = 210;

	//Initializes variables
	MyCharacter();

	//Deallocates memory
	~MyCharacter();

	//Renders texture at given point
	void setSpriteClips();
	void renderCharacter(SDL_Renderer* renderer, SDL_Rect* clip = NULL);
	void renderHealthBar(SDL_Renderer* renderer);
	void renderCurrentAction(SDL_Renderer* renderer);

	//Move
	void handleEvent(SDL_Event& e, SDL_Renderer* renderer);
	void move();
	void moveBackX(int vel);
	void moveBackY(int vel);
	void setFurthestPoints(int furthestLeftPoint, int furthestRightPoint);

	bool isAtEdgeOfScreen();
	bool isToRight();

	void receiveDamage(int damage);
	
	//Gets image offsets
	int getCharPosX();
	int getCharPosY();

	//Gets image velocity
	void setVelocity(int BguardWalkVelocity);
	int getCharVelX();
	int getCharVelY();

	//Gets image dimensions
	int getWidth();
	int getHeight();

	//Deallocates texture
	void free();

private:
	//Axis velocity
	int charVelocity;
	int charVelocityJump;

	//Scene textures
	SDL_Rect charNameRect;
	SDL_Rect characterSpriteClips[totalCharacterPoses];
	SDL_Rect healthBarRect;
	SDL_Rect healthRect;

	//The actual hardware texture
	SDL_Texture* charNameTexture;
	SDL_Texture* charTexture;
	SDL_Texture* healthBarTexture;
	SDL_Texture* healthTexture;

	//The X and Y offsets
	int charPosX, charPosY, ground;
	int leftmostCharacterPos;
	int rightmostCharacterPos;
	//Velocity
	int charVelX, charVelY;
	//Image dimensions
	int charWidth;
	int charHeight;

	int frame;

	Uint32 frameTime;

	bool toLeft;
	bool toRight;
	bool walking;
	bool jumped;
	bool atEdgeOfScreen;

	int damageReceived;
	bool die;
};

