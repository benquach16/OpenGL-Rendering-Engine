#pragma once

#include "glframebuffer.h"

class GBufferFBO : public GLFramebuffer {
public:
    void resize(int width, int height) override;

    GLuint getPosition() const { return m_position; }
    GLuint getDepth() const { return m_depth; }
    GLuint getAlbedo() const { return m_albedo; }
    GLuint getNormal() const { return m_normals; }

private:
    GLuint m_position;
    GLuint m_depth;
    GLuint m_albedo;
    GLuint m_normals;
};