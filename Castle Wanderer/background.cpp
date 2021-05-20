#include "background.h"
#include "basic_init.h"
#include <SDL_image.h>

Background::Background()
{
	cloudsTexture = loadFromImage("image/clouds.png");
    castleTexture = loadFromImage("image/castle.png");
	backgroundForestTexture = loadFromImage("image/background forest.png");
	treeAndBushesTexture = loadFromImage("image/tree and bushes.png");
	tavernTexture = loadFromImage("image/tavern.png");
	roadTexture = loadFromImage("image/road.png");
    
    renderClouds = { NULL };
    renderCastle = { NULL };
    renderbgForest = { NULL };
    renderBushes = { NULL };
    renderTavern = { NULL };
    renderRoad = { NULL };

    resetBackGround();
}

void Background::resetBackGround() {
    clouds_X = 0;
    castle_X = 0;
    bgForest_X = 0;
    foreBG_X = 0;

    atFurthestLeftX = false;
    atFurthestRightX = true;

    foreBG_Velocity = 6, bgForest_velocity = 5, castle_velocity = 2, clouds_Velocity = 1;

    clouds_VelX = 0;
    castle_VelX = 0;
    bgForest_VelX = 0;
    foreBG_VelX = 0;

    objectPlusVelocity = 0;
}

Background::~Background()
{
	free();
}

void Background::render(SDL_Renderer* renderer) {

    renderClouds = { clouds_X, 0,SCREEN_WIDTH,SCREEN_HEIGHT };
    while (renderClouds.x <= -SCREEN_WIDTH) {
        renderClouds.x += SCREEN_WIDTH;
    }
    SDL_RenderCopy(renderer, cloudsTexture, NULL, &renderClouds);
    if (renderClouds.x < 0) {
        renderClouds.x += SCREEN_WIDTH;
        SDL_RenderCopy(renderer, cloudsTexture, NULL, &renderClouds);
    }

    if (castle_X <= 0 && castle_X > -SCREEN_WIDTH) {
        renderCastle = { castle_X,0,SCREEN_WIDTH,SCREEN_HEIGHT };
        SDL_RenderCopy(renderer, castleTexture, NULL, &renderCastle);
    }

    renderbgForest = { bgForest_X,0,SCREEN_WIDTH,SCREEN_HEIGHT };
    while (renderbgForest.x <= -SCREEN_WIDTH) {
        renderbgForest.x += SCREEN_WIDTH;
    }
    SDL_RenderCopy(renderer, backgroundForestTexture, NULL, &renderbgForest);
    if (renderbgForest.x < 0) {
        renderbgForest.x += SCREEN_WIDTH;
        SDL_RenderCopy(renderer, backgroundForestTexture, NULL, &renderbgForest);
    }

    renderRoad = { foreBG_X,road_Y,SCREEN_WIDTH,roadWidth };
    while (renderRoad.x <= -SCREEN_WIDTH) {
        renderRoad.x += SCREEN_WIDTH;
    }
    SDL_RenderCopy(renderer, roadTexture, NULL, &renderRoad);
    if (renderRoad.x < 0) {
        renderRoad.x += SCREEN_WIDTH;
        SDL_RenderCopy(renderer, roadTexture, NULL, &renderRoad);
    }

    renderBushes = { foreBG_X,0,SCREEN_WIDTH,SCREEN_HEIGHT };
    while (renderBushes.x <= -SCREEN_WIDTH) {
        renderBushes.x += SCREEN_WIDTH;
    }
    SDL_RenderCopy(renderer, treeAndBushesTexture, NULL, &renderBushes);
    if (renderBushes.x < 0) {
        renderBushes.x += SCREEN_WIDTH;
        SDL_RenderCopy(renderer, treeAndBushesTexture, NULL, &renderBushes);
    }

    if (foreBG_X <= 0 && foreBG_X > -SCREEN_WIDTH) {
        renderTavern = { foreBG_X,0,SCREEN_WIDTH,SCREEN_HEIGHT };
        SDL_RenderCopy(renderer, tavernTexture, NULL, &renderTavern);
    }

}

void Background::renderCurrentFrame(SDL_Renderer* renderer) {
    render(renderer);
}

void Background::handledEvent(SDL_Event& e, SDL_Renderer* render)
{
    if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
    {
        switch (e.key.keysym.sym)
        {
        case SDLK_d:
            clouds_VelX -= clouds_Velocity;
            castle_VelX -= castle_velocity;
            bgForest_VelX -= bgForest_velocity;
            foreBG_VelX -= foreBG_Velocity;
            break;

        case SDLK_a:
            clouds_VelX += clouds_Velocity;
            castle_VelX += castle_velocity;
            bgForest_VelX += bgForest_velocity;
            foreBG_VelX += foreBG_Velocity;
            break;
        }
    }
    else if (e.type == SDL_KEYUP && e.key.repeat == 0)
    {
        switch (e.key.keysym.sym)
        {
        case SDLK_d:
            clouds_VelX += clouds_Velocity;
            castle_VelX += castle_velocity;
            bgForest_VelX += bgForest_velocity;
            foreBG_VelX += foreBG_Velocity;
            break;

        case SDLK_a:
            clouds_VelX -= clouds_Velocity;
            castle_VelX -= castle_velocity;
            bgForest_VelX -= bgForest_velocity;
            foreBG_VelX -= foreBG_Velocity;
            break;
        }
    }
}

void Background::move()
{
    clouds_X += clouds_VelX;
    castle_X += castle_VelX;
    bgForest_X += bgForest_VelX;
    foreBG_X += foreBG_VelX;


    if (foreBG_X < BGleftMostX) {
        clouds_X += clouds_Velocity;
        castle_X += castle_velocity;
        bgForest_X += bgForest_velocity;
        foreBG_X += foreBG_Velocity;

        objectPlusVelocity = 0;

        atFurthestLeftX = true;
        atFurthestRightX = false;
    }
    else if (foreBG_X >= BGrightMostX) {
        clouds_X -= clouds_Velocity;
        castle_X -= castle_velocity;
        bgForest_X -= bgForest_velocity;
        foreBG_X -= foreBG_Velocity;

        objectPlusVelocity = 0;

        atFurthestLeftX = false;
        atFurthestRightX = true;
    }
    else {
        objectPlusVelocity = foreBG_VelX;

        atFurthestLeftX = false;
        atFurthestRightX = false;
    }

}


void Background::free()
{
    freeTexture(cloudsTexture);
    freeTexture(castleTexture);
    freeTexture(backgroundForestTexture);
    freeTexture(treeAndBushesTexture);
    freeTexture(tavernTexture);
    freeTexture(roadTexture);
}

bool Background::isAtFurthestLeftX() {
    return atFurthestLeftX;
}

bool Background::isAtFurthestRightX() {
    return atFurthestRightX;
}

int Background::getObjectPlusVelocity() {
    return objectPlusVelocity;
}

int Background::getBGVelocity() {
    return foreBG_Velocity;
}

int Background::getFurthestLeftPoint() {
    return BGleftMostX;
}

