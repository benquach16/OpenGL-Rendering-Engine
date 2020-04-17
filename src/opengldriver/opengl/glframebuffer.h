#pragma once

#include <GL/glew.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

class GLFramebuffer 
{
public:
    GLFramebuffer();
    ~GLFramebuffer();

    virtual void resize(int width, int height);
    void bind();

protected:
    void clean();
    int m_depth;
    int m_buffer;
    GLuint m_framebuffer;
};
