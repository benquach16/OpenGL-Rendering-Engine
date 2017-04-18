#pragma once

#include <SDL/SDL.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

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
