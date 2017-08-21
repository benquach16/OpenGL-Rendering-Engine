#include "renderer.h"
#include <iostream>


Renderer::Renderer()
{

	m_screenInfo.m_width = 800;
	m_screenInfo.m_height = 600;
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
		std::cerr << "failed to initialize SDL" << std::endl;
	m_window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
								 m_screenInfo.m_width, m_screenInfo.m_height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
	if(!m_window)
		std::cerr << "failed to create sdl window" << std::endl;
	
	#ifdef TEST_SDL_LOCK_OPTS
	EM_ASM("SDL.defaults.copyOnLock = false; SDL.defaults.discardOnLock = true; SDL.defaults.opaqueFrontBuffer = false;");
	#endif
	auto context = SDL_GL_CreateContext(m_window);
	if(!context)
		std::cerr << "failed to create opengl context" << std::endl;
	SDL_GL_SetSwapInterval(1);

	m_driver.initializeDriver();

	m_driver.resize(m_screenInfo);

}

Renderer::~Renderer()
{
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

void Renderer::run()
{
	//run opengl scene

	glClear(GL_COLOR_BUFFER_BIT);
	m_driver.renderQuad();
	SDL_GL_SwapWindow( m_window );

	
}


