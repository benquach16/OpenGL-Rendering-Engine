#include "glframebuffer.h"

GLFramebuffer::GLFramebuffer() : m_framebuffer(0)
{
}

GLFramebuffer::~GLFramebuffer()
{
    clean();
}

void GLFramebuffer::resize(int width, int height)
{
    clean();
    glGenFramebuffers(1, &m_framebuffer);

}

void GLFramebuffer::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
}

void GLFramebuffer::clean()
{
    if(m_framebuffer != 0) {
        glDeleteFramebuffers(1, &m_framebuffer);
        m_framebuffer = 0;
    }
}
