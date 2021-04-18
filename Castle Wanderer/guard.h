#pragma once
#include "basic_init.h"

enum guardPoses { guardWalk0, guardWalk1, guardWalk2, guardWalk3, guardWalk4, attack1, attack2, totalGuardPoses };

//Texture wrapper class
class Guard
{
public:

	//Time to change to next frame
	const int nextFrameTime = 200;
	const int maxNextMovetime = 4000; //6000 millisecs actually
	const int minNextMovetime = 2000;

	//Limit for position
	const int walkLimit = 350;
	const int baseGround = SCREEN_HEIGHT - 200;
	const int jumpHeight = 210;

	//Attack strength
	const int attackStrength = 10;

	//Speech settings
	const int nextSpeakTime = 4000;
	const Uint32 textWrapLength = 250;

	//Initializes variables
	Guard();

	//Deallocates memory
	~Guard();

	//Renders texture at given point
	void setSpriteClips();
	void render(SDL_Renderer* renderer, SDL_Rect* clip = NULL);
	void renderCurrentAction(SDL_Renderer* renderer);

	//Auto control NPC character
	void randomSpeech();
	bool okayToSpeak;

	void moveRandom();
	void move();
	void moveBackX(int vel);
	void moveBackY(int vel);

	void attack();
	void avoidAttack();

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
	void setRightLimit(int bgLeftMostPos);
	int getGuardVelX();
	int getGuardVelY();
	void setVelocity(int bgVelocity);

private:
	//Maximum axis velocity
	int guardWalkVelocity;
	int guardJumpVelocity;

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

	//Moving status
	bool toLeft;
	bool toRight;
	bool walking;
	bool jumped;

	//Act as good or bad
	bool righteous;

	//Initial health
	int health;

};

