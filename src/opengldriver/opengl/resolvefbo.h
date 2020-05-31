#pragma once

#include "glframebuffer.h"
#include "gbufferfbo.h"

class ResolveFBO : public GLFramebuffer
{
public:
    ResolveFBO(const GBufferFBO* fbo);

    void resize(int width, int height) override;

    GLuint getAlbedo() const { return m_albedo; }

protected:
    GLuint m_albedo;
    const GBufferFBO *m_gbufferFBO;
};