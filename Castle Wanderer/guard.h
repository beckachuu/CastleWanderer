#pragma once
#include "basic_init.h"

enum guardPoses { walk0, walk1, walk2, walk3, walk4, attack1, attack2, totalGuardPoses };

//Texture wrapper class
class Guard
{
public:

	//Maximum axis velocity
	const int gVelocity = 6;
	const int gVelocityJump = 24;

	//Time to change to next frame
	const int nextFrameTime = 200;
	const int maxNextMovetime = 2000; //3000 millisecs actually
	const int minNextMovetime = 1000;

	//Limit for position
	const int walkLimit = 350;
	const int baseGround = SCREEN_HEIGHT - 200;
	const int leftMostGuardPos = -200;
	const int jumpHeight = 210;

	//Attack strength
	const int attackStrength = 10;

	//Initializes variables
	Guard(SDL_Renderer* renderer);

	//Deallocates memory
	~Guard();

	//Loads image at specified path
	bool loadFromFile(std::string path, SDL_Renderer* renderer);

	//Renders texture at given point
	void setSpriteClips();
	void render(SDL_Renderer* renderer, SDL_Rect* clip = NULL);
	void renderCurrentAction(SDL_Renderer* renderer);

	//Auto control NPC character
	void randomSpeech();
	void moveRandom();
	void move();
	void attack();
	void avoidAttack();

	//Deallocates texture
	void free();

	//Gets image dimensions
	int getgWidth();
	int getgHeight();

	//Gets image offsets
	int getguardPosX();
	int getguardPosY();

	//Gets image velocity
	void setPlusVelocity(int bgVelocity);
	void setRightLimit(int bgLeftMostPos);
	int getguardVelX();
	int getguardVelY();

private:

	//Scene textures
	SDL_Rect guardSpriteClips[totalGuardPoses];

	//The actual hardware texture
	SDL_Texture* guardTexture;

	//The X and Y offsets
	int guardPosX, guardPosY, ground;
	int rightmostGuardPos;
	//Velocity
	int plusVelocity;
	int guardVelX, guardVelY;
	//Image dimensions
	int gWidth;
	int gHeight;

	int frame;
	Uint32 frameTime;
	int nextOrBackFrame;
	int moveTime;
	int tempTime;

	bool toLeft;
	bool toRight;
	bool walking;
	bool jumped;
	bool falling;

	bool righteous;
	SDL_Texture* speechTexture;

	int health;
};

