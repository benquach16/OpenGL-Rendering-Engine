#pragma once

#include "glframebuffer.h"

class GBufferFBO : public GLFramebuffer
{
public:
    void resize(int width, int height) override;
private:
    GLuint m_position;
    GLuint m_depth;
    GLuint m_albedo;
    GLuint m_normals;
};