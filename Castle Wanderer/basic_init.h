#ifndef BASIC_INIT_H
#define BASIC_INIT_H

#include <iostream>
#include <cstring>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

const std::string title = "Castle Wanderer";
const int SCREEN_HEIGHT = 540;
const int SCREEN_WIDTH = 960;

enum knightposes { jump1, jump2, jump3, punch1, punch2, walk1, walk2, walk3, walk4, walk5, stand, total };


//Texture wrapper class
class MyTexture
{
public:
	//Initializes variables
	MyTexture();

	//Deallocates memory
	~MyTexture();

	//Loads image at specified path
	bool loadFromFile(std::string path);

	//Deallocates texture
	void free();

	//Set color modulation
	void setColor(Uint8 red, Uint8 green, Uint8 blue);

	//Set blending
	void setBlendMode(SDL_BlendMode blending);

	//💘💘💘💘💘💘💘 Set alpha modulation 💘💘💘💘💘💘💘
	void setAlpha(Uint8 alpha);

	//💘💘💘💘💘💘💘 Renders texture at given point 💘💘💘💘💘💘💘
	void render(int x, int y, SDL_Rect* clip = NULL);

	//Gets image dimensions
	int getWidth();
	int getHeight();

private:
	//The actual hardware texture
	SDL_Texture* mTexture[total];

	//Image dimensions
	int mWidth;
	int mHeight;
};


SDL_Renderer* initSDL();
void logError(std::ostream& out, const std::string& ms, bool fatal);
void close(SDL_Texture* screen);
bool loadMedia(SDL_Texture* picture[]);
SDL_Texture* loadIMG(const std::string& path);

#endif