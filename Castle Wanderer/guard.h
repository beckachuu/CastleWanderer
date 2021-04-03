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
	const int maxNextMovetime = 4000; //6000 millisecs actually
	const int minNextMovetime = 2000;

	//Limit for position
	const int walkLimit = 350;
	const int baseGround = SCREEN_HEIGHT - 200;
	const int leftMostGuardPos = 0;
	const int jumpHeight = 210;

	//Attack strength
	const int attackStrength = 10;

	//Speech settings
	const int speechOffset = 10;
	const int nextSpeakTime = 7000;
	const Uint32 textWrapLength = 250;

	//Initializes variables
	Guard(SDL_Renderer* renderer);

	//Deallocates memory
	~Guard();

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

	//Guard and speech render spaces
	SDL_Rect guardSpriteClips[totalGuardPoses];
	SDL_Rect renderSpeech;

	//The actual hardware texture
	SDL_Texture* guardTexture;
	SDL_Texture* speechTexture;
	SDL_Texture* bubbleSpeechTexture;

	//The X and Y offsets
	int guardPosX, guardPosY;
	int ground;
	int rightmostGuardPos;

	//Velocity
	int plusVelocity;
	int guardVelX, guardVelY;

	//Image dimensions
	int gWidth;
	int gHeight;

	//Frame order and time
	int frame;
	int nextOrBackFrame;

	Uint32 frameTime;
	Uint32 moveTime;
	Uint32 speakTime;

	//Moving status
	bool toLeft;
	bool toRight;
	bool walking;
	bool jumped;
	bool falling;

	//Act as good or bad
	bool righteous;

	//Initial health
	int health;

};

