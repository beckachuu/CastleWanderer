#include "background.h"
#include "basic_init.h"
#include <SDL_image.h>

Background::Background(SDL_Renderer* renderer)
{
	cloudsTexture = loadFromFile("image/clouds.png",renderer);
    castleTexture = loadFromFile("image/castle.png", renderer);
	backgroundForestTexture = loadFromFile("image/background forest.png", renderer);
	treeAndBushesTexture = loadFromFile("image/tree and bushes.png", renderer);
	tavernTexture = loadFromFile("image/tavern.png", renderer);
	roadTexture = loadFromFile("image/road.png", renderer);

    clouds_X = 0;
    castle_X = 0;
    bgForest_X = 0;
    foreBG_X = 0;
    road_Y = SCREEN_HEIGHT - roadWidth;

    //Velocity
    clouds_VelX = 0;
    castle_VelX = 0;
    bgForest_VelX = 0;
    foreBG_VelX = 0;
    
    renderClouds = { NULL };
    renderCastle = { NULL };
    renderbgForest = { NULL };
    renderBushes = { NULL };
    renderTavern = { NULL };
    renderRoad = { NULL };
}

Background::~Background()
{
	free();
}

void Background::render(SDL_Renderer* renderer) {
    //Render and repeat
    renderClouds = { clouds_X, 0,SCREEN_WIDTH,SCREEN_HEIGHT };
    SDL_RenderCopy(renderer, cloudsTexture, NULL, &renderClouds);
    if (clouds_X < 0) {
        renderClouds.x += SCREEN_WIDTH;
        SDL_RenderCopy(renderer, cloudsTexture, NULL, &renderClouds);
        if (clouds_X < -SCREEN_WIDTH) {
            renderClouds.x += SCREEN_WIDTH;
            SDL_RenderCopy(renderer, cloudsTexture, NULL, &renderClouds);
        }
    }

    //Render without repeat
    renderCastle = { castle_X,0,SCREEN_WIDTH,SCREEN_HEIGHT };
    SDL_RenderCopy(renderer, castleTexture, NULL, &renderCastle);

    //Render and repeat
    renderbgForest = { bgForest_X,0,SCREEN_WIDTH,SCREEN_HEIGHT };
    SDL_RenderCopy(renderer, backgroundForestTexture, NULL, &renderbgForest);
    if (bgForest_X < 0) {
        renderbgForest.x += SCREEN_WIDTH;
        SDL_RenderCopy(renderer, backgroundForestTexture, NULL, &renderbgForest);
        if (bgForest_X < -SCREEN_WIDTH) {
            renderbgForest.x += SCREEN_WIDTH;
            SDL_RenderCopy(renderer, backgroundForestTexture, NULL, &renderbgForest);
            if (bgForest_X < -SCREEN_WIDTH) {
                renderbgForest.x += SCREEN_WIDTH;
                SDL_RenderCopy(renderer, backgroundForestTexture, NULL, &renderbgForest);
            }
        }
    }

    //Render and repeat
    renderRoad = { foreBG_X,road_Y,SCREEN_WIDTH,roadWidth };
    SDL_RenderCopy(renderer, roadTexture, NULL, &renderRoad);
    if (foreBG_X < 0) {
        renderRoad.x += SCREEN_WIDTH;
        SDL_RenderCopy(renderer, roadTexture, NULL, &renderRoad);
        if (foreBG_X < -SCREEN_WIDTH) {
            renderRoad.x += SCREEN_WIDTH;
            SDL_RenderCopy(renderer, roadTexture, NULL, &renderRoad);
            if (foreBG_X < -SCREEN_WIDTH) {
                renderRoad.x += SCREEN_WIDTH;
                SDL_RenderCopy(renderer, roadTexture, NULL, &renderRoad);
            }
        }
    }

    //Render and repeat
    renderBushes = { foreBG_X,0,SCREEN_WIDTH,SCREEN_HEIGHT };
    SDL_RenderCopy(renderer, treeAndBushesTexture, NULL, &renderBushes);
    if (foreBG_X < 0) {
        renderBushes.x += SCREEN_WIDTH;
        SDL_RenderCopy(renderer, treeAndBushesTexture, NULL, &renderBushes);
        if (foreBG_X < -SCREEN_WIDTH) {
            renderBushes.x += SCREEN_WIDTH;
            SDL_RenderCopy(renderer, treeAndBushesTexture, NULL, &renderBushes);
            if (foreBG_X < -SCREEN_WIDTH) {
                renderBushes.x += SCREEN_WIDTH;
                SDL_RenderCopy(renderer, treeAndBushesTexture, NULL, &renderBushes);
            }
        }
    }

    //Render without repeat
    renderTavern = { foreBG_X,0,SCREEN_WIDTH,SCREEN_HEIGHT };
    SDL_RenderCopy(renderer, tavernTexture, NULL, &renderTavern);

}

void Background::renderCurrentFrame(SDL_Renderer* renderer) {
    render(renderer);
}

void Background::handledEvent(SDL_Event& e, SDL_Renderer* render)
{
    //If a key was pressed
    if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
    {
        //Adjust the velocity
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

    //If a key was released
    else if (e.type == SDL_KEYUP && e.key.repeat == 0)
    {
        //Adjust the velocity
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

        atFurthestLeftX = true;
        atFurthestRightX = false;
    }
    else if (foreBG_X > BGrightMostX) {
        clouds_X -= clouds_Velocity;
        castle_X -= castle_velocity;
        bgForest_X -= bgForest_velocity;
        foreBG_X -= foreBG_Velocity;

        atFurthestLeftX = false;
        atFurthestRightX = true;
    }
    else {
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

bool Background::toFurthestLeftX() {
    return atFurthestLeftX;
}

bool Background::toFurthestRightX() {
    return atFurthestRightX;
}

int Background::getBGVelX() {
    return foreBG_VelX;
}

int Background::getforeBG_X() {
    return foreBG_X;
}

int Background::getFurthestLeftPoint() {
    return BGleftMostX;
}