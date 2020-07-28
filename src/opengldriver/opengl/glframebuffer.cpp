#include "glframebuffer.h"
#include "util/debug.h"
#include "util/util.h"
#include <iostream>

using namespace std;

GLFramebuffer::GLFramebuffer()
    : m_framebuffer(0)
    , m_width(0)
    , m_height(0)
    , m_multisampleFramebuffer(0)
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
void GLFramebuffer::resolve()
{
    ASSERT(m_multisampleFramebuffer != 0, "Multisample fbo not initialized");
    ASSERT(m_framebuffer != 0, "Framebuffer class not intialized");
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_multisampleFramebuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_framebuffer);

    glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, m_width, m_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}

void GLFramebuffer::clean()
{
    if (m_framebuffer != 0) {
        glDeleteFramebuffers(1, &m_framebuffer);
        m_framebuffer = 0;
    }
    if (m_multisampleFramebuffer != 0) {
        glDeleteFramebuffers(1, &m_multisampleFramebuffer);
        m_multisampleFramebuffer = 0;
    }
}

void GLFramebuffer::checkStatus()
{
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        if (status == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT) {
            cerr << "GL_FRAMEBUFFER: incomplete attachment error" << endl;
        }
        if (status == GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT) {
            cerr << "GL_FRAMEBUFFER: incomplete missing attachment" << endl;
        }
        cerr << "framebuffer error" << endl;
    } else {
        cerr << "framebuffer success" << endl;
    }   
}

void GLFramebuffer::createMultisampleBuffer(int width, int height)
{
    ASSERT(m_multisampleFramebuffer == 0, "Multisample FBO is already initialized");
    glGenFramebuffers(1, &m_multisampleFramebuffer);
    GET_GL_ERROR("Error on creating multisample fbo");
}
