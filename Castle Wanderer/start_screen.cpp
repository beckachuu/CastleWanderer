#include "start_screen.h"

StartScreen::StartScreen() {

	startScreenMusic = loadFromMusic("music/startScreenMusic.mp3");
	Mix_PlayMusic(startScreenMusic, -1);

	startCloudsBGTexture = loadFromImage("image/start screen clouds bg.png");
	startCloudsBGRect.x = 0;
	startCloudsBGRect.y = 0;
	startCloudsBGRect.w = SCREEN_WIDTH;
	startCloudsBGRect.h = SCREEN_HEIGHT;

	startForestBGTexture = loadFromImage("image/start screen forest bg.png");
	startForestBGRect.x = 0;
	startForestBGRect.y = 0;
	startForestBGRect.w = SCREEN_WIDTH;
	startForestBGRect.h = SCREEN_HEIGHT;

	instructButtonTexture = loadFromImage("image/instruction button.png");
	startButtonTexture = loadFromImage("image/start button.png");

	textTexture = loadFromImage("image/start screen bg text.png");

	mouseX = 0;
	mouseY = 0;

	mouseOnStart = false;
	mouseOnInstruct = false;

	quitGame = false;
}

StartScreen::~StartScreen() {
	deleteStartScreen();
}

void StartScreen::runStartScreen(SDL_Renderer* renderer, SDL_Event* e) {
    while (handlingMouseEvents(e)) {
		SDL_RenderClear(renderer);

		renderStartScreen(renderer);
		SDL_RenderPresent(renderer);

		SDL_Delay(50);
    }
}

bool StartScreen::handlingMouseEvents(SDL_Event* e) {
    while (SDL_PollEvent(e) != 0) {
		if (e->type == SDL_MOUSEMOTION)
		{
			SDL_GetMouseState(&mouseX, &mouseY);

			if (mouseX >= startButtonLeft && mouseX <= startButtonRight &&
				mouseY >= startButtonTop && mouseY <= startButtonBot)
			{
				mouseOnStart = true;
				mouseOnInstruct = false;
			}
			else if (mouseX >= instructButtonLeft && mouseX <= instructButtonRight &&
				mouseY >= instructButtonTop && mouseY <= instructButtonBot)
			{
				mouseOnStart = false;
				mouseOnInstruct = true;
			}
			else {
				mouseOnStart = false;
				mouseOnInstruct = false;
			}
		}
		else if (e->type == SDL_MOUSEBUTTONDOWN) {
			if (mouseX >= startButtonLeft && mouseX <= startButtonRight &&
				mouseY >= startButtonTop && mouseY <= startButtonBot)
			{
				return false;
			}
		}

		if (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_ESCAPE) {
			quitGame = true;
			return false;
		}
    }
    return true;
}


void StartScreen::renderButtons(SDL_Renderer* renderer) {
	if (mouseOnStart) {
		SDL_SetTextureAlphaMod(startButtonTexture, 255);
	}
	else {
		SDL_SetTextureAlphaMod(startButtonTexture, 150);
	}
	SDL_RenderCopy(renderer, startButtonTexture, NULL, NULL);

	if (mouseOnInstruct) {
		SDL_SetTextureAlphaMod(instructButtonTexture, 255);
	}
	else {
		SDL_SetTextureAlphaMod(instructButtonTexture, 150);
	}
	SDL_RenderCopy(renderer, instructButtonTexture, NULL, NULL);


	SDL_RenderCopy(renderer, textTexture, NULL, NULL);
}

void StartScreen::renderStartScreen(SDL_Renderer* renderer) {
	SDL_RenderCopy(renderer, startCloudsBGTexture, NULL, &startCloudsBGRect);
	if (startCloudsBGRect.x < 0) {
		startCloudsBGRect.x += SCREEN_WIDTH;
		SDL_RenderCopy(renderer, startCloudsBGTexture, NULL, &startCloudsBGRect);
	}
	else if (startCloudsBGRect.x > 0) {
		startCloudsBGRect.x -= SCREEN_WIDTH;
		SDL_RenderCopy(renderer, startCloudsBGTexture, NULL, &startCloudsBGRect);
	}
	startCloudsBGRect.x += 1;

	SDL_RenderCopy(renderer, startForestBGTexture, NULL, &startForestBGRect);
	if (startForestBGRect.x < 0) {
		startForestBGRect.x += SCREEN_WIDTH;
		SDL_RenderCopy(renderer, startForestBGTexture, NULL, &startForestBGRect);
	}
	else if (startForestBGRect.x > 0) {
		startForestBGRect.x -= SCREEN_WIDTH;
		SDL_RenderCopy(renderer, startForestBGTexture, NULL, &startForestBGRect);
	}
	startForestBGRect.x += 3;

	renderButtons(renderer);
}

bool StartScreen::isQuittingGame() {
	return quitGame;
}

void StartScreen::deleteStartScreen() {
	freeTexture(textTexture);
	freeMusic(startScreenMusic);
}