#include "start_screen.h"

StartScreen::StartScreen() {

	volume = 50;

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

	instruct1 = loadFromImage("image/instruct1.png");
	instruct2 = loadFromImage("image/instruct2.png");

	mouseX = 0;
	mouseY = 0;

	mouseOnStart = false;
	mouseOnInstruct = false;
	choseInstruct1 = false;
	choseInstruct2 = false;

	quitGame = false;
}

StartScreen::~StartScreen() {
	deleteStartScreen();
}

void StartScreen::runStartScreen(SDL_Renderer* renderer, SDL_Event* e) {
    while (handlingMouseEvents(e, renderer) && !quitGame) {
		SDL_RenderClear(renderer);

		renderStartScreen(renderer);
		SDL_RenderPresent(renderer);

		SDL_Delay(50);
    }
}

bool StartScreen::handlingMouseEvents(SDL_Event* e, SDL_Renderer* renderer) {
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

		if (e->type == SDL_MOUSEBUTTONDOWN) {
			if (mouseX >= startButtonLeft && mouseX <= startButtonRight &&
				mouseY >= startButtonTop && mouseY <= startButtonBot)
			{
				return false;
			}

			if (mouseX >= instructButtonLeft && mouseX <= instructButtonRight &&
				mouseY >= instructButtonTop && mouseY <= instructButtonBot)
			{
				choseInstruct1 = true;
				while (choseInstruct1 || choseInstruct2) {
					renderInstruct(e, renderer);
				}
				break;
			}
		}

		if (e->type == SDL_KEYDOWN) {
			switch (e->key.keysym.sym) {
			case SDLK_LEFT:
				if (volume > 0) {
					volume -= volumeChangeAmount;
				}
				Mix_VolumeMusic(volume);
				break;
			case SDLK_RIGHT:
				if (volume < MIX_MAX_VOLUME) {
					volume += volumeChangeAmount;
				}
				Mix_VolumeMusic(volume);
				break;
			case SDLK_ESCAPE:
				if (e->key.repeat == 0) {
					quitGame = true;
					return false;
				}
			}
		}

		if (e->type == SDL_QUIT) {
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

void StartScreen::renderInstruct(SDL_Event* e, SDL_Renderer* renderer) {
	while (SDL_PollEvent(e) != 0) {
		if (e->type == SDL_KEYDOWN && e->key.repeat == 0) {
			switch (e->key.keysym.sym) {
			case SDLK_a:
				choseInstruct1 = true;
				choseInstruct2 = false;
				break;
			case SDLK_d:
				choseInstruct1 = false;
				choseInstruct2 = true;
				break;

			case SDLK_LEFT:
				if (volume > 0) {
					volume -= volumeChangeAmount;
				}
				Mix_VolumeMusic(volume);
				break;
			case SDLK_RIGHT:
				if (volume < MIX_MAX_VOLUME) {
					volume += volumeChangeAmount;
				}
				Mix_VolumeMusic(volume);
				break;

			case SDLK_ESCAPE:
				choseInstruct1 = false;
				choseInstruct2 = false;
				break;
			}
		}

		if (e->type == SDL_QUIT) {
			choseInstruct1 = false;
			choseInstruct2 = false;
			quitGame = true;
		}
	}

	if (choseInstruct1) {
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, instruct1, NULL, NULL);
		SDL_RenderPresent(renderer);
	}
	else if (choseInstruct2) {
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, instruct2, NULL, NULL);
		SDL_RenderPresent(renderer);
	}
}

bool StartScreen::isQuittingGame() {
	return quitGame;
}

void StartScreen::deleteStartScreen() {
	freeTexture(textTexture);
	freeTexture(instruct1);
	freeTexture(instruct2);

	freeMusic(startScreenMusic);
}