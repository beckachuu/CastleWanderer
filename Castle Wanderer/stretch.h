#pragma once
//try stretch
#include "basic_init.h"


void stretch();

/////////////////////////////////////////////////////////////

void stretch()
{
	//Apply the image stretched
	SDL_Rect stretchRect;
	stretchRect.x = 0;
	stretchRect.y = 0;
	stretchRect.w = SCREEN_WIDTH;
	stretchRect.h = SCREEN_HEIGHT;
	SDL_BlitScaled(CurrentSurface, NULL, screenSurface, &stretchRect);

	//Update the surface
	SDL_UpdateWindowSurface(window);
	waitUntilKeyPressed();
}