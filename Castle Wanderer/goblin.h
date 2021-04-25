#pragma once
#include "basic_init.h"

enum goblinPoses { walk0, walk1, walk2, walk3, walk4, angry1, angry2, angry3, angry4, angry5, explode, totalGoblinPoses };

//Texture wrapper class
class Goblin
{
public:

	//Limit for position
	const int walkLimit = SCREEN_HEIGHT - 170 - 125;
	const int baseGround = SCREEN_HEIGHT - 125;
	const int jumpHeight = 180;

	//Time to change to next frame
	const int nextFrameTime = 200;

	const int maxNextMoveTime = 4000; //5 secs actually
	const int minNextMoveTime = 1000;

	const int maxNextSpeakTime = 15000; //25 secs actually
	const int minNextSpeakTime = 10000;

	//Attack strength
	const int attackStrength = 10;

	//Speech settings
	const Uint32 textWrapLength = 250;

	//Initializes variables
	Goblin();

	//Deallocates memory
	~Goblin();

	//Renders texture at given point
	void setSpriteClips();
	void render(SDL_Renderer* renderer, SDL_Rect* clip = NULL);
	void renderCurrentAction(SDL_Renderer* renderer);

	//Auto control NPC character
	void randomSpeech();

	void moveRandom();
	void move();
	void moveBackX(int vel);
	void moveBackY(int vel);

	//Deallocates texture
	void free();

	//Gets image dimensions
	int getGoblinWidth();
	int getGoblinHeight();

	//Gets image offsets
	int getGoblinPosX();
	int getGoblinPosY();

	//Gets image velocity
	void setPlusVelocity(int bgVelocity);
	int getGoblinVelX();
	int getGoblinVelY();
	void setVelocity(int bgVelocity);

private:
	//Maximum axis velocity
	int goblinWalkVelocity;
	int goblinJumpVelocity;

	//Goblin and speech render spaces
	SDL_Rect goblinSpriteClips[totalGoblinPoses];
	SDL_Rect renderSpeech;

	//The actual hardware texture
	SDL_Texture* goblinTexture;
	SDL_Texture* speechTexture;
	SDL_Texture* bubbleSpeechTexture;

	//The X and Y offsets
	int goblinPosX, goblinPosY;
	int ground;
	int rightmostGoblinPos, leftmostGoblinPos;

	//Velocity
	int plusVelocity;
	int goblinVelX, goblinVelY;

	//Image dimensions
	int goblinWidth;
	int goblinHeight;

	//Frame order and time
	int frame;
	int nextOrBackFrame;

	Uint32 frameTime;
	Uint32 moveTime;
	Uint32 nextMoveTime;
	Uint32 speakTime;
	Uint32 nextSpeakTime;
	Uint32 eraseSpeechTime;

	//Moving status
	bool toLeft;
	bool toRight;
	bool walking;
	bool jumped;

	int health;
};
