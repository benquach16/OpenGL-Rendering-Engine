#pragma once

#include "glframebuffer.h"

class ResolveFBO : public GLFramebuffer
{
public:
    ResolveFBO();

    void resize(int width, int height) override;

    GLuint getAlbedo() const { return m_albedo; }
protected:
    GLuint m_albedo;
};