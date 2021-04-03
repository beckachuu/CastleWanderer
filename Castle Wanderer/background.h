#pragma once
#include <iostream>
#include <SDL.h>


class Background {
public:

	//Axis velocity
	const int foreBG_Velocity = 6, bgForest_velocity = 5, castle_velocity = 4, clouds_Velocity = 2;
	const int roadWidth = 190;

	//Map length = four road scenes
	const int BGleftMostX = -1280 * 3;
	const int BGrightMostX = 0;

	//Initializes variables
	Background(SDL_Renderer* renderer);

	//Deallocates memory
	~Background();

	//Renders texture at given point
	void render(SDL_Renderer* renderer);
	void renderCurrentFrame(SDL_Renderer* renderer);

	//Move
	void handledEvent(SDL_Event& e, SDL_Renderer* renderer);
	void move();

	//Deallocates texture
	void free();

	bool toFurthestLeftX();
	bool toFurthestRightX();

	int getBGVelX();
	int getforeBG_X();
	int getFurthestLeftPoint();

private:

	//The actual hardware texture
	SDL_Texture* cloudsTexture, *castleTexture, *backgroundForestTexture, *treeAndBushesTexture, *tavernTexture, *roadTexture;

	//The X and Y offsets
	int clouds_X, castle_X, bgForest_X, foreBG_X;
	//int clouds_Y, castle_Y, bgForest_Y, foreBG_Y;
	int road_Y;

	//Velocity
	int clouds_VelX, castle_VelX, bgForest_VelX, foreBG_VelX;
	//int clouds_VelY, castle_VelY, bgForest_VelY, foreBG_VelY;

	SDL_Rect renderClouds, renderCastle, renderbgForest, renderBushes, renderTavern, renderRoad;

	bool atFurthestLeftX;
	bool atFurthestRightX;
};