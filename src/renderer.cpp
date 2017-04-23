#include "renderer.h"
#include <iostream>


Renderer::Renderer()
{
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
		std::cerr << "failed to initialize SDL" << std::endl;
	window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
	if(!window)
		std::cerr << "failed to create sdl window" << std::endl;
	#ifdef TEST_SDL_LOCK_OPTS
	EM_ASM("SDL.defaults.copyOnLock = false; SDL.defaults.discardOnLock = true; SDL.defaults.opaqueFrontBuffer = false;");
	#endif
	auto context = SDL_GL_CreateContext(window);

	if(!context)
		std::cerr << "failed to create opengl context" << std::endl;
	glClearColor(0.1,0.1,0.7,1.0);
	glClear( GL_COLOR_BUFFER_BIT );
	glClearDepth(1.0f);

	glViewport(0, 0, 640, 480);

	SDL_GL_SwapWindow( window );
	

}

Renderer::~Renderer()
{
	SDL_Quit();
}

void Renderer::run()
{

}


