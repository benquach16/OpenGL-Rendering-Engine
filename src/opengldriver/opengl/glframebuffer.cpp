#include "glframebuffer.h"
#include "util/debug.h"
#include "util/util.h"

GLFramebuffer::GLFramebuffer()
    : m_framebuffer(0)
    , m_width(0)
    , m_height(0)
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
    GET_GL_ERROR("Error on creating FBO");
    m_width = width;
    m_height = height;
}
void GLFramebuffer::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
    GET_GL_ERROR("Error on binding FBO");
}

void GLFramebuffer::clean()
{
    if (m_framebuffer != 0) {
        glDeleteFramebuffers(1, &m_framebuffer);
        m_framebuffer = 0;
    }
}
