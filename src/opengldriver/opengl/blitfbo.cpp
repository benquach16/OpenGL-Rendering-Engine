#include "blitfbo.h"
#include <iostream>

using namespace std;

BlitFBO::BlitFBO()
{
}

void BlitFBO::resize(int width, int height)
{
    GLFramebuffer::resize(width, height);
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
    glGenTextures(1, &m_albedo);
    glBindTexture(GL_TEXTURE_2D, m_albedo);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_albedo, 0);

    GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

    checkStatus();
}
