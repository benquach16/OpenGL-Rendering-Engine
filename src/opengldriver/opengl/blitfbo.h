#pragma once

#include "glframebuffer.h"

class BlitFBO : public GLFramebuffer
{
public:
    BlitFBO();

    void resize(int width, int height) override;

    GLuint getAlbedo() const { return m_albedo; }
protected:
    GLuint m_albedo;
};
