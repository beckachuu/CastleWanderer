#pragma once
#include <iostream>
#include <SDL.h>


class Background {
public:

	const int roadWidth = 190;
	const int road_Y = 510;

	//Map length = four road scenes
	const int BGleftMostX = -1280 * 3;
	const int BGrightMostX = 0;

	Background();
	void resetBackGround();
	~Background();

	void render(SDL_Renderer* renderer);
	void renderCurrentFrame(SDL_Renderer* renderer);

	void handledEvent(SDL_Event& e, SDL_Renderer* renderer);
	void move();

	void free();

	bool isAtFurthestLeftX();
	bool isAtFurthestRightX();

	int getObjectPlusVelocity();
	int getBGVelocity();
	int getFurthestLeftPoint();


private:
	int foreBG_Velocity, bgForest_velocity, castle_velocity, clouds_Velocity;

	SDL_Texture* cloudsTexture, *castleTexture, *backgroundForestTexture, *treeAndBushesTexture, *tavernTexture, *roadTexture;

	int clouds_X, castle_X, bgForest_X, foreBG_X;

	int clouds_VelX, castle_VelX, bgForest_VelX, foreBG_VelX;

	SDL_Rect renderClouds, renderCastle, renderbgForest, renderBushes, renderTavern, renderRoad;

	bool atFurthestLeftX;
	bool atFurthestRightX;

	int objectPlusVelocity;
};