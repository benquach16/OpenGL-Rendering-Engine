#pragma once

#include <SDL/SDL.h>

class Renderer
{
public:
	Renderer();
	~Renderer();
	void setup();
	void run();
protected:

	SDL_Surface *screen;
};
