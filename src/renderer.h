#pragma once

#include <SDL2/SDL.h>
#include <GL/gl.h>
#include "opengldriver.h"
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
	int m_width;
	int m_height;
	SDL_Window *m_window;
	OpenGLDriver m_driver;
};
