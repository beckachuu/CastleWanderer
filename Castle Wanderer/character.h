#pragma once
#include "basic_init.h"

enum characterPoses { walkR1, walkR2, walkR3, walkR4, walkR5, walkR6,
	walkL1, walkL2, walkL3, walkL4, walkL5, walkL6, stand, attack, totalCharacterPoses};

const int max_fire_spell = 7;

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

	//Initializes variables
	MyCharacter();

	//Deallocates memory
	~MyCharacter();

	//Renders texture at given point
	void setSpriteClips();
	void render(SDL_Renderer* renderer, SDL_Rect* clip = NULL);
	void renderCurrentAction(SDL_Renderer* renderer);

	//Move
	void handleEvent(SDL_Event& e, SDL_Renderer* renderer);
	void move();
	void moveBackX(int vel);
	void moveBackY(int vel);
	void setFurthestPoints(int furthestLeftPoint, int furthestRightPoint);

	bool isAtEdgeOfScreen();
	bool isToRight();
	
	//Gets image offsets
	int getcharPosX();
	int getcharPosY();

	//Gets image velocity
	void setVelocity(int BguardWalkVelocity);
	int getcharVelX();
	int getcharVelY();

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
	SDL_Rect characterSpriteClips[totalCharacterPoses];

	//The actual hardware texture
	SDL_Texture* cTexture;

	//The X and Y offsets
	int charPosX, charPosY, ground;
	int leftmostCharacterPos;
	int rightmostCharacterPos;
	//Velocity
	int charVelX, charVelY;
	//Image dimensions
	int cWidth;
	int cHeight;

	int frame;

	Uint32 frameTime;

	bool toLeft;
	bool toRight;
	bool walking;
	bool jumped;
	bool atEdgeOfScreen;

	int health;
};

