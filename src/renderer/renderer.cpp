#include "renderer.h"
#include <iostream>

Renderer::Renderer()
{

    m_screenInfo.m_width = 1600;
    m_screenInfo.m_height = 900;
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        std::cerr << "failed to initialize SDL" << std::endl;
    m_window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        m_screenInfo.m_width, m_screenInfo.m_height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (!m_window)
        std::cerr << "failed to create sdl window" << std::endl;

#ifdef TEST_SDL_LOCK_OPTS
    EM_ASM("SDL.defaults.copyOnLock = false; SDL.defaults.discardOnLock = true; SDL.defaults.opaqueFrontBuffer = false;");
#endif

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    auto context = SDL_GL_CreateContext(m_window);
    if (!context)
        std::cerr << "failed to create opengl context" << std::endl;
    SDL_GL_SetSwapInterval(1);

    m_driver.initializeDriver();

    m_driver.resize(m_screenInfo);
}
void Renderer::setup()
{
    //setup render scene
    mesh = new Mesh;
    mesh->load("assets/monkey.obj");
}

Renderer::~Renderer()
{
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

void Renderer::run()
{
    setup();
    //run opengl scene
    float rot = 0.0f;
    bool exit = false;
    while (exit == false) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            // for some reason overly sensitive
            /*
			if(event.key.keysym.sym == SDLK_ESCAPE) {
				exit = true;
			}
			*/
            if (event.type == SDL_QUIT) {
                exit = true;
            }
        }
        m_driver.submit(mesh->getBuffer());

        glm::vec3 position(0.0f, 0.0f, 2.0);

        glm::mat4 view = glm::lookAt(
            position,
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 Projection = glm::perspective(glm::radians(40.0f), 16.0f / 9.0f, 0.1f, 200.f);

        //Projection = Projection * view;
        view = glm::rotate(view, rot, glm::vec3(0.0, 1.0, 0.0));
        rot += 0.001f;
        m_driver.setCameraPerspective(view, Projection, position);
        m_driver.render();
        SDL_GL_SwapWindow(m_window);
    }
}

static float lightVerts[] = {};

void Renderer::createLightVolume()
{
}
