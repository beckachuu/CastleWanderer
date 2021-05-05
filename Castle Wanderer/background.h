#pragma once
#include <iostream>
#include <SDL.h>


class Background {
public:

	const int roadWidth = 190;

	//Map length = four road scenes
	const int BGleftMostX = -1280 * 3;
	const int BGrightMostX = 0;

	//Initializes variables
	Background();

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

	bool isAtFurthestLeftX();
	bool isAtFurthestRightX();

	int getBGVelX();
	int getBGspeed();
	int getFurthestLeftPoint();

	void speedUp();
	void speedDown();

private:
	int foreBG_Velocity, bgForest_velocity, castle_velocity, clouds_Velocity;

	SDL_Texture* cloudsTexture, *castleTexture, *backgroundForestTexture, *treeAndBushesTexture, *tavernTexture, *roadTexture;

	int clouds_X, castle_X, bgForest_X, foreBG_X;
	//int clouds_Y, castle_Y, bgForest_Y, foreBG_Y;
	int road_Y;

	int clouds_VelX, castle_VelX, bgForest_VelX, foreBG_VelX;
	//int clouds_VelY, castle_VelY, bgForest_VelY, foreBG_VelY;

	SDL_Rect renderClouds, renderCastle, renderbgForest, renderBushes, renderTavern, renderRoad;

	bool atFurthestLeftX;
	bool atFurthestRightX;
};