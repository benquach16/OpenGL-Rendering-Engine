#pragma once

#include "glframebuffer.h"

class ResolveFBO : public GLFramebuffer
{
public:
    ResolveFBO();

    void resize(int width, int height) override;
protected:
    GLuint m_albedo;
};