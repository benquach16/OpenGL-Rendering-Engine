#pragma once

#include <SDL2/SDL.h>
#include <GL/gl.h>

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
	SDL_Window *window;
	SDL_Surface *screen;
};
