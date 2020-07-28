#pragma once

#include <GL/glew.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

class GLFramebuffer {
public:
    GLFramebuffer();
    virtual ~GLFramebuffer();

    virtual void resize(int width, int height);

    void bind();
    void resolve();
    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }

protected:
    virtual void createMultisampleBuffer(int width, int height);
    void checkStatus();
    void clean();
    int m_width;
    int m_height;
    GLuint m_framebuffer;
    GLuint m_multisampleFramebuffer;
};
