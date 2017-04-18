#include "renderer.h"


Renderer::Renderer()
{
	SDL_Init(SDL_INIT_VIDEO);
	screen = SDL_SetVideoMode(800, 600, 32, SDL_SWSURFACE);
	#ifdef TEST_SDL_LOCK_OPTS
	EM_ASM("SDL.defaults.copyOnLock = false; SDL.defaults.discardOnLock = true; SDL.defaults.opaqueFrontBuffer = false;");
	#endif

	if (SDL_MUSTLOCK(screen)) SDL_UnlockSurface(screen);
}

Renderer::~Renderer()
{
	SDL_Quit();
}

void Renderer::run()
{
	SDL_Flip(screen);
}


