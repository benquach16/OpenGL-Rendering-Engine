#include "renderer.h"
#include <iostream>

Renderer::Renderer()
{
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
		std::cerr << "failed to initialize SDL" << std::endl;
	window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
	#ifdef TEST_SDL_LOCK_OPTS
	EM_ASM("SDL.defaults.copyOnLock = false; SDL.defaults.discardOnLock = true; SDL.defaults.opaqueFrontBuffer = false;");
	#endif
}

Renderer::~Renderer()
{
	SDL_Quit();
}

void Renderer::run()
{

}


