#include "renderer.h"


Renderer::Renderer()
{
	SDL_Init(SDL_INIT_VIDEO);
	screen = SDL_SetVideoMode(800, 600, 32, SDL_SWSURFACE);
	
}
