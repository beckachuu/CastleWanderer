#pragma once
#include "basic_init.h"
#include "spell_fire.h"

enum characterPoses { walkR1, walkR2, walkR3, walkR4, walkR5, walkR6,
	walkL1, walkL2, walkL3, walkL4, walkL5, walkL6, stand, attack, totalCharacterPoses};

const int max_fire_spell = 7;

//Texture wrapper class
class myCharacter
{
public:

	//Maximum axis velocity
	const int cVelocity = 5;
	const int cVelocityJump = 24;
	//Time to change to next frame
	const int nextFrame = 170;

	//Limit for position
	const int walkLimit = 350;
	const int baseGround = SCREEN_HEIGHT - 200;
	const int jumpHeight = 210;

	//Initializes variables
	myCharacter();

	//Deallocates memory
	~myCharacter();

	//Loads image at specified path
	bool loadFromFile(std::string path, SDL_Renderer* renderer);

	//Renders texture at given point
	void setSpriteClips();
	void render(SDL_Renderer* renderer, SDL_Rect* clip = NULL);
	void renderCurrentAction(SDL_Renderer* renderer);

	//Move
	void handleEvent(SDL_Event& e, SDL_Renderer* renderer);
	void move();

	//Deallocates texture
	void free();

	//Gets image dimensions
	int getWidth();
	int getHeight();

	//Gets image offsets
	int getcPosX();
	int getcPosY();

	//Gets image velocity
	int getcVelX();
	int getcVelY();

	//Check if need to stop
	bool gotToFar;

private:

	//Scene textures
	SDL_Rect characterSpriteClips[totalCharacterPoses];

	//The actual hardware texture
	SDL_Texture* cTexture;

	//The X and Y offsets
	int cPosX, cPosY, ground;
	const int leftmostCharacterPos = 300;
	const int rightmostCharacterPos = 900;
	//Velocity
	int cVelX, cVelY;
	//Image dimensions
	int cWidth;
	int cHeight;

	int frame;

	//Fire spell
	Fire* fire[max_fire_spell] = { nullptr };

	Uint32 frameTime;

	bool toLeft;
	bool toRight;
	bool walking;
	bool jumped;
	bool falling;

	int health;
};

