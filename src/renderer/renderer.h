#pragma once

#include "../opengldriver/opengldriver.h"
#include "mesh.h"
#include <SDL2/SDL.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

class Renderer {
public:
    Renderer();
    ~Renderer();
    void setup();
    void run();
    void createLightVolume();

protected:
    ScreenInfo m_screenInfo;
    SDL_Window* m_window;
    OpenGLDriver m_driver;

    Mesh* mesh;
};
