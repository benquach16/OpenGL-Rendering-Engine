#pragma once

#include "gbufferfbo.h"
#include "glframebuffer.h"

//FBO with pointer to GBuffer FBO for depth
class ResolveFBO : public GLFramebuffer {
public:
    ResolveFBO(const GBufferFBO* fbo);

    void resize(int width, int height) override;

    GLuint getAlbedo() const { return m_albedo; }
    GLuint getDepth() const { return m_gbufferFBO->getDepth(); }

protected:

    GLuint m_albedo;
    const GBufferFBO* m_gbufferFBO;
};