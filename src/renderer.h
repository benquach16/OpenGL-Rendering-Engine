#pragma once

#include "opengldriver.h"
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
	ScreenInfo m_screenInfo;
	SDL_Window *m_window;
	OpenGLDriver m_driver;
};
