#pragma once
#include "basic_init.h"

class StartScreen {

public:

	const int startButtonTop = 350;
	const int startButtonBot = 475;
	const int startButtonLeft = 515;
	const int startButtonRight = 735;

	const int instructButtonTop = 525;
	const int instructButtonBot = 635;
	const int instructButtonLeft = 500;
	const int instructButtonRight = 775;

	StartScreen();
	~StartScreen();

	void runStartScreen(SDL_Renderer* renderer, SDL_Event* e);
	bool handlingMouseEvents(SDL_Event* e, SDL_Renderer* renderer);
	void renderButtons(SDL_Renderer* renderer);
	void renderStartScreen(SDL_Renderer* renderer);

	void renderInstruct(SDL_Event* e, SDL_Renderer* renderer);

	bool isQuittingGame();

	void deleteStartScreen();

private:
	int volume;

	Mix_Music* startScreenMusic;

	SDL_Texture* startCloudsBGTexture;
	SDL_Rect startCloudsBGRect;

	SDL_Texture* startForestBGTexture;
	SDL_Rect startForestBGRect;

	SDL_Texture* instructButtonTexture;
	SDL_Texture* startButtonTexture;

	SDL_Texture* textTexture;

	SDL_Texture* instruct1;
	SDL_Texture* instruct2;

	int mouseX;
	int mouseY;

	bool mouseOnStart;
	bool mouseOnInstruct;
	bool choseInstruct1;
	bool choseInstruct2;

	bool quitGame;
};